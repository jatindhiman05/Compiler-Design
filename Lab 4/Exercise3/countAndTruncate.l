%{
#include <stdio.h>
int long_words = 0;
%}

%%
[a-zA-Z]{11,}   { long_words++; printf("%.10s...\n", yytext); }
.               { printf("%s", yytext); }
%%

int main() {
    yylex();
    printf("\nNumber of long words: %d\n", long_words);
    return 0;
}

int yywrap() { return 1; }
