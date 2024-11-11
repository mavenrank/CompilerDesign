#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_TOKEN_SIZE 100
typedef enum
{
    IDENTIFIER,
    NUMBER,
    OPERATOR,
    DATATYPE,
    UNKNOWN,
    KEYWORD
} TokenType;
typedef struct
{
    TokenType type;
    char value[MAX_TOKEN_SIZE];
} Token;
void printToken(Token
                    token)
{
    switch (
        token.type)
    {
    case IDENTIFIER:
        printf("%s is identifier\n",
               token.value);
        break;
    case NUMBER:
        printf("%s is number\n",
               token.value);
        break;
    case OPERATOR:
        printf("%s is operator\n",
               token.value);
        break;
    case DATATYPE:
        printf("%s is datatype\n",
               token.value);
        break;
    case KEYWORD:
        printf("%s is keyword\n",
               token.value);
        break;
    default:
        printf("%s is unknown\n",
               token.value);
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
    char *keywords[] = {"int", "float", "char", "double", "void", "if",
                        "else", "for", "while", "return"};
    int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    for (int i = 0; i < numKeywords; i++)
    {
        if (strcmp(
                str, keywords[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}
void lexAnalysis(const char *
                     input)
{
    int i = 0;
    int length = strlen(
        input);
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
            if (isKeyword(token.value))
            {
                token.type = KEYWORD;
            }
            else if (isDatatype(token.value))
            {
                token.type = DATATYPE;
            }
            else
            {
                token.type = IDENTIFIER;
            }
        }
        else if (isdigit(
                     input[i]))
        {
            while (i < length && isdigit(
                                     input[i]))
            {
                token.value[j++] =
                    input[i++];
            }
            token.value[j] = '\0';
            token.type = NUMBER;
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
        printToken(token);
    }
}
int main()
{
    char input[MAX_TOKEN_SIZE];
    printf("Enter input: ");
    fgets(input, MAX_TOKEN_SIZE, stdin);
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n')
    {
        input[len - 1] = '\0';
    }
    lexAnalysis(input);
    return 0;
}