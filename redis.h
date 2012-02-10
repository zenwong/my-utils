#ifndef REDIS_H
#define REDIS_H
#include <hiredis/hiredis.h>
#include <zen/zstring.h>

static redisContext *c;
static redisReply   *r;

int longest_string(redisReply *r){
  int longest_string = 0;
  for(int i = 0; i < r->elements; i++){
    if(longest_string < r->element[i]->len)
      longest_string = r->element[i]->len;
  }
  return longest_string;
}

int total_length(redisReply *r){
  int len = 0;
  for(int i = 0; i < r->elements; i++)
    len += r->element[i]->len;
  return len;
}

void setvid(char *title, char *desc, char *actress, char *tags){
  r = redisCommand(c, "incr vid");
  int id = r->integer;
  freeReplyObject(r);
  char *date        = today();
  char **act_tokens = split(actress, ',');
  char **tag_tokens = split(tags,    ',');
  int nactress      = nwords(actress);
  int ntags         = nwords(tags);
  int i;
  r = redisCommand(c, "multi");
  r = redisCommand(c, "hmset vid.%d title %s desc %s date %s", id, title, desc, date);
  r = redisCommand(c, "zincrby vid.list 1 %d", id);
  for(i = 0; i < nactress; i++){
    r = redisCommand(c, "sadd vid.%d.actress %s", id, act_tokens[i]);
    r = redisCommand(c, "zincrby actress.list 1 %s",  act_tokens[i]);
  }
  for(i = 0; i < ntags; i++){
    r = redisCommand(c, "sadd vid.%d.tags %s", id, tag_tokens[i]);
    r = redisCommand(c, "zincrby tag.list 1 %s",   tag_tokens[i]);
  }
  r = redisCommand(c, "exec");
  freeReplyObject(r);
}

#endif
