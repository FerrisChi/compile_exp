
calc.exe: y.tab.o lex.yy.o 
	gcc -o calc y.tab.o lex.yy.o
	rm $(dir)*.o

lex.yy.o: lex.yy.c y.tab.h
	gcc -c lex.yy.c

y.tab.o: y.tab.c
	gcc -c y.tab.c

lex.yy.c: calc.l
	flex calc.l

y.tab.c: calc.y
	bison -v --yacc -d  calc.y

y.tab.h: calc.y
	echo"y.tab.h was created at the same time as y.tab.c." 

clean:
	rm -f calc lex.yy.c y.tab.c y.tab.h
