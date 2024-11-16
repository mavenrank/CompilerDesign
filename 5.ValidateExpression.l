%{
    #include "program6.yacc.tab.h"
%}
L   [a-zA-Z]
D   [0-9]
KW1 if
KW2 while
KW3 for
ID  {L}({L}|{D})*
NUM {D}+
ROP "<"|">"|"<="|">="
COP [{}]
SOP [;\n]
PO  [()]
AOP [-+*/=]
IDOP "++"|"--"
%%
    {KW1} return(IF);
    {KW2} return(WHILE);
    {KW3} return(FOR);
    {ID} return(IDEN);
    {NUM} return(NUM);
    {ROP} return *yytext;
    {COP} return *yytext;
    {SOP} return *yytext;
    {PO} return *yytext;
    {IDOP} return *yytext;
    {AOP} return *yytext;
    . {yyerror("Invalid");}
%%