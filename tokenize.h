#define TOKENIZE_H
#ifndef TOKENIZE_H
#include <zen/slist.h>
#include <zen/sdict.h>

slist *tokenize(char *str, char *delim){
  slist *ll = slist_init();
  int len = strlen(str) + 1;
  char tmp[len];
  strcpy(tmp, str);

  char *result;
  result = strtok(tmp, delim);
  while(result != NULL){
    slist_add(ll, result);
    result = strtok(NULL, delim);
  }
  return ll;
}

sdict *parse_query_list(slist *ll){
  sdict *d = sdict_init();
  int len;
  char tmp[len];
  for(int i = 0; i < ll->index; i++){
    len = strlen(ll->entry[i]) + 1;
    strcpy(tmp, ll->entry[i]);
    char *key = strtok(tmp,  "=");
    char *val = strtok(NULL, "=");
    sdict_add(d, key, val);
  }
  return d;
}

sdict *extract_url_parameters(char *str){
  slist *ll = slist_init();
  int len = strlen(str) + 1;
  char tmp[len];
  strcpy(tmp, str);
  char *result;
  result = strtok(tmp, "&");
  while(result != NULL){
    slist_add(ll, result);
    result = strtok(NULL, "&");
  }

  sdict *d = sdict_init();
  int n;
  char buf[n];
  for(int i = 0; i < ll->index; i++){
    n = strlen(ll->entry[i]) + 1;
    strcpy(buf, ll->entry[i]);
    char *key = strtok(buf,  "=");
    char *val = strtok(NULL, "=");
    sdict_add(d, key, val);
  }
  return d;
}
#endif
