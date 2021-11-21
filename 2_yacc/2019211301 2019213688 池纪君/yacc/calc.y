%{
#include <stdio.h>
#include <string.h>
int yylex(void);
void yyerror(char *);
%}

%union{
    int inum;
    double dnum;
    char ch;
}

%token ADD SUB MUL DIV CR LEFT RIGHT
%token <dnum> NUM

%type <dnum> expression term frac

%start linelist

%%
linelist: line
    | linelist line

line : expression CR  {printf("Ans = %lf\n\n",$1);}

expression: term
    | expression ADD term {printf("Reduce by exp -> exp + term\n"); $$=$1+$3;}
    | expression SUB term {printf("Reduce by exp -> exp - term\n"); $$=$1-$3;}
    ;

term: frac
    | term MUL frac {printf("Reduce by term -> term * num\n"); $$=$1*$3;}
    | term DIV frac {printf("Reduce by term -> term / num\n"); $$=$1/$3;}
    ;

frac: LEFT expression RIGHT {printf("Reduce by frac -> (expression)\n"); $$=$2;}
    | NUM {printf("Reduce by frac -> num\n"); $$=$1;}

%%

void yyerror(char *str){
    fprintf(stderr,"error:%s\n",str);
}

int yywrap(){
    
    return 1;
}
int main()
{
    yyparse();
}