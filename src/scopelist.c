#include "scopelist.h"


void insertList(L_NODE** head, int line, int col, int scope) {
  L_NODE* iNode = (L_NODE*) malloc(sizeof(L_NODE));
  L_NODE *last = *head;
  iNode->lineno = line;
  iNode->colno = col;
  iNode->scope = scope;
  iNode->next = NULL;
  if (*head == NULL){
    iNode->prev = NULL;
    *head = iNode;
    return;
  }
  while (last->next != NULL) {last = last->next;}
  last->next = iNode;
  iNode->prev = last;
}

void dispList(L_NODE** head) {
  printf("LINE : COL : SCOPE\n");
  L_NODE* begin = *head;
  while (begin->next != NULL) {
    if (begin->lineno >= 10) {
      printf("%d", begin->lineno);
    } else {
      printf("%d", begin->lineno);
      printf("%s", " ");
    }
    printf("     ");
    if (begin->colno >= 10) {
      printf("%d", begin->colno);
    } else {
      printf("%d", begin->colno);
      printf("%s", " ");
    }
    printf("    ");
    printf("%d\n", begin->scope);
    begin = begin->next;
  }
}

