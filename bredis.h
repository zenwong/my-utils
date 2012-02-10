#ifndef BREDIS_H
#define BREDIS_H
#include <zen/bsingle.h>
#include <hiredis/hiredis.h>

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
  bstring btitle   = bfromcstr(title);
  bstring bdesc    = bfromcstr(desc);
  bstring bactress = bfromcstr(actress);
  bstring btags    = bfromcstr(tags);
  bstring bdate    = btoday();  
  struct bstrList *at = bsplit(bactress, ',');
  struct bstrList *tt = bsplit(btags,    ',');

  r = redisCommand(c, "hmset vid.%d title %s desc %s date %s", id, btitle->data, bdesc->data, bdate->data);
  freeReplyObject(r);
  r = redisCommand(c, "zincrby vid.list 1 %d", id);
  freeReplyObject(r);
  
  for(int i = 0; i < at->qty; i++){
    btrimws(at->entry[i]);
    r = redisCommand(c, "sadd vid.%d.actress %s", id, at->entry[i]->data);
    freeReplyObject(r);
    r = redisCommand(c, "zincrby actress.list 1 %s",  at->entry[i]->data);
    freeReplyObject(r);
  }

  for(int i = 0; i < tt->qty; i++){
    btrimws(tt->entry[i]);
    r = redisCommand(c, "sadd vid.%d.tags %s", id, tt->entry[i]->data);
    freeReplyObject(r);
    r = redisCommand(c, "zincrby tag.list 1 %s",   tt->entry[i]->data);
    freeReplyObject(r);
  }
}

void bdict_setvid(bdict *d){
  setvid(d->val[0]->data, d->val[1]->data, d->val[3]->data, d->val[4]->data);
}

bstring bdictfind(bdict *d, bstring lookup){
  for (int i = 0; i < d->index; i++){
    if(bstrcmp(lookup, d->key[i]) == 0){
      return d->val[i];
      break;
    }
  }
  return NULL;
}

bdict *bdict_getvid(int id){
  bdict *ht = bdict_init();
  r = redisCommand(c, "hgetall vid.%d", id);
  int v = 1;
  for(int k = 0; k < r->elements; k += 2){
    bdict_addc(ht, r->element[k]->str, r->element[v]->str);
    v += 2;
  }
  freeReplyObject(r);

  r = redisCommand(c, "smembers vid.%d.actress", id);
  bstring actress = bfromcstr(r->element[0]->str);
  bcatcstr(actress, " ");
  for(int i = 1; i < r->elements; i++){
    bcatcstr(actress, r->element[i]->str);
    bcatcstr(actress, " ");
  }
  bdict_addc(ht, "actress", actress->data);
  freeReplyObject(r);

  r = redisCommand(c, "smembers vid.%d.tags", id);
  bstring tags = bfromcstr(r->element[0]->str);
  bcatcstr(tags, " ");
  for(int i = 1; i < r->elements; i++){
    bcatcstr(tags, r->element[i]->str);
    bcatcstr(tags, " ");
  }
  bdict_addc(ht, "tags", tags->data);
  freeReplyObject(r);

  return ht;
}
#endif
