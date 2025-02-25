%{
#include <stdio.h>
int line_number = 1;
%}

%%
^(.*)  { printf("%d %s\n", line_number++, yytext); }
%%

int main() {
    yylex();
    return 0;
}

int yywrap() { return 1; }
