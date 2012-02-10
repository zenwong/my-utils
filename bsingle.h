#ifndef BSINGLE_H
#define BSINGLE_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bstrlib.h>

typedef struct bdict {
  bstring *key;
  bstring *val;
  int index;
} bdict;


typedef struct blist {
  bstring *entry;
  int index;
} blist;

bstring enclose(char *str){
  bstring buf = bfromcstr("{{");
  bcatcstr(buf, str);
  bcatcstr(buf, "}}");
  return buf;
}

bstring breadfile(char *filename){
  FILE *fp = fopen(filename, "r");
  bstring contents = bread((bNread)fread,fp);
  return contents;
}

bstring btoday(){
  char *outstr = malloc(15);
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);
  strftime(outstr, 15, "%d-%m-%Y", tm);
  bstring date = bfromcstr(outstr);
  return date;
}

int nwords(char str[]){
  int count = 0;
  for(int i = 0; str[i] != 0; ++i)
    if(str[i] == ',') ++count;
  return count + 1;
}

int num_digits_in_int(int x){
  if (x < 0) x = -x;
  int count = 1;
  x = x / 10;
  while( x > 0){
    count++;
    x = x / 10;
  }
  return count;
}

bstring itoa(int num){
  char *buf = malloc(num_digits_in_int(num));
  sprintf(buf, "%d", num);
  bstring str = bfromcstr(buf);
  return str;
}

/****************************************************************************************/

bdict *bdict_init(){
  bdict *d;
  d      = malloc(sizeof(d->index) + sizeof(d->key) + sizeof(d->val));
  d->key = malloc(sizeof(d->key));
  d->val = malloc(sizeof(d->val));
  d->index = 0;
  return d;
}

void bdict_add(bdict *d, bstring key, bstring val){
  d->key = realloc(d->key, (d->index + 1) * sizeof(key));
  d->val = realloc(d->val, (d->index + 1) * sizeof(val));
  d->key[d->index] = key;
  d->val[d->index] = val;
  d->index++;
}

void bdict_addc(bdict *d, char *key, char *val){
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
    bdict_add(d, ll->entry[k], ll->entry[v]);
    v += 2;
  }
  return d;
}

/****************************************************************************************/

blist *blist_init(){
  blist *d = malloc(sizeof(d->index) + sizeof(d->entry));
  d->entry = malloc(sizeof(d->entry));
  d->index = 0;
  return d;
}

void blist_add(blist *d, bstring value){
  d->entry = realloc(d->entry, (d->index + 1) * sizeof(value));
  d->entry[d->index] = value;
  d->index++;
}

void blist_addc(blist *d, char *value){
  d->entry  = realloc(d->entry, (d->index + 1) * sizeof(value));
  d->entry[d->index] = bfromcstr(value);
  d->index++;
}

bstring flatten(blist *ll){
  bstring buf   = bstrcpy(ll->entry[0]);
  bstring space = bfromcstr(" ");
  bconcat(buf, space);
  for(int i = 1; i < ll->index; i++){
    bconcat(buf, ll->entry[i]);
    bconcat(buf, space);
  }
  return buf;
}

bstring htmlize(blist *ll){
  bstring html = bfromcstr("<html>");
  bcatcstr(html, "<ol>");
  for(int i = 0; i < ll->index; i++){
    bcatcstr(html, "<li>");
    bconcat( html, ll->entry[i]);
    bcatcstr(html, "</li>");
  }
  bcatcstr(html, "</ol>");
  bcatcstr(html, "</html>");
  return html;
}

void blist_foreach(blist *ll){
  for(int i = 0; i < ll->index; i++)
    printf("%s\n", ll->entry[i]->data);
}

/****************************************************************************************/

bdict *extract_url_params(char *str){
  blist *ll = blist_init();
  int len = strlen(str) + 1;
  char tmp[len];
  strcpy(tmp, str);
  char *result;
  result = strtok(tmp, "&");
  while(result != NULL){
    blist_add(ll, result);
    result = strtok(NULL, "&");
  }

  bdict *d = bdict_init();
  int n;
  char buf[n];
  for(int i = 0; i < ll->index; i++){
    n = strlen(ll->entry[i]) + 1;
    strcpy(buf, ll->entry[i]);
    char *key = strtok(buf,  "=");
    char *val = strtok(NULL, "=");
    bdict_add(d, key, val);
  }
  return d;
}

void split_by_equals(blist *ll, bstring str){
  struct bstrList *equals = bsplit(str, '=');
  for(int i = 0; i < equals->qty; i++){
    blist_add(ll, equals->entry[i]);
  }
}

bdict *get_url_params(bstring str){
  blist *l = blist_init();
  struct bstrList *ands = bsplit(str, '&');
  for(int i = 0; i < ands->qty; i++)
    split_by_equals(l, ands->entry[i]);
  bdict *d = blist_to_bdict(l);
  return d;
}

bdict *get_url_paramsc(char *str){
  blist *l = blist_init();
  bstring tmp = bfromcstr(str);
  struct bstrList *ands = bsplit(tmp, '&');
  for(int i = 0; i < ands->qty; i++)
    split_by_equals(l, ands->entry[i]);
  bdict *d = blist_to_bdict(l);
  return d;
}

bdict *get_url_params_const(const char *str){
  blist *l = blist_init();
  bstring tmp = bfromcstr(str);
  struct bstrList *ands = bsplit(tmp, '&');
  for(int i = 0; i < ands->qty; i++)
    split_by_equals(l, ands->entry[i]);
  bdict *d = blist_to_bdict(l);
  return d;
}

bstring url_decode(char *str){
  bstring plus  = bfromcstr("+");
  bstring comma = bfromcstr("%2C");
  bstring bstr  = bfromcstr(str);
  bstring rpl1  = bfromcstr(" ");
  bstring rpl2  = bfromcstr(",");
  bfindreplace(bstr, plus,  rpl1, 0);
  bfindreplace(bstr, comma, rpl2, 0);
  return bstr;
}
#endif
