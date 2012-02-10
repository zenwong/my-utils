#ifndef BLIST_H
#define BLIST_H
#include <zen/bstrlib.h>

typedef struct {
  bstring *entry;
  int index;
} blist;

blist *blist_init(){
  blist *d    = malloc(sizeof(d->index) + sizeof(d->entry));
  d->entry = malloc(sizeof(d->entry));
  d->index = 0;
  return d;
}

void blist_add(blist *d, char *value){
  d->entry  = realloc(d->entry, (d->index + 1) * sizeof(value));
  d->entry[d->index] = bfromcstr(value);
  d->index++;
}

void blist_addb(blist *d, bstring value){
  d->entry = realloc(d->entry, (d->index + 1) * sizeof(value));
  bassign(d->entry[d->index], value);
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

#endif
