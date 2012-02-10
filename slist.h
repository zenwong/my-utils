#define SLIST_H
#ifndef SLIST_H
#include <zstring.h>

typedef struct slist {
  char **entry;
  int index;
} slist;

slist *slist_init(){
  slist *ll = malloc(sizeof(slist) + sizeof(ll->entry));
  ll->entry = malloc(sizeof(ll->entry));
  ll->index = 0;
  return ll;
}

void slist_add(slist *ll, char *str){
  ll->entry = realloc(ll->entry, (ll->index + 1) * sizeof(str));
  ll->entry[ll->index] = malloc(strlen(str) + 1);
  strcpy(ll->entry[ll->index], str);
  ll->index++;
}

void slist_foreach(slist *ll){
  for(int i = 0; i < ll->index; i++)
    printf("%s\n", ll->entry[i]);
}

char *senclose(char *str){
  char *buf = malloc(strlen(str) + 5);
  sprintf(buf, "{{%s}}", str);
  return buf;
}
#endif

