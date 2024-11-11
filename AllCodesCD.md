# Compiler Design All Codes

## Lab Assessment - 1

### Lexical Analyzer

Design a basic lexical analyzer in C that analyzes the lexemes and gives tokens and
their type as output

```c
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
```

### Symbol Table

Construct a symbol table that shows each lexeme along with its location, lexeme,
type and size of the lexeme. Also show errors in the code if present.

```c
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
}
```

## Lab Assessment - 2

### Email and Phone Validator

Design an Email, Phone Number and Register Number Validator using FLEX.

```lex
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
```

_flex filename.l_
_gcc lex.yy.c_
_./a.out_

### Lexical Analyzer in flex

Design a Lexical Analyzer that identifies tokens gives their type, Identifier, Keyword, Operator, Delimiter etc.

lex.l

```flex
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
```

_flex filename.l_
_gcc lex.yy.c_
_./a.out_

## Lab Assessment - 3

### Yacc program to validate id = expression;

Create a YACC program that uses Context-Free Grammar (CFG) to validatecassignment statements of the form id = expression;. The program should ensure that the assignment follows the correct syntax, including the proper use of identifiers, operators, and the required semicolon at the end

lex.l

```lex
%{
#include "yacc.tab.h"
%}
L [a-zA-Z]
D [0-9]
ID {L}({L}|{D})*
IN {D}+
AOP [-+*/]
SP [;\n]
AO =
%%
{ID} return IDE;
{IN} return INUM;
{AOP} return *yytext;
{SP} return *yytext;
{AO} return *yytext;
%%
```

yacc.y

```yacc
%{
#include"yacc.tab.h"
#include<stdio.h>
int yylex(void);
void yyerror(char *);
%}

%token IDE INUM

%%
S :IDE'='E';''\n' {printf("Valid Syntax");}
E :IDE OP IDE | IDE OP INUM | INUM OP IDE | INUM OP INUM | IDE
| INUM
OP : '+' | '-' | '*' | '/'

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
```

_flex filename.l_
_bison -d yacc.y_
_gcc lex.yy.c yacc.tab.c_
_./a.out_

## Lab Assessment - 4

### Simple Calculator using Yacc

YACC Program to Design a Simple Calculator using Expression Grammar

lex.l

```lex
%{
#include <stdio.h>
#include "simplecalc.tab.h"
extern int yylval;
%}

L [a-zA-Z]
D [0-9]
ID {L}|({L}|{D})*
NUM {D}+
AO [-+*/]
PO [()]
ASO =
SO [;\n]

%%

{NUM} {yylval=atoi(yytext); return (NUM);}
{AO} return *yytext;
{PO} return *yytext;
{ASO} return *yytext;
{SO} return *yytext;

%%
```

yacc.y

```yacc
%{
#include <stdio.h>
#include "simplecalc.tab.h"
void yyerror(char *);
int yylex(void);
%}

%token NUM

%%
S: E ';''\n' {printf("%d \n", $1);}
E: E '+' T {$$ = $1 + $3;}
| E '-' T {$$ = $1 - $3;}
| T {$$ = $1;}
T: T '*' F {$$ = $1 * $3;}
| T '/' F {$$ = $1 / $3;}
| T '%' F {$$ = $1 % $3;}
| F {$$ = $1;}
F: '(' E ')' {$$ = $2;}
| NUM {$$ = $1;}

%%

int main(){
    printf("Enter the statement :");
    yyparse();
    return 0;
}

int yywrap(){
    return 1;
}

void yyerror(char * err){
    fprintf(stderr, "%s", err);
}
```

_flex lex.l_
_bison -d yacc.y_
_gcc lex.yy.c yacc.tab.c_
_./a.out_

### Predictive Parser

C/C++/Java/Python Program to Design a Predictive Parsing Table for the given CFG

```c++
#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;
struct Grammar
{
    map<char, vector<string>> rules;
};
void eliminateLeftRecursion(Grammar &grammar)
{
    map<char, vector<string>> newRules;
    for (auto &[nonTerminal, productions] : grammar.rules)
    {
        vector<string> alpha, beta;
        for (auto &production : productions)
        {
            if (production[0] == nonTerminal)
            {
                alpha.push_back(production.substr(1));
            }
            else
            {
                beta.push_back(production);
            }
        }
        if (!alpha.empty())
        {
            char newNonTerminal = nonTerminal + '\'';
            for (auto &b : beta)
            {
                newRules[nonTerminal].push_back(b + newNonTerminal);
            }
            for (auto &a : alpha)
            {
                newRules[newNonTerminal].push_back(a + newNonTerminal);
            }
            newRules[newNonTerminal].push_back("ε");
        }
        else
        {
            newRules[nonTerminal] = productions;
        }
    }
    grammar.rules = newRules;
}
void performLeftFactoring(Grammar &grammar)
{
    map<char, vector<string>> newRules;
    for (auto &[nonTerminal, productions] : grammar.rules)
    {
        map<string, vector<string>> prefixMap;
        for (auto &production : productions)
        {
            string prefix;
            for (char ch : production)
            {
                if (isupper(ch))
                    break;
                prefix += ch;
            }
            prefixMap[prefix].push_back(production);
        }
        for (auto &[prefix, group] : prefixMap)
        {
            if (group.size() > 1)
            {
                char newNonTerminal = nonTerminal + '\'';
                newRules[nonTerminal].push_back(prefix + newNonTerminal);
                for (auto &prod : group)
                {
                    newRules[newNonTerminal].push_back(prod.substr(prefix.size()));
                }
            }
            else
            {
                newRules[nonTerminal].push_back(group[0]);
            }
        }
    }
    grammar.rules = newRules;
}
void computeFirst(Grammar &grammar, map<char, set<char>> &first)
{
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (auto &[nonTerminal, productions] : grammar.rules)
        {
            for (auto &production : productions)
            {
                for (char symbol : production)
                {
                    if (isupper(symbol))
                    {
                        int sizeBefore = first[nonTerminal].size();
                        first[nonTerminal].insert(first[symbol].begin(),
                                                  first[symbol].end());
                        int sizeAfter = first[nonTerminal].size();
                        if (sizeAfter > sizeBefore)
                            changed = true;
                        if (first[symbol].find('ε') == first[symbol].end())
                            break;
                    }
                    else
                    {
                        if (first[nonTerminal].find(symbol) ==
                            first[nonTerminal].end())
                        {
                            first[nonTerminal].insert(symbol);
                            changed = true;
                        }
                        break;
                    }
                }
            }
        }
    }
}
void computeFollow(Grammar &grammar, map<char, set<char>> &first, map<char, set<char>> &follow)
{
    follow['S'].insert('$');
    bool changed = true;
    while (changed)
    {
        changed = false;
        for (auto &[nonTerminal, productions] : grammar.rules)
        {
            for (auto &production : productions)
            {
                for (size_t i = 0; i < production.size(); i++)
                {
                    char symbol = production[i];
                    if (isupper(symbol))
                    {
                        size_t j = i + 1;
                        while (j < production.size())
                        {
                            char nextSymbol = production[j];
                            if (isupper(nextSymbol))
                            {
                                int sizeBefore = follow[symbol].size();
                                set<char> tempFirst = first[nextSymbol];
                                tempFirst.erase('ε');
                                follow[symbol].insert(tempFirst.begin(),
                                                      tempFirst.end());
                                int sizeAfter = follow[symbol].size();
                                if (sizeAfter > sizeBefore)
                                    changed = true;
                                if (first[nextSymbol].find('ε') ==
                                    first[nextSymbol].end())
                                    break;
                            }
                            else
                            {
                                if (follow[symbol].find(nextSymbol) ==
                                    follow[symbol].end())
                                {
                                    follow[symbol].insert(nextSymbol);
                                    changed = true;
                                }
                                break;
                            }
                            j++;
                        }
                        if (j == production.size())
                        {
                            int sizeBefore = follow[symbol].size();
                            follow[symbol].insert(follow[nonTerminal].begin(),
                                                  follow[nonTerminal].end());
                            int sizeAfter = follow[symbol].size();
                            if (sizeAfter > sizeBefore)
                                changed = true;
                        }
                    }
                }
            }
        }
    }
}
void constructParsingTable(Grammar &grammar, map<char, set<char>> &first,
                           map<char, set<char>> &follow, map<char, map<char, string>> &parsingTable)
{
    for (auto &[nonTerminal, productions] : grammar.rules)
    {
        for (auto &production : productions)
        {
            set<char> firstSet;
            bool containsEpsilon = false;
            for (char symbol : production)
            {
                if (isupper(symbol))
                {
                    firstSet.insert(first[symbol].begin(),
                                    first[symbol].end());
                    if (first[symbol].find('ε') == first[symbol].end())
                        break;
                    containsEpsilon = true;
                }
                else
                {
                    firstSet.insert(symbol);
                    break;
                }
            }
            firstSet.erase('ε');
            for (char terminal : firstSet)
            {
                parsingTable[nonTerminal][terminal] = nonTerminal +
                                                      string("->") + production;
            }
            if (containsEpsilon || firstSet.empty())
            {
                for (char terminal : follow[nonTerminal])
                {
                    parsingTable[nonTerminal][terminal] = nonTerminal +
                                                          string("->") + production;
                }
            }
        }
    }
}
void printParsingTable(map<char, map<char, string>> &parsingTable)
{
    cout << "\nPredictive Parsing Table:\n";
    cout << setw(15) << left << "Non-Terminal"
         << setw(15) << left << "Terminal"
         << setw(30) << left << "Production" << endl;
    cout << "---------------------------------------------------------------\n";
    for (auto &[nonTerminal, row] : parsingTable)
    {
        for (auto &[terminal, production] : row)
        {
            cout << setw(15) << left << nonTerminal
                 << setw(15) << left << terminal
                 << setw(30) << left << production << endl;
        }
    }
}
int main()
{
    Grammar grammar;
    int numRules;
    cout << "Enter the number of grammar rules: ";
    cin >> numRules;
    cin.ignore();
    cout << "Enter the grammar rules (in the form A->BCD, where A is a non-terminal and B, C, D are symbols):\n";
    for (int i = 0; i < numRules; i++)
    {
        string rule;
        getline(cin, rule);
        char nonTerminal = rule[0];
        string production = rule.substr(3);
        grammar.rules[nonTerminal].push_back(production);
    }
    eliminateLeftRecursion(grammar);
    performLeftFactoring(grammar);
    map<char, set<char>> first, follow;
    computeFirst(grammar, first);
    computeFollow(grammar, first, follow);
    map<char, map<char, string>> parsingTable;
    constructParsingTable(grammar, first, follow, parsingTable);
    printParsingTable(parsingTable);
    return 0;
}
```

## Lab Assessment - 5

### Type checking for given expression

Write a program that performs type checking for a given expression, where all the variables in the expression have already been declared. Check the data type of each identifier or constant in the right-side expression against the left-side identifier to determine whether they match

```c++
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <vector>
#include <algorithm>
using namespace std;
vector<string> split(const string &str, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}
string removeSpaces(const string &input)
{
    string result;
    for (char c : input)
    {
        if (c != ' ')
            result += c;
    }
    return result;
}
string removeSemicolon(const string &input)
{
    if (!input.empty() && input.back() == ';')
    {
        return input.substr(0, input.size() - 1);
    }
    return input;
}
int main()
{
    int n;
    cout << "Enter the number of declarations and expressions: ";
    cin >> n;
    cin.ignore();
    map<string, string> varTypes;
    string declaration, expression;
    for (int i = 0; i < n; ++i)
    {
        cout << "Enter declaration " << i + 1 << " (e.g., 'int a,b; float c,d;'): ";
        getline(cin, declaration);
        vector<string> declParts = split(declaration, ';');
        for (const auto &part : declParts)
        {
            istringstream iss(part);
            string type, variables;
            iss >> type;
            getline(iss, variables);
            variables = removeSpaces(variables);
            vector<string> vars = split(variables, ',');
            for (const auto &var : vars)
            {
                varTypes[var] = type;
            }
        }
    }
    for (int i = 0; i < n; ++i)
    {
        cout << "Enter expression " << i + 1 << " : ";
        getline(cin, expression);
        expression = removeSemicolon(expression);
        vector<string> exprParts = split(expression, '=');
        if (exprParts.size() != 2)
        {
            cout << "Invalid expression. " << endl;
            continue;
        }
        string lhs = exprParts[0];
        string rhs = exprParts[1];
        lhs = removeSpaces(lhs);
        rhs = removeSpaces(rhs);
        vector<string> rhsVars = split(rhs, '+');
        if (varTypes.find(lhs) == varTypes.end())
        {
            cout << "Error: Variable '" << lhs << "' not declared." << endl;
            continue;
        }
        string lhsType = varTypes[lhs];
        bool typeMismatch = false;
        for (const auto &rhsVar : rhsVars)
        {
            if (varTypes.find(rhsVar) == varTypes.end())
            {
                cout << "Error: Variable '" << rhsVar << "' not declared." << endl;
                typeMismatch = true;
                break;
            }
            string rhsType = varTypes[rhsVar];
            if (lhsType != rhsType)
            {
                cout << "Type mismatch: '" << lhs << "' (" << lhsType << ") does not
                                                                         match '" << rhsVar << "'(" << rhsType << ")
                                                                             ." << endl;
                                                                         typeMismatch = true;
            }
        }
        if (!typeMismatch)
        {
            cout << "Types of all variables in the expression '" << expression << "'
                                                                                  match." << endl;
        }
    }
    return 0;
}
```

### Translation of Three Address Code to Assembly Code

Write a program to translate the given Three Address Code to Assembly Code. If the Three Address Code is in the form x = y op z (where y and z are identifiers or constants), the corresponding assembly code should be:
MOV z, R
OP y, R
MOV R, x
If either y or z is a float, the opcode should end with an F (e.g., MOVF, ADDF, etc.)

```c++
#include <iostream>
#include <string>
#include <algorithm>
#include <cctype>
using namespace std;
bool isFloat(const string &s)
{
    bool hasDecimal = false;
    bool hasDigits = false;
    for (size_t i = 0; i < s.size(); ++i)
    {
        if (isdigit(s[i]))
        {
            hasDigits = true;
        }
        else if (s[i] == '.')
        {
            if (hasDecimal)
                return false;
            hasDecimal = true;
        }
        else if (s[i] == '-' || s[i] == '+')
        {
            if (i != 0)
                return false;
        }
        else
        {
            return false;
        }
    }
    return hasDigits;
}
bool isInteger(const string &s)
{
    return !s.empty() && (isdigit(s[0]) || s[0] == '-' || s[0] == '+') &&
           s.find('.') == string::npos;
}
bool isFloatingPoint(const string &s)
{
    return isFloat(s) && s.find('.') != string::npos;
}
void tacToAssembly(string tac)
{
    string result, op1, op2;
    char op;
    size_t equalsPos = tac.find('=');
    result = tac.substr(0, equalsPos);
    size_t operatorPos;
    if (tac.find('+') != string::npos)
    {
        op = '+';
        operatorPos = tac.find('+');
    }
    else if (tac.find('-') != string::npos)
    {
        op = '-';
        operatorPos = tac.find('-');
    }
    else if (tac.find('*') != string::npos)
    {
        op = '*';
        operatorPos = tac.find('*');
    }
    else if (tac.find('/') != string::npos)
    {
        op = '/';
        operatorPos = tac.find('/');
    }
    else
    {
        cout << "Unsupported operator" << endl;
        return;
    }
    op1 = tac.substr(equalsPos + 1, operatorPos - equalsPos - 1);
    op2 = tac.substr(operatorPos + 1);
    result.erase(remove(result.begin(), result.end(), ' '), result.end());
    op1.erase(remove(op1.begin(), op1.end(), ' '), op1.end());
    op2.erase(remove(op2.begin(), op2.end(), ' '), op2.end());
    bool op1IsFloat = isFloatingPoint(op1);
    bool op2IsFloat = isFloatingPoint(op2);
    bool op1IsInteger = isInteger(op1);
    bool op2IsInteger = isInteger(op2);
    if (op2IsFloat)
    {
        cout << "MOVF #" << op2 << ", R1" << endl;
    }
    else if (op2IsInteger)
    {
        cout << "MOV #" << op2 << ", R1" << endl;
    }
    else
    {
        cout << "MOV " << op2 << ", R1" << endl;
    }
    if (op1IsFloat)
    {
        cout << "ADDF " << (op2IsInteger ? "#" + op1 : op1) << ", R1" << endl;
    }
    else
    {
        cout << "ADD " << (op1IsInteger ? "#" + op1 : op1) << ", R1" << endl;
    }
    cout << "MOVF R1, " << result << endl; // Use MOVF if any operand is a float
}
int main()
{
    string tac;
    cout << "Enter the Three Address Code: ";
    getline(cin, tac);
    cout << "----------------------------------------" << endl;
    tacToAssembly(tac);
    cout << "----------------------------------------" << endl;
    return 0;
}
```

## Lab Assessment - 6

### Constant Folding

```c
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
```

### Expression Finding

```c
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
```
