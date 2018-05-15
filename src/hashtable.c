#include "hashtable.h"

//FUNCTIONS FOR H_MAP

//CREATE
H_TABLE* createTable() {
    H_TABLE* ma = malloc(sizeof(H_TABLE));
    ma->size = 100;
    ma->inx = 0;
    ma->keys = malloc(sizeof(ma->arr));
    for (int i = 0; i < 100; i++) {
        ma->arr[i] = NULL;
    }
    mtable = ma;
    return ma;
}

//HASH1 FUNCTIONS using djb2 function

int HASH1(unsigned char* str, H_TABLE* table) {
    unsigned long hash = 5381;
    int c;
    while (c = *str++) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash % table->size;
}


//Create pair
H_NODE* cnew(char* key, TUPLE val) {
    H_NODE* retVal = NULL;
    retVal = (H_NODE*)malloc(sizeof(H_NODE));
    retVal->key = key;
    retVal->val = val;
    retVal->next = NULL;
    return retVal;
}


//Search
TUPLE getVal(H_TABLE* table, char* key) {
    H_NODE* fin;
    for (fin = table->arr[HASH1(key, table)]; fin != NULL; fin = fin->next) {
        if (fin->key == key) { return fin->val;}
    }
}


char* getTy(H_TABLE* table, char* key) {
  H_NODE* fin;
  int ha = HASH1(key, table);
  while (table->arr[ha] != NULL) {
    if (strcmp(table->arr[ha]->key, key) == 0) {
      return table->arr[ha]->val.type;
    }
    ha++;
  }
  return key;
}


bool searcher(H_TABLE* table, char* key) {
  H_NODE* fin;
  for (fin = table->arr[HASH1(key, table)]; fin != NULL; fin = fin->next) {
      if (fin->key == key) { return true;}
  }
  return false;
}

//Insert
void insert(H_TABLE* table, char* key, int scope, char* type, const char* annot, int LIN, int COL) {
    TUPLE val;
    val.scope = scope;
    val.type = type;
    val.annot = annot;
    val.LIN = LIN;
    val.COL = COL;

    /*
    if (searcher(table, key, type)) {
      return;
    }
    */

    H_NODE* ins = cnew(key, val);
    int hashv = HASH1(key, table);
    ins->next = table->arr[hashv];
    table->arr[hashv] = ins;
    table->keys[table->inx] = key;
    table->inx++;
}

//display table
void disp(H_TABLE* table) {
    printf("%s", "NAME            |   SCOPE   |   TYPE   |      ANNOTATION\n");
    for (int i = 0; i < table->inx; i++) {
        printf("%s",table->keys[i]);
        int k = 19 - strlen(table->keys[i]);
        for (int a = k; a > -1; a--) {
            printf(" ");
        }
        printf("%d ",getVal(table, table->keys[i]).scope);
        int q = 9;
        for (int a = q; a > -1; a--) {
            printf(" ");
        }
        printf("%s ",getVal(table, table->keys[i]).type);
        int b = 13 - strlen(getVal(table, table->keys[i]).type);
        for (int a = b; a > -1; a--) {
            printf(" ");
        }
        printf("%s ",getVal(table, table->keys[i]).annot);
        printf("\n\r");
    }
}

FILE *symFile=NULL;
void dispF(H_TABLE* table) {
		symFile = fopen("./compiler.st","w");
	fprintf(symFile,"%s", "NAME            |   SCOPE   |   TYPE   |      ANNOTATION\n");



	for(int i = 0 ; i < table->inx; i++){
	fprintf(symFile, "%s",  table->keys[i]); int k = 19 - strlen(table->keys[i]);
        for (int a = k; a > -1; a--) {
            fprintf(symFile ," ");
        }
	fprintf(symFile, "%d", getVal(table, table->keys[i]).scope);int q = 9;
        for (int a = q; a > -1; a--) {
            fprintf(symFile, " ");
        }
	fprintf(symFile, "%s", getVal(table, table->keys[i]).type); int b = 13 - strlen(getVal(table, table->keys[i]).type);
        for (int a = b; a > -1; a--) {
            fprintf(symFile, " ");
        }
	fprintf(symFile, "%s", getVal(table, table->keys[i]).annot);
	fprintf(symFile, "\n");


}


     fclose(symFile);
}
