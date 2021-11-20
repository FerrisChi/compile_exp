%{

#include <stdio.h> 

#define KEYWORD		    1
#define ID              2
#define NUMBER          3
#define	OP_ASSIGN	    4
#define RELOP           5
#define OP_OTHER        6
#define STRING_LITERAL  7
#define C_CHAR          8
#define ERRORCHAR       9

int yylval;
int keyCnt = 0, idCnt = 0, numCnt = 0, strCnt = 0, charCnt = 0, opCnt = 0, errorCnt = 0;
 
%}

delim		[ \t \n]
ws			{delim}+
letter	    [A-Za-z_]
digit		[0-9]
id			{letter}({letter}|{digit})*
D			[0-9]
L			[a-zA-Z_]
H			[a-fA-F0-9]
E			([Ee][+-]?{D}*)
P           ([Pp][+-]?{D}+)
FS			(f|F|l|L)
IS          ((u|U)|(u|U)?(l|L|ll|LL)|(l|L|ll|LL)(u|U)) 
key         auto|break|case|char|const|continue|default|do|double|else|enum|extern|float|for|goto|if|inline|int|long|register|restrict|return|short|signed|sizeof|static|struct|switch|typedef|union|unsigned|void|volatile|while|_Alignas|_Alignof|_Atomic|_Bool|_Complex|_Generic|_Imaginary|_Noreturn|_Static_assert|_Thread_local

%%

"/*"			        { comt(); }
"//"[^\n]*              { /* consume //-comment */ }

{ws}	                {;/* Go ahead */}
{key}	                {yylval = KEYWORD; return (KEYWORD);}
{id}	                {yylval = installID (); return (ID);}

  /*
	L?'(\\.|[^\\'\n])+'	        { yylval = NUMBER; return(NUMBER); }
	This project consider char as character(s) rather than an unsigned integer and abandoned this rule.
  */
{D}+{E}{FS}?		        { yylval = NUMBER; return(NUMBER); }
0[xX]{H}+{IS}?		        { yylval = NUMBER; return(NUMBER); }
{D}*"."{D}+{E}?{FS}?	    { yylval = NUMBER; return(NUMBER); }
{D}+"."{D}*{E}?{FS}?	    { yylval = NUMBER; return(NUMBER); }
0[xX]{H}+{P}{FS}?	        { yylval = NUMBER; return(NUMBER); }
0[xX]{H}*"."{H}+{P}{FS}?    { yylval = NUMBER; return(NUMBER); }
0[xX]{H}+"."{H}*{P}{FS}?    { yylval = NUMBER; return(NUMBER); }
0[0-7]*{IS}?		        { yylval = NUMBER; return(NUMBER); }
[1-9]{D}*{IS}?		        { yylval = NUMBER; return(NUMBER); }

L?\"(\\.|[^\\"\n])*\"	    { yylval = STRING_LITERAL;return(STRING_LITERAL); }

  /*
	C_CHAR is added to ANSI C
  */
L?\'(\\.|[^\\"\n])*\'	    { yylval = C_CHAR; return(C_CHAR); }

"..."			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
">>="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"<<="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"+="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"-="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"*="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"/="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"%="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"&="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"^="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"|="			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
">>"			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"<<"			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"++"			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"--"			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"->"			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"&&"			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }
"||"			{ yylval = OP_ASSIGN; return(OP_ASSIGN); }

"<"	            { yylval = RELOP; return (RELOP);}
"<="	        { yylval = RELOP; return (RELOP);}
"=="	        { yylval = RELOP; return (RELOP);}
"!="	        { yylval = RELOP; return (RELOP);}
">"	            { yylval = RELOP; return (RELOP);}
">="	        { yylval = RELOP; return (RELOP);}

"#"             { yylval = OP_OTHER; return (OP_OTHER);}
";"			    { yylval = OP_OTHER; return (OP_OTHER);}
("{"|"<%")	    { yylval = OP_OTHER; return (OP_OTHER);}
("}"|"%>")	    { yylval = OP_OTHER; return (OP_OTHER);}
","			    { yylval = OP_OTHER; return (OP_OTHER);}
":"			    { yylval = OP_OTHER; return (OP_OTHER);}
"="			    { yylval = OP_OTHER; return (OP_OTHER);}
"("			    { yylval = OP_OTHER; return (OP_OTHER);}
")"			    { yylval = OP_OTHER; return (OP_OTHER);}
("["|"<:")	    { yylval = OP_OTHER; return (OP_OTHER);}
("]"|":>")	    { yylval = OP_OTHER; return (OP_OTHER);}
"."			    { yylval = OP_OTHER; return (OP_OTHER);}
"&"			    { yylval = OP_OTHER; return (OP_OTHER);}
"!"			    { yylval = OP_OTHER; return (OP_OTHER);}
"~"			    { yylval = OP_OTHER; return (OP_OTHER);}
"-"			    { yylval = OP_OTHER; return (OP_OTHER);}
"+"			    { yylval = OP_OTHER; return (OP_OTHER);}
"*"			    { yylval = OP_OTHER; return (OP_OTHER);}
"/"			    { yylval = OP_OTHER; return (OP_OTHER);}
"%"			    { yylval = OP_OTHER; return (OP_OTHER);}
"^"			    { yylval = OP_OTHER; return (OP_OTHER);}
"|"			    { yylval = OP_OTHER; return (OP_OTHER);}
"?"			    { yylval = OP_OTHER; return (OP_OTHER);}

.			    {yylval = ERRORCHAR; return ERRORCHAR;}
 
%%

int installID () {
	return ID;
}
 
 
int installNum () {
  return NUMBER;
}

int yywrap (){
  return 1;
}
 
void writeout(int c){
  switch(c){
  	case ERRORCHAR: errorCnt++; fprintf(yyout, "(ERRORCHAR, %s)\n", yytext);break;
    case OP_ASSIGN:
    case OP_OTHER: opCnt++; fprintf(yyout, "(OP %s)\n", yytext); break;
  	case RELOP: opCnt++; fprintf(yyout, "(RELOP %s)\n", yytext); break;
    case KEYWORD: keyCnt++; fprintf(yyout, "(KEYWORD, %s)\n", yytext); break;
    case NUMBER: numCnt++; fprintf(yyout, "(NUM %s)\n", yytext); break;
    case ID: idCnt++; fprintf(yyout, "(ID %s)\n", yytext); break;
    case STRING_LITERAL: strCnt++; fprintf(yyout, "(STRING_LITERAL %s)\n", yytext);break;
	case C_CHAR: charCnt++; fprintf(yyout, "(C_CHAR %s)\n", yytext); break;
    default:break;
  }
  return;
}

void comt()
{
	char c, prev = 0;
  
	while ((c = input()) != 0)      /* (EOF maps to 0) */
	{
		if (c == '/' && prev == '*')
			return;
		prev = c;
	}
	printf("unterminated comment\n");
}
 
int main (int argc, char ** argv){
	int c,j=0;
	if (argc>=2){
	  if ((yyin = fopen(argv[1], "r")) == NULL){
	    printf("Can't open file %s\n", argv[1]);
	    return 1;
	  }
	  if (argc>=3){
	    yyout=fopen(argv[2], "w");
	  }
	}

	while (c = yylex()){
		writeout(c);
	}
	if(argc>=2){
	  fclose(yyin);
	  if (argc>=3) fclose(yyout);
	}
    fprintf(yyout,
          "\nTotal:\n%d keywords\n%d "
          "identifiers\n%d "
          "constants\n%d operator\n%d string\n%d char\n",
          keyCnt, idCnt, numCnt, opCnt, strCnt, charCnt);
	return 0;
}