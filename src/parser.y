  %{
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "scopelist.h"
#include "pairTable.c"
#include "genIR.c"
#define YYERROR_VERBOSE 1
#define STACK_MAX_SZ 1000

extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern int scope;
extern int linecounter;
extern int columncounter;

char** addvals;

int stk = 0;
int ing = 0;
int eps = 0;

H_TABLE* table;
S_STACK* stack[STACK_MAX_SIZE];
T_TREE* trees[STACK_MAX_SIZE];
D_NODE* roots[STACK_MAX_SIZE];
S_STACK* addexps;
bool inexp = false;

extern L_NODE* head;
pairNode* top = NULL;
extern int currLine;

void yyerror(const char* p);
%}


%union {
  int inx;
  char chr;
  char* str;
  float real;
  char* keyword;
  char* punc;
  char* operate;
  char* comment;
  char* cons;
  char* typi;
  //other
}
/* Bison declarations */
/* token declaration begins (non-terminals) */
%token <str> ID
%token <keyword> T_INTEGER
%token <keyword> T_REAL
%token <keyword> T_BOOLEAN
%token <keyword> T_CHARACTER
%token <keyword> T_STRING
%token <cons> C_INTEGER
%token <cons> C_REAL
%token <cons> C_CHARACTER
%token <cons> C_STRING
%token <cons> C_FALSE
%token <cons> C_TRUE
  // keywords
%token <keyword> NULL_PTR
%token <keyword> RESERVE
%token <keyword> RELEASE
%token <keyword> FOR
%token <keyword> WHILE
%token <keyword> IF
%token <keyword> THEN
%token <keyword> ELSE
%token <keyword> SWITCH
%token <keyword> CASE
%token <keyword> OTHERWISE
%token <keyword> TYPE
%token <keyword> FUNCTION
%token <keyword> CLOSURE
  // punctuation - grouping
%token <punc> L_PARENTHESIS
%token <punc> R_PARENTHESIS
%token <punc> L_BRACKET
%token <punc> R_BRACKET
%token <punc> L_BRACE
%token <punc> R_BRACE
%token <punc> S_QUOTE
%token <punc> D_QUOTE
  // punctuation - other
%token <punc> SEMI_COLON
%token <punc> COLON
%token <punc> COMMA
%token <punc> ARROW
%token <punc> BACKSLASH

%token END "end of file"
  // comments
%token <comment> COMMENT

%type <str> identifier_list assignable parameter_list non_empty_parameter_list
            parameter_declaration declaration declaration_list
            argument_list non_empty_argument_list
%type <keyword> statement definition memOP statement_list definition_list expression add_expression typ caser
%type <punc> sblock dblock pblock ablock
%type <operate> preUnaryOperator postUnaryOperator assignOP binaryOperator binaryOperatorBool recOP
%type <cons> constant id lparen rparen comma
%left <operate> LESS_THAN EQUAL_TO ASSIGN IS_NULL INT2REAL REAL2INT GREATER_THAN
%left <operate> ADD SUB_OR_NEG
%left <operate> MUL DIV REM
%right <operate> NOT AND OR DOT
%start program
%%

program: definition_list {push(stack[eps], "MAIN"); eps++;} sblock {push(stack[eps], "END_MAIN");}
        ;
definition_list:  definition definition_list {}
                | {}
                ;

definition: TYPE id COLON dblock { insert(table, $2, scope, $<str>1, "record", linecounter, columncounter);}
          | TYPE id COLON constant ARROW typ COLON lparen constant rparen {insert(table, $<str>2, scope, $6, "array", linecounter, columncounter);}
          | TYPE id COLON pblock ARROW typ { insert(table, $<str>2, scope, $<str>6, "User def Type", linecounter, columncounter);}
          | FUNCTION ID COLON ID  {push(stack[eps], "FUNCTION"); eps++; push(stack[eps], $<str>2); eps++;} sblock {push(stack[eps], "END_FUNC"); eps++; insert(table, $2, scope, $4, "function definiton", linecounter, columncounter);}
          ;
sblock: L_BRACE statement_list sblock R_BRACE {inexp = true;}
      | L_BRACE dblock statement_list R_BRACE {inexp = true;}
      | L_BRACE dblock statement_list sblock R_BRACE {inexp = true;}
      | L_BRACE statement_list R_BRACE {inexp = true;}
      | L_BRACE statement_list sblock statement_list R_BRACE {inexp = true;}
      | L_BRACE dblock statement_list sblock statement_list R_BRACE {inexp = true;}
      ;
dblock: L_BRACKET {inexp = false;} declaration_list {inexp = false;} R_BRACKET {inexp = true;}
      ;
declaration_list: declaration SEMI_COLON declaration_list {inexp = false;}
                | declaration {inexp = false;}
                ;
declaration: typ COLON identifier_list  { insert(table, $3, scope, $1, "declaration", linecounter, columncounter); if (ing > 0) {for (int i = 0; i < ing; i++) {insert(table, addvals[i], scope, $1, "declaration", linecounter, columncounter);}} ing = 0;}
           | id COLON identifier_list { insert(table, $3, scope, $1, "user type declaration", linecounter, columncounter); if (ing > 0) {for (int i = 0; i < ing; i++) {insert(table, addvals[i], scope, $1, "user declaration", linecounter, columncounter);}} ing = 0;}
           ;
identifier_list: id {}
               | id assignOP constant {inexp = true;}
               | id comma identifier_list { addvals[ing] = $3; ing++;}
               | id lparen assignOP constant rparen comma identifier_list {}
               | id lparen assignOP constant rparen {}
               ;
statement_list: statement statement_list {inexp = true;}
              | statement {inexp = true;}
              ;
statement: FOR { inexp = false; push(stack[eps], "FOR");eps++;} lparen {inexp = true;} statement SEMI_COLON expression {eps++;} SEMI_COLON statement {inexp = false;} rparen {inexp = true;} sblock {push(stack[eps], "END_FOR"); eps++;  inexp = true;}
         | WHILE {push(stack[eps], "WHILE");eps++;inexp = false;} lparen {inexp = true;} expression {inexp = false;} rparen { inexp = true; eps++;} sblock {push(stack[eps], "END_WHILE"); eps++; inexp = true;}
         | IF {push(stack[eps], "IF"); eps++; inexp = false;} lparen {inexp = true;} expression {inexp = false;} rparen {inexp = true; eps++;} THEN {push(stack[eps], "THEN");eps++;} sblock ELSE {push(stack[eps], "ELSE"); eps++;} sblock {push(stack[eps], "END_IF"); eps++; inexp = true;}
         | SWITCH {push(stack[eps], "SWITCH");eps++; inexp = false;} lparen {inexp = true;} expression {inexp = false;} rparen  {inexp = true; eps++;} caser OTHERWISE {push(stack[eps], "OTHERWISE");eps++;}COLON sblock {push(stack[eps], "END_SWITCH"); eps++; inexp = true;}
         | assignable assignOP expression SEMI_COLON { inexp = true; eps++;  insertPair(&top, $1, $3, linecounter);}
           //printf("%s and %s   |  %s == %s  |  line: %d\n", $1, $3, lookUp(&headT, $1), lookUp(&headT, $3), linecounter);}
         | memOP assignable SEMI_COLON {inexp = true; eps++; if (searcher(table, $2)) {TUPLE abc = getVal(table, $2); abc.annot = "array";}}
         ;
caser: CASE constant COLON sblock {inexp = true;}
     | CASE constant COLON sblock caser {inexp = true;}
     ;
expression: add_expression {inexp = true;}
          | binaryOperator add_expression {inexp = true;}
          | binaryOperatorBool add_expression {inexp = true;}
          | add_expression binaryOperator expression {inexp = true;}
          | add_expression binaryOperatorBool expression {inexp = true;}
          ;
add_expression: constant {inexp = true; $$ = $<str>1;}
              | preUnaryOperator add_expression {inexp = true;}
              | assignable {inexp = true;}
              | lparen add_expression postUnaryOperator rparen {inexp = true;}
              | lparen expression rparen {inexp = true;}
              ;
assignable: id {inexp = true; $$ = $<str>1;}
          | assignable ablock {inexp = true;}
          | assignable recOP id { char* currty = getTy(table, $1); if (strcmp(currty, "record") != 0) {printf("Not of record type: %s Line: %d", $1, linecounter);}}
          ;
pblock: lparen parameter_list rparen {}
      ;
parameter_list: non_empty_parameter_list {}
              | {$$ = NULL;}
              ;
non_empty_parameter_list: parameter_declaration comma non_empty_parameter_list {}
                        | parameter_declaration {}
                        ;
parameter_declaration: typ COLON id { inexp = true; insert(table, $<str>3, scope, $<str>1, "param declaration", linecounter, columncounter);}
                     ;
ablock: lparen argument_list rparen { inexp = true;}
      ;
argument_list: non_empty_argument_list {inexp = true;}
             | {}
             ;
non_empty_argument_list: expression comma non_empty_argument_list {inexp = true;;}
                       | expression {inexp = true;}
                       ;
preUnaryOperator: NOT {  if (inexp) {push(stack[eps], $<str>1);} $$ = $<str>1;}
                | INT2REAL {  if (inexp) {push(stack[eps], ""); push(stack[eps], $<str>1);} $$ = $<str>1;}
                | REAL2INT {  if (inexp) {push(stack[eps], ""); push(stack[eps], $<str>1);} $$ = $<str>1;}
                ;
postUnaryOperator: IS_NULL { if (inexp) {push(stack[eps], ""); push(stack[eps], $<str>1);} $$ = $<str>1;}
                 ;
memOP: RESERVE { if (inexp) {push(stack[eps], $<str>1);} $$ = $<str>1;}
     | RELEASE { if (inexp) {push(stack[eps], $<str>1);} $$ = $<str>1;}
     ;
assignOP: ASSIGN { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
        ;
recOP: DOT { if (inexp) {push(stack[eps], $<str>1);} $$ = $<str>1;}
     ;
binaryOperator: ADD { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
              | SUB_OR_NEG { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
              | MUL { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
              | DIV { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
              | REM { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
              | AND { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
              | OR { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
              ;
binaryOperatorBool: LESS_THAN { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
                  | EQUAL_TO { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
                  | GREATER_THAN { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
                  ;
constant: C_INTEGER { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
        | C_STRING { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
        | C_CHARACTER { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
        | C_REAL { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
        | C_TRUE { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
        | C_FALSE { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
        ;
typ: T_BOOLEAN {$$ = $1;}
    | T_INTEGER {$$ = $1;}
    | T_CHARACTER {$$ = $1;}
    | T_STRING {$$ = $1;}
    | T_REAL {$$ = $1;}
    ;
id: ID { if (inexp) {push(stack[eps], $<str>1);}  $$ = $<str>1;}
  ;
lparen: L_PARENTHESIS {if (inexp) {push(stack[eps], $<str>1);} $$ = $<str>1;}
      ;
rparen: R_PARENTHESIS {if (inexp) {push(stack[eps], $<str>1);} $$ = $<str>1;}
      ;
comma: COMMA {if (inexp) {push(stack[eps], $<str>1);} $$ = $<str>1;}
     ;
%%
/* Grammar rules ends */
#include <stdio.h>
#include "string.h"
extern int yylineno;
extern void yyset_in(FILE *);

FILE *inputFile;
int n;

int main(int argc, char *argv[]){
  addvals = malloc(sizeof(char*) * 20);
    if (argc == 2){
      FILE* afg = fopen(argv[1], "a");
      fprintf(afg, "%s\n", "a");
      fclose(afg);


      inputFile = fopen(argv[1], "r");
      table = createTable();
      for (int a = 0; a < 100; a++) {
        stack[a] = createStack();
        trees[a] = createTree1();
        roots[a] = NULL;
      }
      yyset_in(inputFile);
      yyparse();

        //typeCheck(&top, &headT);

      fclose(inputFile);
      FILE* af = fopen(argv[1], "a");
      size_t pos = ftell(af);
      fseek(af, 0, SEEK_END);
      size_t lenth = ftell(af);
      fseek(af, pos, SEEK_SET);
      truncate(argv[1], lenth - 2);
      fclose(af);
    } else if (argc==3){
        if (strcmp(argv[1],"-st")==0) {

          FILE* afg = fopen(argv[2], "a");
          fprintf(afg, "%s\n", "a");
          fclose(afg);


          //print symbol table
          inputFile = fopen(argv[2], "r");
          table = createTable();

          //trees = malloc(sizeof(T_TREE*) * 100);
          //roots = malloc(sizeof(D_NODE*) * 100);
          for (int a = 0; a <  STACK_MAX_SZ; a++) {
            stack[a] = createStack();
            //trees[a] = createTree1();
            //roots[a] = (D_NODE*)malloc(sizeof(D_NODE));
          }

          yyset_in(inputFile);
          yyparse();
          dispF(table);
          //dispType(&headT);
          //dispPairs(&top);
          //typeCheck(&top, &headT);


          FILE* af = fopen(argv[2], "a");
          size_t pos = ftell(af);
          fseek(af, 0, SEEK_END);
          size_t lenth = ftell(af);
          fseek(af, pos, SEEK_SET);
          truncate(argv[2], lenth - 2);
          fclose(af);

          //this line will contain the call to symbol table
          } else if (strcmp(argv[1],"-asc") == 0) {
            #define CHUNK 1024 /* read 1024 bytes at a time */
            char buf[1024];
           // int linenumber = 1;
            int scope = 0;
            int parse_value;
            FILE* afg = fopen(argv[2], "a");
            fprintf(afg, "%s", "a");
            fclose(afg);

            inputFile = fopen(argv[2], "r");

            table = createTable();
            for (int a = 0; a < 100; a++) {
              stack[a] = createStack();
              trees[a] = createTree1();
              roots[a] = NULL;
            }

            yyset_in(inputFile);
	          yyparse();
            //dispList(&head);

            FILE* af = fopen(argv[1], "a");
            size_t pos = ftell(af);
            fseek(af, 0, SEEK_END);
            size_t lenth = ftell(af);
            fseek(af, pos, SEEK_SET);
            truncate(argv[2], lenth - 2);
            fclose(af);

            inputFile = fopen(argv[2], "r");
	          FILE *prog = NULL;
	          prog = fopen("./compiler.asc","w");
            L_NODE** begin = &head;

		        while(!feof(inputFile)) {
              while((*begin)->next!=NULL) {
                fprintf(prog,"%d:%d:%d\t", (*begin)->lineno, (*begin)->scope,(*begin)->colno);
                (*begin)=(*begin)->next;
		            break;
              }
              fgets(buf, 1024, inputFile);
		          //fprintf(prog,"%d:%d:0\t", linenumber,scope);
		          //linenumber = linenumber+1;
		          fprintf(prog,"%s",buf);
            }
            printf("%s\n", "Created compiler.asc");
	          fclose(inputFile);
	          fclose(prog);
        } else if (strcmp(argv[1],"-ir") == 0) {
          FILE* afg = fopen(argv[2], "a");
          fprintf(afg, "%s\n", "a");
          fclose(afg);
          char buf[1024];

          inputFile = fopen(argv[2], "r");

          table = createTable();
          for (int a = 0; a < 100; a++) {
            stack[a] = createStack();
            trees[a] = createTree1();
            //roots[a] = createNode1();
          }

          yyset_in(inputFile);
          yyparse();


          inputFile = fopen(argv[2], "r");
          FILE *prog = NULL;
          prog = fopen("./compiler.ir","w");

          for (int i = 0; i <= eps; i++) {
            //dispStack(stack[i]);
            //dispStack(postfix(stack[i]));
            stack[i] = postfix(stack[i]);
            //dispStack(stack[i]);
            roots[i] = createTree(stack[i], trees[i]);
            //dispTree(roots[i]);
            char* curd = "";
            curd = genExp(roots[i]);
            fprintf(prog, "%s\n", curd);
          }

          fclose(inputFile);
          fclose(prog);

          //typeCheck(&top, &headT);

          FILE* af = fopen(argv[2], "a");
          size_t pos = ftell(af);
          fseek(af, 0, SEEK_END);
          size_t lenth = ftell(af);
          fseek(af, pos, SEEK_SET);
          truncate(argv[2], lenth - 2);
          fclose(af);


        } else if (strcmp(argv[1],"-opt") == 0) {
          FILE* afg = fopen(argv[2], "a");
          fprintf(afg, "%s\n", "a");
          fclose(afg);
          char buf[1024];

          inputFile = fopen(argv[2], "r");

          table = createTable();
          for (int a = 0; a < 100; a++) {
            stack[a] = createStack();
            trees[a] = createTree1();
            //roots[a] = createNode1();
          }

          yyset_in(inputFile);
          yyparse();


          inputFile = fopen(argv[2], "r");
          FILE *prog = NULL;
          prog = fopen("./compiler.opt","w");

          for (int i = 0; i <= eps; i++) {
            //dispStack(stack[i]);
            //dispStack(postfix(stack[i]));
            stack[i] = postfix(stack[i]);
            //dispStack(stack[i]);
            roots[i] = createTree(stack[i], trees[i]);
            //dispTree(roots[i]);
            char* curd = "";
            curd = genExp(roots[i]);
            fprintf(prog, "%s\n", curd);
          }

          fclose(inputFile);
          fclose(prog);

          //typeCheck(&top, &headT);

          FILE* af = fopen(argv[2], "a");
          size_t pos = ftell(af);
          fseek(af, 0, SEEK_END);
          size_t lenth = ftell(af);
          fseek(af, pos, SEEK_SET);
          truncate(argv[2], lenth - 2);
          fclose(af);


        }  else if (argv[1] == n) {

          FILE* afg = fopen(argv[2], "a");
          fprintf(afg, "%s\n", "a");
          fclose(afg);
          char buf[1024];

          inputFile = fopen(argv[2], "r");

          table = createTable();
          for (int a = 0; a < 100; a++) {
            stack[a] = createStack();
            trees[a] = createTree1();
            //roots[a] = createNode1();
          }

          yyset_in(inputFile);
          yyparse();


          inputFile = fopen(argv[2], "r");
          FILE *prog = NULL;
          prog = fopen("./compiler.ir","w");

          for (int i = 0; i <= eps; i++) {
            //dispStack(stack[i]);
            //dispStack(postfix(stack[i]));
            stack[i] = postfix(stack[i]);
            //dispStack(stack[i]);
            roots[i] = createTree(stack[i], trees[i]);
            //dispTree(roots[i]);
            char* curd = "";
            curd = genExp(roots[i]);
            fprintf(prog, "%s\n", curd);
          }

          fclose(inputFile);
          fclose(prog);

          //typeCheck(&top, &headT);

          FILE* af = fopen(argv[2], "a");
          size_t pos = ftell(af);
          fseek(af, 0, SEEK_END);
          size_t lenth = ftell(af);
          fseek(af, pos, SEEK_SET);
          truncate(argv[2], lenth - 2);
          fclose(af);


        } else {
            printf("Invalid invoking.\n");
            printf("Valid invoking:\n 1) compile -st prog \n 2) compile -asc prog \n");
        }
    } else{
        printf("Invalid invoking.\n");
        printf("Valid invoking:\n 1) compile -st prog \n 2) compile -asc prog \n");
    }
}


void yyerror(const char* p) {
  printf("%s Line: %d Col: %d\n ", p, linecounter, columncounter);

}
