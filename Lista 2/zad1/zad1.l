%{
#include<stdio.h>
int yylex();
int yywrap();
int words;
int lines;
%}

%%

^[[:blank:]]*\n*      ;
[[:blank:]]+    ;
[[:blank:]]+$   ;
\n              { fprintf(yyout,"\n"); lines++; }
[[:blank:]]*    { fprintf(yyout," "); }
[a-zA-Z]+       { words++; ECHO; }


%%

int yywrap() {
    return 1;
}

int main(int argc, char* argv[]) {
    yyin = fopen(argv[1], "r");
    yyout = fopen("wynik.txt", "w");
    yylex();
    printf("\n words =  %d \n lines = %d\n", words, lines);

    return 0;
}