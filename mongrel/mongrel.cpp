#include <zstring.hpp>
#include <zhelpers.hpp>
#include <zmq.hpp>
#include <boost/algorithm/string.hpp>
using namespace std;
using namespace zmq;
using namespace boost;

struct request{
  string uuid, conn;
};

void reply(socket_t& pub, request& r, string body){
  ostringstream resp, msg;

  resp << "HTTP/1.1 200 OK\r\n" << "Content-Length: " << body.size() << "\r\n\r\n" << body;
  msg  <<  r.uuid << " " << r.conn.size() << ":" << r.conn << ", " << resp.str();

  s_send(pub, msg.str());
}

request parse(string netstr){
  map<string,string> m;
  vector<string> t, t2;
  split(t,  netstr,  is_any_of("\""));
  split(t2, t.at(0), is_any_of(" "));

  for(auto token : t){
    if( !strcasecmp(token.c_str(), ":") == 0 )
      cout << token << '\n';
  }
  cout << "===========================================================\n";

  request r;
  r.uuid = t2.at(0);
  r.conn = t2.at(1);

  //m.insert( make_pair("uuid", t2.at(0)) );
  //m.insert( make_pair("conn", t2.at(1)) );
  //m.insert( make_pair("path", t2.at(2)) );

  for(int i = 0; i < t.size(); i++){
    if(strcmp(t.at(i).c_str(), "x-forwarded-for") == 0)
      m.insert( make_pair("ip", t.at(i + 2)) );

    else if(strcmp(t.at(i).c_str(), "cookie") == 0)
      m.insert( make_pair("cookie", t.at(i + 2)) );

    else if(strcmp(t.at(i).c_str(), "METHOD") == 0)
      m.insert( make_pair("method", t.at(i + 2)) );
    
    else if(strcmp(t.at(i).c_str(), "QUERY") == 0)
      m.insert( make_pair("query", t.at(i + 2)) );
  }
  return r;
}

int main(int argc, char **argv){
  context_t ctx(1);
  socket_t  pull(ctx, ZMQ_PULL);
  socket_t  pub(ctx, ZMQ_PUB);

  string uuid = "82209006-86FF-4982-B5EA-D1E29E55D481";
  //pub.setsockopt(ZMQ_IDENTITY, "node", 4);

  pull.connect("tcp://*:9999");
  pub.connect ("tcp://*:9998");

  while(1){

    string netstr =  s_recv(pull);
    cout << netstr << "\n";
    cout << "===========================================================\n";

    request r = parse(netstr);
    reply(pub, r, "hello from handler");

  }
  sleep(1);
  return 0;
}
