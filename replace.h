#include <sys/types.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
extern char* string_replace (const char* src, const char* find, const char* replacement);
extern char* string_replace_n (const char* src, const char* find, const char* replacement, size_t n);

char* string_replace (const char* src, const char* find, const char* replacement){
  return string_replace_n (src, find, replacement, (size_t)-1);
}

char* string_replace_n (const char* src, const char* find, const char* replacement, size_t n){
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

void zreplace_n(const char* src, const char* find, const char* replacement, size_t n){
  const char* srcp = src;
  const char* needle = NULL;
  size_t str_len = strlen(src);
  size_t replace_len = strlen(replacement);
  size_t search_len = strlen(find);
  size_t newl = 0;

  while (n-- && (needle=strstr(srcp,find))) {
    str_len += replace_len-search_len;
    char* rp=realloc(new, str_len+1);
    new = rp;
    size_t skip_len = needle - srcp;
    memcpy(new + newl, srcp, skip_len);
    memcpy(new + newl + skip_len, replacement, replace_len);
    newl += skip_len + replace_len;
    srcp = needle + search_len;
  }
  strcpy(new + newl, srcp);
}

char * strrep(char *str, char *o_s, char *n_s){
  char *newstr = NULL;
  char *c = NULL;
   
  if ((c = strstr(str, o_s)) == NULL) {
    return str;
  }

  if ((newstr = (char *) malloc((int) sizeof(str) -
	  (int) sizeof(o_s) +
	  (int) sizeof(n_s) + 1)) == NULL) {
    printf("ERROR: unable to allocate memory\n");
    return NULL;
  }

  strncpy(newstr, str, c-str);  
  sprintf(newstr+(c-str), "%s%s", n_s, c+strlen(o_s));

  return newstr;
}
