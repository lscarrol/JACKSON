#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "stack.c"

typedef struct D_NODE {
  bool isLeaf;
  char* op;
  int chil;
  struct D_NODE* next;
  struct D_NODE* parent;
  struct D_NODE* left;
  struct D_NODE* right;
} D_NODE;

typedef struct T_TREE {
  int stackSize;
  D_NODE* root;
} T_TREE;


typedef struct T_STACK {
    int size;
    D_NODE* top;
    struct T_STACK* next;
} T_STACK;

T_TREE* createTree1();

D_NODE* createNode1();

D_NODE* createNode(T_TREE* tree, char* op, D_NODE* left, D_NODE* right);

D_NODE* createTree(S_STACK* que, T_TREE* tree);

void dispTree(D_NODE* root);

void pushT(T_STACK* stack, D_NODE* node);

D_NODE* popT(T_STACK* stack);

char* peekT(T_STACK* stack);
