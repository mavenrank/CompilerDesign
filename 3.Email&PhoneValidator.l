EMAIL [a-zA-Z0-9\._-]+@vit(student)?\.ac.in
PHONE [6-9][0-9]{9}
REGNO (1[6-9]|2[0-4])(BCE|BKT|BIT|BBS|BDS|BCT|BCI|BEE|BME|BEC)[0-9]{4}
%%
    {EMAIL} printf("It is a Valid Email Address");
    {PHONE} printf("It is a valid Phone Number");
    {REGNO} printf("It is a valid Register Number");
    . printf("It is not valid");
%%
int main(){
    yylex();
    return 0;
}
int yywrap(){
    return 1;
}