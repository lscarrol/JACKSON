#include "pairTable.h"


void insertPair(pairNode** head, char* left, char* right, int line) {
  pairNode* node = (pairNode*) malloc(sizeof(pairNode));
  pairNode *last = *head;
  node->left = left;
  node->right = right;
  node->line = line;
  node->next = NULL;
  if (*head == NULL){
      *head = node;
      return;
  }
  while (last->next != NULL) {last = last->next;}
  last->next = node;
}

void dispPairs(pairNode** head) {
  printf("\n\nLeft : Right\n");
  pairNode* begin = *head;
  while (begin->next != NULL) {
    printf("%s : %s   |  line: %d\n", begin->left, begin->right, begin->line);
    begin = begin->next;
  }
}
