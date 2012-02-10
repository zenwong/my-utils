#pragma once
#define BSTRING_H
#ifndef BSTRING_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bstrlib.h>
#include <zen/bdict.h>
#include <zen/blist.h>

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

#endif
