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

typedef struct {
    char expression[MAX_TOKEN_LEN * 3];
    int exprNumber;
} Expression;

char *input;
int pos = 0;
Token currentToken;
Expression expressions[MAX_EXPR];
int exprCount = 0;

// Helper Functions
void skipWhitespace() {
    while (isspace(input[pos])) pos++;
}

int isOperatorChar(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Tokenizer
void getNextToken() {
    skipWhitespace();
    if (input[pos] == '\0') {
        currentToken.type = TOKEN_EOF;
        return;
    }
    if (isalpha(input[pos])) {
        int start = pos;
        while (isalnum(input[pos])) pos++;
        strncpy(currentToken.text, &input[start], pos - start);
        currentToken.text[pos - start] = '\0';
        currentToken.type = TOKEN_IDENTIFIER;
    } else if (isOperatorChar(input[pos])) {
        currentToken.type = TOKEN_OPERATOR;
        currentToken.text[0] = input[pos++];
        currentToken.text[1] = '\0';
    } else if (input[pos] == '=') {
        currentToken.type = TOKEN_ASSIGN;
        currentToken.text[0] = input[pos++];
        currentToken.text[1] = '\0';
    } else if (input[pos] == ';') {
        currentToken.type = TOKEN_SEMICOLON;
        currentToken.text[0] = input[pos++];
        currentToken.text[1] = '\0';
    } else {
        error("Unknown character encountered");
    }
}

// Error Handler
void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(EXIT_FAILURE);
}

// Expression Collector
void collectExpressions() {
    Token tokens[MAX_EXPR * 2];
    int tokenCount = 0;

    while (currentToken.type != TOKEN_EOF && currentToken.type != TOKEN_SEMICOLON) {
        tokens[tokenCount++] = currentToken;
        getNextToken();
    }

    for (int i = 0; i < tokenCount - 2; i++) {
        if (tokens[i].type == TOKEN_IDENTIFIER &&
            tokens[i + 1].type == TOKEN_OPERATOR &&
            tokens[i + 2].type == TOKEN_IDENTIFIER) {
            snprintf(expressions[exprCount].expression, sizeof(expressions[exprCount].expression),
                     "%s%s%s", tokens[i].text, tokens[i + 1].text, tokens[i + 2].text);
            expressions[exprCount].exprNumber = exprCount + 1;
            exprCount++;
        }
    }
}

// Print Expressions
void printExpressions() {
    printf("\nExpressions Found:\n");
    for (int i = 0; i < exprCount; i++) {
        printf("Expression %d: %s\n", expressions[i].exprNumber, expressions[i].expression);
    }
}

// Find Common Expressions
void findCommonExpressions() {
    printf("\nCommon Expressions:\n");
    int foundCommon = 0;
    for (int i = 0; i < exprCount - 1; i++) {
        for (int j = i + 1; j < exprCount; j++) {
            if (strcmp(expressions[i].expression, expressions[j].expression) == 0) {
                printf("Expression %d & %d: %s\n",
                       expressions[i].exprNumber, expressions[j].exprNumber, expressions[i].expression);
                foundCommon = 1;
            }
        }
    }
    if (!foundCommon) {
        printf("No common expressions found.\n");
    }
}

// Main Function
int main() {
    char buffer[1024];
    printf("Enter Input String: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character

    input = buffer;
    pos = 0;
    getNextToken();

    if (currentToken.type != TOKEN_IDENTIFIER) error("Expected identifier on LHS");
    getNextToken();
    if (currentToken.type != TOKEN_ASSIGN) error("Expected '=' after identifier");
    getNextToken();

    collectExpressions();
    printExpressions();
    findCommonExpressions();

    return 0;
}
