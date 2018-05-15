#include "DAG.h"

T_TREE* createTree1() {
  T_TREE* tree = malloc(sizeof(T_TREE));
  tree->root = NULL;
  return tree;
}

D_NODE* createNode1() {
  D_NODE* ret = malloc(sizeof(D_NODE));
  return ret;
}


void pushT(T_STACK* stack, D_NODE* node){
    if (stack->top == NULL){
        stack->top = node;
        stack->size++;
        return;
    } else {
        node->next = stack->top;
        stack->top = node;
        stack->size++;
    }
}



D_NODE* popT(T_STACK* stack) {
      if (stack->top != NULL) {
        D_NODE *temp = stack->top;
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


D_NODE* createNode(T_TREE* tree, char* op, D_NODE* left, D_NODE* right) {
    D_NODE* ret = malloc(sizeof(D_NODE));
    ret->next = NULL;
    if ((left == NULL) && (right == NULL)) {
        ret->isLeaf = true;
        ret->op = op;
        ret->left = NULL;
        ret->right = NULL;
        tree->root = ret;
        return ret;
    } else {
        ret->isLeaf = false;
        ret->op = op;
        ret->left = left;
        ret->right = right;
        tree->root = ret;
        return ret;
    }
}

D_NODE* createTree(S_STACK* que, T_TREE* tree) {
    //que = postfix(que);
    T_STACK* stack = malloc(sizeof(T_STACK));
    if (que == NULL) {return NULL;} else if (que->size == 0) {return NULL;}
    S_NODE* curr = que->last;
    while (curr != NULL) {
      if (isParen(curr->data)) {curr = curr->prev;}
      else if (!(isOp(curr->data))) {
            D_NODE* addVal = createNode(tree, curr->data, NULL, NULL);
            pushT(stack, addVal);
            curr = curr->prev;
        } else {
            D_NODE* right = stack->top;
            popT(stack);
            D_NODE* left = stack->top;
            popT(stack);
            D_NODE* addVal = createNode(tree, curr->data, left, right);
            pushT(stack, addVal);
            curr = curr->prev;
        }
    }

    D_NODE* check = tree->root;
    while (tree->root->next != NULL) {
      D_NODE* le = tree->root;
      while (le->left != NULL) {le = le->left;}
      le->left = malloc(sizeof(D_NODE));
      le->left = tree->root->next;
      tree->root->next = tree->root->next->next;
    }

    return tree->root;
}


void dispTree(D_NODE* root) {
    if (root != NULL) {
        dispTree(root->left);
        printf("%s ", root->op);
        dispTree(root->right);
    }
}
