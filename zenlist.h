#include <stdio.h>  
#include <stdlib.h>

typedef struct {
  char** words;
  size_t nWords;
  size_t size;
  size_t block_size;
} list;

list* create_list(size_t block_size) {
  list* pWordList = malloc(sizeof(list));
  if (NULL != pWordList) {
    pWordList->nWords = 0;
    pWordList->size = block_size;
    pWordList->block_size = block_size;
    pWordList->words = malloc(sizeof(char*)*block_size);
    if (NULL == pWordList->words) {
      free(pWordList);
      return NULL;    
    }
  }
  return pWordList;
}

void delete_list(list* pWordList) {
  free(pWordList->words);
  free(pWordList);
}

int add_list(list* pWordList, char* word) {
  size_t nWords = pWordList->nWords;
  if (nWords >= pWordList->size) {
    size_t newSize = pWordList->size + pWordList->block_size;
    void* newWords = realloc(pWordList->words, sizeof(char*)*newSize); 
    if (NULL == newWords)
      return 0;
    else {    
      pWordList->size = newSize;
      pWordList->words = (char**)newWords;
    }
  }
  pWordList->words[nWords] = word;
  ++pWordList->nWords;
  return 1;
}

char** list_start(list* pWordList) {
  return pWordList->words;
}

char** list_end(list* pWordList) {
  return &pWordList->words[pWordList->nWords];
}
