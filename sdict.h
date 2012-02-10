#define SDICT_H
#ifndef SDICT_H
#include <zstring.h>

typedef struct sdict {
  char **key;
  char **val;
  int index;
} sdict;

sdict *sdict_init(){
  sdict *d = malloc(sizeof(sdict) + sizeof(d->key) + sizeof(d->val) + sizeof(d->index));
  d->key   = malloc(sizeof(d->key));
  d->val   = malloc(sizeof(d->val));
  d->index = 0;
  return d;
}

void sdict_add(sdict *d, char *key, char *val){
  d->key = realloc(d->key, (d->index + 1) * sizeof(key));
  d->val = realloc(d->val, (d->index + 1) * sizeof(val));
  d->key[d->index] = malloc(strlen(key) + 1);
  d->val[d->index] = malloc(strlen(val) + 1);
  strcpy(d->key[d->index], key);
  strcpy(d->val[d->index], val);
  d->index++;
}

void sdict_foreach(sdict *d){
  for(int i = 0; i < d->index; i++)
    printf("%s\t\t%s\n", d->key[i], d->val[i]);
}

char *sdict_lookup(sdict *d, char *str){
  for(int i = 0; i < d->index; i++)
    if(strcmp(d->key[i], str) == 0) return d->val[i];
  return NULL;
}
#endif
