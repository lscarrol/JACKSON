//
// Created by oswald on 3/6/18.
//

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>


struct H_TABLE* mtable;

//def tuple
typedef struct TUPLE {
    int scope;
    char* type;
    const char* annot;
    int COL;
    int LIN;
} TUPLE;

//def node for hashtable
typedef struct H_NODE {
    char* key;
    TUPLE val;
    struct H_NODE* next;
} H_NODE;

//def the map
typedef struct H_TABLE {
    int size;
    H_NODE* arr[1000];
    int inx;
    char** keys;
} H_TABLE;


//FUNCTIONS FOR H_MAP

//CREATE
H_TABLE* createTable();

//HASH FUNCTIONS using djb2 function
int HASH1(unsigned char* str, H_TABLE* table);

//Create pair
H_NODE* cnew(char* key, TUPLE val);

//Insert
void insert(H_TABLE* table, char* key, int scope, char* type, const char* annot, int LIN, int COL);

//Search
TUPLE getVal(H_TABLE* table, char* key);

char* getTy(H_TABLE* table, char* key);
//find
bool searcher(H_TABLE* table, char* key);

//display table
void disp(H_TABLE* table);

void dispF(H_TABLE* table);
