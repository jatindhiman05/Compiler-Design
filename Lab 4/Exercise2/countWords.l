%{
#include <stdio.h>
int word_count = 0;
%}

%%
[ \t\n]+  { word_count++; }
.         { }
%%

int main() {
    yylex();
    printf("Number of words: %d\n", word_count);
    return 0;
}

int yywrap() { return 1; }
