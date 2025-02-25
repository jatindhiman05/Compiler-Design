%{
#include <stdio.h>
%}

%%
\/\/.*       { /* Ignore single-line comments */ }
\/\*([^*]|\*+[^/*])*\*+\/  { /* Ignore multi-line comments */ }
.           { printf("%s", yytext); }
%%

int main() {
    yylex();
    return 0;
}

int yywrap() { return 1; }
