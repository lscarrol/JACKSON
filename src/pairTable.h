#include <stdlib.h>
#include <stdio.h>

typedef struct pairNode {
  char* left;
  char* right;
  int line;
  struct pairNode* next;
} pairNode;

void insertPair(pairNode** head, char* left, char* right, int line);

void dispPairs(pairNode** head);

void checkType(pairNode** head);
