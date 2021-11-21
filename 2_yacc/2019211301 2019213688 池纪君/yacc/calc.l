%{
#include <stdio.h>
#include "y.tab.h"
void yyerror(char *);
void getNum();
%}

O   [0-7]
D   [0-9]
NZ  [1-9]
H   [a-fA-F0-9]
HP  (0[xX])
E   ([Ee][+-]?{D}+)
P   ([Pp][+-]?{D}+)
FS  (f|F|l|L)
IS  (((u|U)(l|L|ll|LL)?)|((l|L|ll|LL)(u|U)?))
CP  (u|U|L)
ES  (\\(['"\?\\abfnrtv]|[0-7]{1,3}|x[a-fA-F0-9]+))


%%

{HP}{H}+{IS}?				{ getNum(); return NUM; }
{NZ}{D}*{IS}?				{ getNum(); return NUM; }
"0"{O}*{IS}?				{ getNum(); return NUM; }
{CP}?"'"([^'\\\n]|{ES})+"'"		{ getNum(); return NUM; }

{D}+{E}{FS}?				{ getNum(); return NUM; }
{D}*"."{D}+{E}?{FS}?			{ getNum(); return NUM; }
{D}+"."{E}?{FS}?			{ getNum();  return NUM; }
{HP}{H}+{P}{FS}?			{ getNum(); return NUM; }
{HP}{H}*"."{H}+{P}{FS}?			{ getNum(); return NUM; }
{HP}{H}+"."{P}{FS}?			{ getNum(); return NUM; }

"+"							return ADD;
"-"							return SUB;
"*"							return MUL;
"/"							return DIV;
"("                         return LEFT;
")"                         return RIGHT;
\n                          return CR;
[ \t]+                     /* ignore whitespace */;

%%

void getNum() {
    sscanf(yytext,"%lf",&yylval.dnum);
}