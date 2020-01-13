%option yylineno
%{
#include <iostream>
#include "parser.tab.hpp"

%}

%option noyywrap

%%

"DECLARE"		return DECLARE;
"BEGIN"		    return _BEGIN;
"END"		    return END;

"ASSIGN"		return ASSIGN;

"IF"		    return IF;
"THEN"		    return THEN;
"ELSE"		    return ELSE;
"ENDIF"		    return ENDIF;

"WHILE"		    return WHILE;
"DO"		    return DO;
"ENDWHILE"	    return ENDWHILE;
"ENDDO"	        return ENDDO;

"FOR"		    return FOR;
"FROM"		    return FROM;
"TO"		    return TO;
"DOWNTO"	    return DOWNTO;
"ENDFOR"	    return ENDFOR;

"WRITE"		    return WRITE;
"READ"		    return READ;

"EQ"            return EQ;
"LE"            return LE;
"GE"            return GE;
"LEQ"           return LEQ;
"GEQ"           return GEQ;
"NEQ"           return NEQ;

"PLUS"          return ADD;
"MINUS"         return SUB;
"TIMES"         return MUL;
"DIV"           return DIV;
"MOD"           return MOD;
"("             return LB;
")"             return RB;

"-"             return MINUS;

[0-9]+          { yylval.str = (char *)strdup(yytext); return NUMBER; }
[_a-z]+         { yylval.str = (char *)strdup(yytext); return PIDENTIFER; }

","             { return COMA;}
";"             { return SEM;}
":"             { return COL;}

[ \t\r\n]+

%%