
%{
#include<stdio.h>
#include<stdbool.h>

int yylex();
int yywrap();

bool preserve_docs = false;
%}

%x MULTILINE_COMMENT
%x INCLUDE

singleline ^[[:blank:]]*\/\/.*\n
open_multiline \/\*(\n)?

doc_singleline ^[[:blank:]]*\/\/(\/|!).*\n 
open_doc_multiline \/\*(\*|!)(\n)?
close_multiline \*\/(\n)?


%%

.*\\/n?.*                  ;
{doc_singleline}         { if (preserve_docs) ECHO; }
{singleline}             ;
{open_doc_multiline}     { if (preserve_docs) ECHO; else BEGIN(MULTILINE_COMMENT); }
{open_multiline}         { BEGIN(MULTILINE_COMMENT); }

<MULTILINE_COMMENT>{
.|\n                     ;
{close_multiline}        { BEGIN(0); }
}


%%

int yywrap() {
    return 1;
}

int main(int argc, char**argv) {
    if ( argv[1][0] == '-' && argv[1][1] == 'd'){
        preserve_docs = true;
        yyin = fopen(argv[2], "r");
        yyout = fopen("wyniki-doc.cpp", "w");
    } else {
        yyin = fopen(argv[1], "r");
        yyout = fopen("wyniki.cpp", "w");
    }
    yylex();
    return 0;
}

// backslash nowa linia powinien byc zignorwoany nawet jezeli jest w srodku tokenu