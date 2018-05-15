#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#define STACK_MAX_SIZE 1000

char* operators[18];
int precedences[18];


//DEFINE STACK
typedef struct S_NODE {
    char* data;
    struct S_NODE* next;
    struct S_NODE* prev;
} S_NODE;

typedef struct S_STACK {
  int size;
  S_NODE* top;
  S_NODE* last;
} S_STACK;


//FUNCTIONS

//PUSH
void push(S_STACK* stack, char* data);

//POP
S_NODE* pop(S_STACK* stack);

S_NODE* popL(S_STACK* stack);

void addExp(S_STACK* stack, S_STACK* exps);

//PEEK
char* peek(S_STACK* stack);

bool isEmpty(S_STACK* stack);

bool isOp(char* c);

bool isParen(char* c);

S_STACK* postfix(S_STACK* stack);

//DISPLAY STAX
void dispStack(S_STACK* stack);
