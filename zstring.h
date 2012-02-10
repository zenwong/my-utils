#ifndef ZSTRING_H
#define ZSTRING_H
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
typedef char * string;

static char *abc = "1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int rnum(){
  int rn = rand() % 62;
  return rn;
}

char *rstr(int len){
  //srand((time(NULL) & 0xFFFF));
  char *buf = malloc(len + 1);
  for(int i = 0; i < len; i++)
    buf[i] = abc[rnum()];
  return buf;
}

string today(){
  string outstr = malloc(15);
  time_t t;
  struct tm *tm;
  t = time(NULL);
  tm = localtime(&t);
  strftime(outstr, 15, "%d-%m-%Y", tm);
  return outstr;
}

int nwords(char str[]){
  int count = 0;
  for(int i = 0; str[i] != 0; ++i)
    if(str[i] == ',') ++count;
  return count + 1;
}

int filesize(const char *filename){
  struct stat fstat;
  stat(filename, &fstat);
  return fstat.st_size;
}

char *readfile(const char *filename){
  int sz = filesize(filename);
  FILE *fp = fopen(filename, "r");
  char *buffer = (char *)malloc(sz + 1);
  fread(buffer,1,sz,fp);
  buffer[sz] = '\0';
  return buffer;
  fclose(fp);
  free(buffer);
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

char *itoa(int num){
  char *buf = malloc(num_digits_in_int(num));
  sprintf(buf, "%d", num);
  return buf;
}

char **split(char *str, char delimiter) {
  char *tempWords		= NULL;
  char **tempStoreWord = NULL;

  int i, length = strlen(str);
  int a, b, flag = 0;

  a = b = 1;
  for(i = 0; i < length; i++) {
    if(str[i] == delimiter) {
      if(flag) {
	tempWords[a - 1] = '\0';

	tempStoreWord = (char **) realloc(tempStoreWord, b * sizeof(char *));
	tempStoreWord[b++ - 1] = tempWords;

	a = 1;
	flag = 0;
      }
    } else {
      if(flag) {
	tempWords = (char *) realloc(tempWords, (a + 1) * sizeof(char));
	tempWords[a++ - 1] = str[i];
      } else {
	tempWords = (char *) calloc(1, sizeof(char));
	tempWords[a++ -1] = str[i];

	flag = 1;
      }
    }
  }

  if(flag) {
    tempWords[a++] = '\0';

    tempStoreWord = (char **) realloc(tempStoreWord, b * sizeof(char *));
    tempStoreWord[b - 1] = tempWords;

    flag = 0;
  }

  return tempStoreWord;
}

extern char* string_replace (const char* src, const char* find, const char* replacement);
extern char* string_replace_n (const char* src, const char* find, const char* replacement, size_t n);

char* string_replace (const char* src, const char* find, const char* replacement){
    //fprintf (stderr, "string_replace: replacing all [%s] with [%s] in [%s]\n", find, replacement, src);
    return string_replace_n (src, find, replacement, (size_t)-1);
}

char* string_replace_n (const char* src, const char* find, const char* replacement, size_t n){
    //fprintf(stderr, "string_replace_n: replacing first [%zu] [%s] with [%s] in [%s]\n", n, find, replacement, src);

    const char* srcp=src;
    const char* needle=NULL;
    size_t str_len=strlen(src);
    size_t replace_len=strlen(replacement);
    size_t search_len=strlen(find);
    size_t newl=0;

    char* new=malloc(str_len+1);
    if (!new) {
        fprintf(stderr, "Error: string_replace: Out of memory\n");
        return NULL;
    }
    while (n-- && (needle=strstr(srcp,find))) {
        str_len+=replace_len-search_len;
        char* rp=realloc(new, str_len+1);
        if (!rp) {
            fprintf(stderr, "Error: string_replace: Out of memory\n");
            free(new);
            return NULL;
        } else {
            new = rp;
        }
        size_t skip_len=needle-srcp;
        memcpy(new+newl, srcp, skip_len);
        memcpy(new+newl+skip_len, replacement, replace_len);
        newl+=skip_len+replace_len;
        srcp=needle+search_len;
    }
    strcpy(new+newl, srcp);

    return new;
}

char *enclose_in_brackets(char *str){
  char *buf = malloc(strlen(str) + 5);
  sprintf(buf, "{{%s}}", str);
  return buf;
}

/****************************************************************************************/

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

/****************************************************************************************/
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

/****************************************************************************************/

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

char *url_decode(char *str){
  char *tmp = string_replace(str, "%2C", ",");
  tmp = string_replace(tmp, "+", " ");
  return tmp;
}

/****************************************************************************************/
#endif
