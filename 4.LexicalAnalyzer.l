letter [a-zA-Z]
digit [0-9]
id {letter}({letter}|{digit})*
int_num {digit}+
float_num {digit}+"."{digit}+
keyword int|float|double|if|for|while|case|switch
operator \+|\-|\*|\/|=|<|>
rel_op <=|>=|==|!=|<|>
delimiter ;|,|\(|\)|\{|\}|\[|\]

%%

{keyword} printf("%s is a Keyword\n", yytext);
{id} printf("%s is an Identifier\n", yytext);
{int_num} printf("%s is an Integer\n", yytext);
{float_num} printf("%s is a Float\n", yytext);
{operator} printf("%s is an Operator\n", yytext);
{rel_op} printf("%s is a Relational Operator\n", yytext);
{delimiter} printf("%s is a Delimiter\n", yytext);
[ \t\n] ;
. printf("%s is an Unknown token\n", yytext);

%%

int main() {
    extern FILE *yyin;
    yyin = fopen("input.txt", "r");
    yylex();
    fclose(yyin);
    return 0;
}

int yywrap() {
    return 1;
}