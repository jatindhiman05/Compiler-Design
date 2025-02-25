%{
#include <stdio.h>
int line_count = 0, a_word_count = 0;
%}

%%
\n       { line_count++; }
[aA][a-zA-Z]* { a_word_count++; }
.        { }
%%

int main() {
    yylex();
    printf("Lines: %d\nWords starting with 'a': %d\n", line_count, a_word_count);
    return 0;
}

int yywrap() { return 1; }
