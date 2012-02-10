#pragma once

bdict *extract_url_params(char *str){
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
