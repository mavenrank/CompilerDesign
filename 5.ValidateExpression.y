%{
    #include"yacc.tab.h"
    #include<stdio.h>
    int yylex(void);
    void yyerror(char *);
%}

%token IDE INUM

%%
S   :IDE'='E';''\n' {printf("Valid Syntax");}
E   :IDE OP IDE | IDE OP INUM | INUM OP IDE | INUM OP INUM | IDE
    | INUM
OP  : '+' | '-' | '*' | '/'

%%

int main(){
    printf("Enter the statement to validate");
    yyparse();
    return 0;
}
int yywrap(){
    return 1;
}
void yyerror(char *s){
    fprintf(stderr, "%s\n", s);
}