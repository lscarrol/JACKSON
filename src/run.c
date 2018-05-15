#include <stdio.h>
#include "hashtable.c"
extern int yylineno;
#include "string.h"
H_TABLE* table = createTable(100);
//give me a second i am her this is amrit ok
//// extern char* yytext;
// extern int yyleng;
// extern int linecounter;
// extern int columncounter;
// extern int hold;


extern "C" int yyparse();
extern void yyset_in(FILE *);


FILE *inputFile;

int main(int argc, char *argv[]){

    if (argc == 2){
        inputFile = fopen(argv[1], "r");
        yyset_in(inputFile);
        printf("%s %d\n", argv[1], argc);
        //only pirnts the errors

        yyparse();
    } else if (argc==3){

        if (strcmp(argv[1],"-st")==0){
            //print symbol table
            inputFile = fopen(argv[2], "r");
            yyset_in(inputFile);
            disp(table);
            //this line will contain the call to symbol table
        } else if (strcmp(argv[1],"-asc")==0){
            inputFile = fopen(argv[2], "r");
            yyset_in(inputFile);
            /* print annotated source code.
            * storing the output in a *.asc file
            */
            yyparse();
        } else{
            printf("Invalid invoking.\n");
            printf("Valid invoking:\n 1) compile -st prog \n 2) compile asc prog \n");
        }
    }else{
        printf("Invalid invoking.\n");
        printf("Valid invoking:\n 1) compile -st prog \n 2) compile asc prog \n");
    }
}



/*  Part 1 run file
int main(void){
  int tok;
  tok = yylex();
  int lineno = 1;
  int colno = 1;
  while(tok) {
	if (tok == 700)printf("%3d %s %d %d\n", tok, yytext, hold, columncounter);
 	else printf("%3d %s %d %d\n", tok, yytext, linecounter, columncounter);
	columncounter = columncounter + yyleng;
	//if(tok == 700){
	//	for (size_t i = 0; i < yyleng; i++){
	//		if (yytext[i] == '\n') {linecounter = linecounter -1;}
        //  }linecounter = linecounter -1;
	//}
	tok = yylex();
	linecounter = yylineno;
  }
return 0;
}
*/
