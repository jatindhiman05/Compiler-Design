%{
#include <stdio.h>
%}

%%
[A-Z]   { printf("%c", yytext[0] + 32); }
.       { printf("%s", yytext); }
%%

int main() {
    yylex();
    return 0;
}

int yywrap() { return 1; }