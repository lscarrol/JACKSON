#include <stdlib.h>
#include <stdio.h>


typedef struct L_NODE {
  int lineno;
  int colno;
  int scope;
  struct L_NODE* prev;
  struct L_NODE* next;
} L_NODE;


void insertList(L_NODE** head, int line, int col, int scope);

void dispList(L_NODE** head);
