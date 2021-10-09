%{ 
    int yylineno;
%}

%%

^(.*)\n printf("%4d\t%s",  yylineno++, yytext);

%%


int main(int argc, char *argv[])
{ 
    FILE *fp = NULL;
    if (argc == 2) {
      fp = fopen(argv[1], "r");
      if (NULL != fp) {
         yyin = fp;  
      } 
    }  
     
    yylex();
    
    if (NULL != fp) { 
       fclose(fp); 
    }
    
    return 0;
}

int yywrap()
{
  return 1;
}