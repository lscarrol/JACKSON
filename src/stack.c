#include "stack.h"


char* operators[18] = {"reserve", "release", ",", ":=", "i2r", "r2i", "&", "|", "<", "=", "isNull", "+", "-", "/", "*", "%", "!", "."};
int precedences[18] = {7, 7, 6, 1, 1, 1, 2, 2, 3, 3, 3, 4, 4, 5, 5, 5, 5, 6};



S_STACK* createStack() {
  S_STACK* ret = (S_STACK*)malloc(sizeof(S_STACK));
  ret->size = 0;
  ret->top = NULL;
  return ret;
}

bool isEmpty(S_STACK* stack) {
  return stack->size == 0;
}

char* getData(S_NODE* node) {return node->data;}

void push(S_STACK* stack, char* data){
  S_NODE* add = (S_NODE*) malloc(sizeof(S_NODE));
  add->data = data;
  if (stack->top == NULL){
      stack->top = add;
      stack->last = add;
      stack->size++;
      return;
  } else {
      stack->top->prev = add;
      add->next = stack->top;
      stack->top = add;
      stack->size++;
  }
}


S_NODE* pop(S_STACK* stack) {
    if (!isEmpty(stack)) {
        S_NODE *temp = stack->top;
        if (stack->top->next == NULL) {
            stack->top = NULL;
            stack->size--;
            return NULL;
        } else {
            stack->top = stack->top->next;
            stack->size--;
            return temp;
        }
    } else {
        return NULL;
    }
}

S_NODE* popL(S_STACK* stack) {
    if (!isEmpty(stack)) {
        S_NODE *temp = stack->last;
        if (stack->last->prev == NULL) {
            stack->last = NULL;
            stack->size--;
            return NULL;
        } else {
            stack->last = stack->last->prev;
            stack->size--;
            return temp;
        }
    } else {
        return NULL;
    }
}

void addExp(S_STACK* stack, S_STACK* exps) {
  while (exps->top != NULL) {
    push(stack, peek(exps));
    pop(exps);
  }
}


char* peek(S_STACK* stack) {
  if (isEmpty(stack)) {
      return NULL;
  } else {return stack->top->data;}
}



bool isOp(char* c) {
    for (int i = 0; i < 15; i++) {
        if (strcmp(c, operators[i]) == 0) {return true;}
    }
    return false;
}

bool isParen(char* c) {
  if (strcmp(c, "(") == 0) {return true;}
  else if (strcmp(c, ")") == 0) {return true;}
  return false;
}


int precedence(char* op) {
    for (int i = 0; i < 15; i++) {
        if (strcmp(operators[i], op) == 0) {
            return precedences[i];
        }
    }
}


S_STACK* postfix(S_STACK* stack) {
    S_STACK* out = createStack();
    S_STACK* ops = createStack();
    if (stack == NULL) {
      return NULL;
    }
    if (stack->top == NULL) {
      return NULL;
    }
    S_NODE* curr = stack->last;
    int prec1 = precedence(curr->data);
    int prec2;
    while (curr != NULL) {
      if ((!isOp(curr->data)) && (!isParen(curr->data))) {
        push(out, curr->data);
        curr = curr->prev;
      } else {
          if (ops->size == 0) {
          push(ops, curr->data);
          curr = curr->prev;
        } else if (strcmp(curr->data, "(") == 0) {
         push(ops, curr->data);
         curr = curr->prev;
       } else if (strcmp(curr->data, ")") == 0) {
         while (strcmp(ops->top->data, "(") != 0) {
           push(out, ops->top->data);
           pop(ops);
           if (ops->size == 0) {break;}
         }
         if (ops->size != 0) {pop(ops);}
         curr = curr->prev;
       } else {
          prec2 = precedence(ops->top->data);
          if (prec1 > prec2) {
            push(ops, curr->data);
            curr = curr->prev;
          } else {
            while ((ops->size != 0) && !(prec1 <= prec2)) {
              push(out, ops->top->data);
              pop(ops);
              if (ops->size != 0) {prec2 = precedence(ops->top->data);} else {break;}
            }
            push(ops, curr->data);
            curr = curr->prev;
          }
        }
      }
    }
    while (ops->top != NULL) {
      push(out, ops->top->data);
      pop(ops);
    }
    return out;

}

void dispStack(S_STACK* stack){
  if (stack == NULL) {
    return;
  }
  if (stack->last == NULL) {
    return;
  } else {
    S_NODE* curr = stack->last;
    while (curr != NULL) {
      printf("%s\n", curr->data);
      curr = curr->prev;
    }
  }
  printf("%s\n", "------------");
}
