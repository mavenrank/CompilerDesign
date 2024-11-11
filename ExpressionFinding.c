#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR 100
#define MAX_TOKEN_LEN 100

typedef enum {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_OPERATOR,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON
} TokenType;

typedef struct {
    TokenType type;
    char text[MAX_TOKEN_LEN];
} Token;

char *input;
int pos = 0;
Token currentToken;

void getNextToken();

void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

int isOperatorChar(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

void skipWhitespace() {
    while (isspace(input[pos])) {
        pos++;
    }
}

void getNextToken() {
    skipWhitespace();

    if (input[pos] == '\0') {
        currentToken.type = TOKEN_EOF;
        return;
    }

    if (isalpha(input[pos])) {
        int start = pos;
        while (isalnum(input[pos])) {
            pos++;
        }
        int len = pos - start;
        strncpy(currentToken.text, &input[start], len);
        currentToken.text[len] = '\0';
        currentToken.type = TOKEN_IDENTIFIER;
    } else if (isOperatorChar(input[pos])) {
        currentToken.type = TOKEN_OPERATOR;
        currentToken.text[0] = input[pos];
        currentToken.text[1] = '\0';
        pos++;
    } else if (input[pos] == '=') {
        currentToken.type = TOKEN_ASSIGN;
        currentToken.text[0] = '=';
        currentToken.text[1] = '\0';
        pos++;
    } else if (input[pos] == ';') {
        currentToken.type = TOKEN_SEMICOLON;
        currentToken.text[0] = ';';
        currentToken.text[1] = '\0';
        pos++;
    } else {
        error("Unknown character encountered");
    }
}

typedef struct {
    char expression[MAX_TOKEN_LEN * 3]; // operand + operator + operand
    int exprNumber;
} Expression;

Expression expressions[MAX_EXPR];
int exprCount = 0;

void collectExpressions() {
    int tokenIndex = 0;
    Token tokens[MAX_EXPR * 2];
    int tokenCount = 0;

    // Tokenize the entire input after the assignment operator
    while (currentToken.type != TOKEN_EOF && currentToken.type != TOKEN_SEMICOLON) {
        tokens[tokenCount++] = currentToken;
        getNextToken();
    }

    // Collect expressions of the form operand operator operand
    while (tokenIndex < tokenCount - 2) {
        if (tokens[tokenIndex].type == TOKEN_IDENTIFIER &&
            tokens[tokenIndex + 1].type == TOKEN_OPERATOR &&
            tokens[tokenIndex + 2].type == TOKEN_IDENTIFIER) {

            // Build the expression string
            char exprStr[MAX_TOKEN_LEN * 3];
            snprintf(exprStr, sizeof(exprStr), "%s%s%s",
                     tokens[tokenIndex].text,
                     tokens[tokenIndex + 1].text,
                     tokens[tokenIndex + 2].text);

            // Store the expression
            strcpy(expressions[exprCount].expression, exprStr);
            expressions[exprCount].exprNumber = exprCount + 1;
            exprCount++;

            // Move ahead by one token to find overlapping expressions
            tokenIndex++;
        } else {
            tokenIndex++;
        }
    }
}

void printExpressions() {
    printf("\nThe Expressions available in the given statements are\n");
    for (int i = 0; i < exprCount; i++) {
        printf("Expression %d : %s\n", expressions[i].exprNumber, expressions[i].expression);
    }
}

void findCommonExpressions() {
    printf("\nThe Common Expression Available in given statement are\n");
    int foundCommon = 0;
    for (int i = 0; i < exprCount - 1; i++) {
        for (int j = i + 1; j < exprCount; j++) {
            if (strcmp(expressions[i].expression, expressions[j].expression) == 0) {
                printf("Expression%d & Expression%d --> %s\n",
                       expressions[i].exprNumber,
                       expressions[j].exprNumber,
                       expressions[i].expression);
                foundCommon = 1;
            }
        }
    }
    if (!foundCommon) {
        printf("There is no Common Expression in given statement\n");
    }
}

int main() {
    char buffer[1024];

    printf("Enter the Input String : ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    input = buffer;
    pos = 0;

    getNextToken();

    // Parse LHS
    if (currentToken.type != TOKEN_IDENTIFIER) {
        error("Expected identifier on the left-hand side");
    }
    getNextToken();

    if (currentToken.type != TOKEN_ASSIGN) {
        error("Expected '=' after identifier");
    }
    getNextToken();

    // Collect expressions
    collectExpressions();

    // Print expressions
    printExpressions();

    // Find common expressions
    findCommonExpressions();

    return 0;
}
