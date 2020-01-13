%{
#include<stdio.h>
int yylex();
int yywrap();
%}

%x TAG
%x COM
%x STR

o_com \<!--
c_com --\>

%%

\<                  {ECHO; BEGIN(TAG);}
<TAG>{
    \".*\"	        ECHO;
    .               ECHO;
    \n              ECHO;
    .>             {ECHO; BEGIN(0);}
}


{o_com}      BEGIN(COM);

<COM>{
.|\n                ;
{c_com}     BEGIN(0);
}


\<!--.*--.*--\> {ECHO;}

%%

int yywrap() {
    return 1;
}

int main(int argc, char* argv[]) {
    yyin = fopen(argv[1], "r");
    yyout = fopen("wynik.xml", "w");
    yylex();
    return 0;
}