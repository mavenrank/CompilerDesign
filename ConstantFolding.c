#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKEN_LEN 100

typedef enum {
    TOKEN_EOF,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_OPERATOR,
    TOKEN_ASSIGN,
    TOKEN_SEMICOLON,
    TOKEN_LPAREN,
    TOKEN_RPAREN
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

int isOperator(char c) {
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
    } else if (isdigit(input[pos])) {
        int start = pos;
        while (isdigit(input[pos])) {
            pos++;
        }
        int len = pos - start;
        strncpy(currentToken.text, &input[start], len);
        currentToken.text[len] = '\0';
        currentToken.type = TOKEN_NUMBER;
    } else if (isOperator(input[pos])) {
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
    } else if (input[pos] == '(') {
        currentToken.type = TOKEN_LPAREN;
        currentToken.text[0] = '(';
        currentToken.text[1] = '\0';
        pos++;
    } else if (input[pos] == ')') {
        currentToken.type = TOKEN_RPAREN;
        currentToken.text[0] = ')';
        currentToken.text[1] = '\0';
        pos++;
    } else {
        error("Unknown character encountered");
    }
}

typedef struct Node {
    enum { NODE_NUMBER, NODE_IDENTIFIER, NODE_OPERATOR } type;
    union {
        int number;
        char identifier[MAX_TOKEN_LEN];
        struct {
            char operator;
            struct Node *left;
            struct Node *right;
        } op;
    } data;
} Node;

Node *factor();
Node *term();
Node *expr();

Node *newNodeNumber(int value) {
    Node *node = malloc(sizeof(Node));
    node->type = NODE_NUMBER;
    node->data.number = value;
    return node;
}

Node *newNodeIdentifier(const char *name) {
    Node *node = malloc(sizeof(Node));
    node->type = NODE_IDENTIFIER;
    strcpy(node->data.identifier, name);
    return node;
}

Node *newNodeOperator(char operator, Node *left, Node *right) {
    Node *node = malloc(sizeof(Node));
    node->type = NODE_OPERATOR;
    node->data.op.operator = operator;
    node->data.op.left = left;
    node->data.op.right = right;
    return node;
}

Node *factor() {
    Node *node;
    if (currentToken.type == TOKEN_NUMBER) {
        int value = atoi(currentToken.text);
        node = newNodeNumber(value);
        getNextToken();
    } else if (currentToken.type == TOKEN_IDENTIFIER) {
        node = newNodeIdentifier(currentToken.text);
        getNextToken();
    } else if (currentToken.type == TOKEN_LPAREN) {
        getNextToken(); // consume '('
        node = expr();
        if (currentToken.type != TOKEN_RPAREN) {
            error("Expected ')'");
        }
        getNextToken();
    } else {
        error("Invalid factor");
    }
    return node;
}

Node *term() {
    Node *node = factor();
    while (currentToken.type == TOKEN_OPERATOR && (currentToken.text[0] == '*' || currentToken.text[0] == '/')) {
        char op = currentToken.text[0];
        getNextToken();
        Node *right = factor();
        node = newNodeOperator(op, node, right);
    }
    return node;
}

Node *expr() {
    Node *node = term();
    while (currentToken.type == TOKEN_OPERATOR && (currentToken.text[0] == '+' || currentToken.text[0] == '-')) {
        char op = currentToken.text[0];
        getNextToken();
        Node *right = term();
        node = newNodeOperator(op, node, right);
    }
    return node;
}

void optimizeConstants(Node **node, int *constValue, int *hasConst) {
    if (*node == NULL) return;

    if ((*node)->type == NODE_OPERATOR) {
        Node *left = (*node)->data.op.left;
        Node *right = (*node)->data.op.right;
        int leftConst = 0, rightConst = 0;
        int leftValue = 0, rightValue = 0;

        optimizeConstants(&left, &leftValue, &leftConst);
        optimizeConstants(&right, &rightValue, &rightConst);

        if (leftConst && rightConst) {
            int result;
            switch ((*node)->data.op.operator) {
                case '+':
                    result = leftValue + rightValue;
                    break;
                case '-':
                    result = leftValue - rightValue;
                    break;
                case '*':
                    result = leftValue * rightValue;
                    break;
                case '/':
                    if (rightValue == 0) {
                        error("Division by zero");
                    }
                    result = leftValue / rightValue;
                    break;
                default:
                    error("Unknown operator");
            }
            free(*node);
            *node = newNodeNumber(result);
            *constValue = result;
            *hasConst = 1;
        } else {
            (*node)->data.op.left = left;
            (*node)->data.op.right = right;

            if (leftConst) {
                *constValue = leftValue;
                *hasConst = 1;
            } else if (rightConst) {
                *constValue = rightValue;
                *hasConst = 1;
            } else {
                *hasConst = 0;
            }
        }
    } else if ((*node)->type == NODE_NUMBER) {
        *constValue = (*node)->data.number;
        *hasConst = 1;
    } else {
        *hasConst = 0;
    }
}

void printOptimizedExpr(Node *node) {
    if (node == NULL) return;

    if (node->type == NODE_OPERATOR) {
        if (node->data.op.operator == '+' || node->data.op.operator == '-') {
            printOptimizedExpr(node->data.op.left);
            printf("%c", node->data.op.operator);
            printOptimizedExpr(node->data.op.right);
        } else {
            printOptimizedExpr(node->data.op.left);
            printf("%c", node->data.op.operator);
            printOptimizedExpr(node->data.op.right);
        }
    } else if (node->type == NODE_IDENTIFIER) {
        printf("%s", node->data.identifier);
    } else if (node->type == NODE_NUMBER) {
        printf("%d", node->data.number);
    }
}

int collectConstants(Node *node) {
    if (node == NULL) return 0;

    if (node->type == NODE_NUMBER) {
        return node->data.number;
    } else if (node->type == NODE_OPERATOR) {
        int leftValue = collectConstants(node->data.op.left);
        int rightValue = collectConstants(node->data.op.right);

        if (node->data.op.operator == '+') {
            return leftValue + rightValue;
        } else if (node->data.op.operator == '-') {
            return leftValue - rightValue;
        } else if (node->data.op.operator == '*') {
            return leftValue * rightValue;
        } else if (node->data.op.operator == '/') {
            if (rightValue == 0) {
                error("Division by zero");
            }
            return leftValue / rightValue;
        }
    }
    return 0;
}

void freeTree(Node *node) {
    if (node == NULL) return;
    if (node->type == NODE_OPERATOR) {
        freeTree(node->data.op.left);
        freeTree(node->data.op.right);
    }
    free(node);
}

int main() {
    char buffer[1024];

    printf("Enter the Input String : ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';

    input = buffer;
    pos = 0;

    getNextToken();

    if (currentToken.type != TOKEN_IDENTIFIER) {
        error("Expected identifier on the left-hand side");
    }
    char lhs[MAX_TOKEN_LEN];
    strcpy(lhs, currentToken.text);
    getNextToken();

    if (currentToken.type != TOKEN_ASSIGN) {
        error("Expected '=' after identifier");
    }
    getNextToken();

    Node *root = expr();

    int constValue = 0;
    int hasConst = 0;
    optimizeConstants(&root, &constValue, &hasConst);

    printf("%s=", lhs);
    printOptimizedExpr(root);
    printf("\n");

    int totalConstValue = collectConstants(root);
    printf("The value of the constant expression is : %d\n", totalConstValue);

    freeTree(root);
    return 0;
}
