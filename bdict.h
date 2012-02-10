#ifndef BDICT_H
#ifndef BDICT_H
#include <zen/bstrlib.h>
#include <zen/blist.h>

typedef struct {
  bstring *key;
  bstring *val;
  int index;
} bdict;

bdict *bdict_init(){
  bdict *d;
  d      = malloc(sizeof(d->index) + sizeof(d->key) + sizeof(d->val));
  d->key = malloc(sizeof(d->key));
  d->val = malloc(sizeof(d->val));
  d->index = 0;
  return d;
}

void bdict_add(bdict *d, char *key, char *val){
  d->key = realloc(d->key, (d->index + 1) * sizeof(key));
  d->val = realloc(d->val, (d->index + 1) * sizeof(val));
  d->key[d->index] = bfromcstr(key);
  d->val[d->index] = bfromcstr(val);
  d->index++;
}

void bdict_foreach(bdict *d){
  for(int i = 0; i < d->index; i++)
    printf("%s\t\t%s\n", d->key[i]->data, d->val[i]->data);
}

bstring bdictparse(char *filename, bdict *d){
  bstring contents = breadfile(filename);
  bstring title    = bfromcstr("{{title}}");
  bstring desc     = bfromcstr("{{desc}}");
  bstring date     = bfromcstr("{{date}}");
  bstring actress  = bfromcstr("{{actress}}");
  bstring tags     = bfromcstr("{{tags}}");
  bfindreplace(contents, title,   d->val[0], 0);
  bfindreplace(contents, desc,    d->val[1], 0);
  bfindreplace(contents, date,    d->val[2], 0);
  bfindreplace(contents, actress, d->val[3], 0);
  bfindreplace(contents, tags,    d->val[4], 0);
  return contents;
}

bdict *blist_to_bdict(blist *ll){
  bdict *d = bdict_init();
  int v = 1;
  for(int k = 0; k < ll->index; k += 2){
    bdict_add(d, ll->key[i], ll->val[v]);
    v += 2;
  }
  return d;
}
#endif
