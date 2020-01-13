%option noyywrap
%{

#include "calculator.tab.hpp"

int yylex();
%}

%%
^#(.|\\\n)*\n   ;
\\\n            ;
[[:blank:]]+ 	;
[0-9]+          { yylval = atoi(yytext); return NUM; }
"+"             { return PLUS;}
"-"             { return MINUS;}
"*"             { return MULTIPLY;}
"/"             { return DIVIDE;}
"%"             { return MOD;}
"^"             { return POW;}
\(              { return LBRACKET;}
\)              { return RBRACKET;}
\n              { return END;}
EOF             { return 0;}
.               { return ERROR;}
%%