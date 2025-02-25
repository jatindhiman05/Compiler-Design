%{
#include <stdio.h>
FILE *fp;
%}

%%
[a-zA-Z_][a-zA-Z0-9_]*\[[0-9]*\]  {
    fp = fopen("arrays.txt", "a");
    fprintf(fp, "Array declaration: %s\n", yytext);
    fclose(fp);
}
.   { }
%%

int main() {
    yylex();
    return 0;
}

int yywrap() { return 1; }
