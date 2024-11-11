#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_TOKEN_SIZE 100
#define MAX_SYMBOLS 100
typedef enum
{
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DATATYPE,
    KEYWORD,
    UNKNOWN
} TokenType;
typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_SIZE];
} Token;
typedef struct
{
    char symbol[MAX_TOKEN_SIZE];
    char type[MAX_TOKEN_SIZE];
    int size;
    int location;
} Symbol;
Symbol symbolTable[MAX_SYMBOLS];
int symbolCount = 0;
int currentLocation = 0;
void addSymbol(char *
                   symbol,
               char *
                   type,
               int
                   size)
{
    strcpy(symbolTable[symbolCount].symbol,
           symbol);
    strcpy(symbolTable[symbolCount].type,
           type);
    symbolTable[symbolCount].size =
        size;
    symbolTable[symbolCount].location = currentLocation;
    currentLocation +=
        size;
    symbolCount++;
}
void printSymbolTable()
{
    printf("Location\tSymbol\tType\tSize\n");
    for (int i = 0; i < symbolCount; i++)
    {
        printf("%d\t\t%s\t%s\t%d\n", symbolTable[i].location,
               symbolTable[i].symbol, symbolTable[i].type, symbolTable[i].size);
    }
}
int isOperator(char
                   ch)
{
    return ch == '+' ||
           ch == '-' ||
           ch == '*' ||
           ch == '/' ||
           ch == ',' ||
           ch == ';' ||
           ch == '=' ||
           ch == '<' ||
           ch == '>' ||
           ch == '(' ||
           ch == ')' ||
           ch == '{' ||
           ch == '}';
}
int isSpecialOperator(const char *
                          str)
{
    return strcmp(
               str, "==") == 0 ||
           strcmp(
               str, "!=") == 0 ||
           strcmp(
               str, "<=") == 0 ||
           strcmp(
               str, ">=") == 0 ||
           strcmp(
               str, "++") == 0 ||
           strcmp(
               str, "--") == 0;
}
int isDatatype(const char *
                   str)
{
    return strcmp(
               str, "int") == 0 ||
           strcmp(
               str, "float") == 0 ||
           strcmp(
               str, "char") == 0 ||
           strcmp(
               str, "double") == 0 ||
           strcmp(
               str, "void") == 0;
}
int isKeyword(const char *
                  str)
{
    return strcmp(
               str, "for") == 0 ||
           strcmp(
               str, "while") == 0 ||
           strcmp(
               str, "if") == 0 ||
           strcmp(
               str, "else") == 0 ||
           strcmp(
               str, "return") == 0 ||
           strcmp(
               str, "break") == 0 ||
           strcmp(
               str, "continue") == 0;
}
int getSizeOfType(const char *
                      type)
{
    if (strcmp(
            type, "int") == 0)
        return 4;
    if (strcmp(
            type, "float") == 0)
        return 4;
    if (strcmp(
            type, "char") == 0)
        return 1;
    if (strcmp(
            type, "double") == 0)
        return 8;
    return 0;
}
void lexAnalysis(const char *
                     input)
{
    int i = 0;
    int length = strlen(
        input);
    char currentType[MAX_TOKEN_SIZE] = "";
    while (i < length)
    {
        Token token;
        int j = 0;
        if (isspace(
                input[i]))
        {
            i++;
            continue;
        }
        if (isalpha(
                input[i]))
        {
            while (i < length && (isalpha(
                                      input[i]) ||
                                  isdigit(
                                      input[i])))
            {
                token.value[j++] =
                    input[i++];
            }
            token.value[j] = '\0';
            if (isDatatype(token.value))
            {
                token.type = DATATYPE;
                strcpy(currentType, token.value);
            }
            else if (isKeyword(token.value))
            {
                token.type = KEYWORD;
            }
            else
            {
                token.type = IDENTIFIER;
                if (isdigit(token.value[0]))
                {
                    printf("Error: '%s' is not a valid identifier\n",
                           token.value);
                }
                else if (currentType[0] != '\0')
                {
                    addSymbol(token.value, currentType,
                              getSizeOfType(currentType));
                }
                else
                {
                    printf("Error: '%s' is not a valid identifier\n",
                           token.value);
                }
            }
        }
        else if (isdigit(
                     input[i]))
        {
            while (i < length && (isdigit(
                                      input[i]) ||
                                  isalpha(
                                      input[i])))
            {
                token.value[j++] =
                    input[i++];
            }
            token.value[j] = '\0';
            token.type = NUMBER;
            printf("Error: '%s' is not a valid identifier\n", token.value);
        }
        else if (isOperator(
                     input[i]))
        {
            token.value[j++] =
                input[i++];
            if (i < length && isOperator(
                                  input[i]))
            {
                token.value[j++] =
                    input[i++];
                token.value[j] = '\0';
                if (isSpecialOperator(token.value))
                {
                    token.type = OPERATOR;
                }
                else
                {
                    token.value[--j] = '\0';
                    i--;
                    token.type = OPERATOR;
                }
            }
            else
            {
                token.value[j] = '\0';
                token.type = OPERATOR;
            }
        }
        else
        {
            token.value[j++] =
                input[i++];
            token.value[j] = '\0';
            token.type = UNKNOWN;
        }
    }
}
int main()
{
    char input[MAX_TOKEN_SIZE];
    printf("Enter the input string: ");
    fgets(input, MAX_TOKEN_SIZE, stdin);
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
    }
    lexAnalysis(input);
    printSymbolTable();
    return 0;