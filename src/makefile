#clean up, then remake the lexer and run
compiler:
	make clean
	bison -dv parser.y
	flex lexer.lex
	gcc -o compiler parser.tab.c lex.yy.c

debug:
	make clean
	bison -dv parser.y
	flex lexer.lex
	gcc parser.tab.c lex.yy.c -g
	ddd ./a.out

debug1:
	make clean
	bison -dv parser.y
	flex lexer.lex
	gcc parser.tab.c lex.yy.c -g
	gdb ./a.out

clean:
	rm -f *.st
	rm -f *.ir
	rm -f *.asc
	rm -f *.o
	rm -f a.out
	rm -f lexer
	rm -f lex.yy.c
	rm -f output.txt
	rm -f *.tab.c
	rm -f *.output
	rm -f compiler
	rm -f parser.tab.h
	rm -f try
