%{
    #include <stdio.h>
    #include "simplecalc.tab.h"
    extern int yylval;
%}

L   [a-zA-Z]
D   [0-9]
ID  {L}|({L}|{D})*
NUM {D}+
AO  [-+*/]
PO  [()]
ASO =
SO  [;\n]

%%
{NUM}   {yylval=atoi(yytext); return (NUM);}
{AO}    return *yytext;
{PO}    return *yytext;
{ASO}   return *yytext;
{SO}    return *yytext;
%%