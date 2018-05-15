#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "DAG.c"
#include "hashtable.c"

bool inMain = false;
bool inFunc = false;
bool inFor = false;
bool inWhile = false;
bool inSwitch = false;
bool inCase = false;
bool inOtherWise = false;
bool inIf = false;
bool inThen = false;
bool inElse = false;
int count = 0;
int funcc = 0;
int inco = 0;
int lcount = 0;
int forcount = 0;
char* foradder;
int wcount = 0;
int mainco = 0;
char* funcadder;
bool inmem;
int scount = 0;

typedef struct I_NODE {
  D_NODE* root;
  char* express;
  int lcount;
  struct I_NODE* next;
} I_NODE;


I_NODE* createINode();

I_NODE* gen(D_NODE* roots[]);

char* append(char* str1, char* str2);

char* genExp(D_NODE* root);

char* simplify(D_NODE* root, char* currs);
