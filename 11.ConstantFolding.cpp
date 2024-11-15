#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <stack>
#include <sstream>
#include <algorithm> // Required for std::find

using namespace std;

// Function to tokenize the input string
vector<string> tokenize(const string& input) {
    vector<string> tokens;
    string token;
    for (size_t i = 0; i < input.size(); i++) { // Changed to post-increment
        char ch = input[i];

        if (isalpha(ch)) {
            // Start of an identifier
            token.clear();
            while (i < input.size() && (isalnum(input[i]) || input[i] == '_')) {
                token += input[i];
                i++; // Changed to post-increment
            }
            i--; // Decrement to adjust for the last iteration
            tokens.push_back(token);
        } else if (isdigit(ch)) {
            // Start of a number
            token.clear();
            while (i < input.size() && isdigit(input[i])) {
                token += input[i];
                i++; // Changed to post-increment
            }
            i--; // Decrement to adjust for the last iteration
            tokens.push_back(token);
        } else if (ispunct(ch)) {
            // Single operator or punctuation
            tokens.push_back(string(1, ch));
        }
    }
    return tokens;
}

// Function to evaluate a simple arithmetic expression
int evaluateArithmetic(const vector<string>& expression) {
    stack<int> operands;
    stack<char> operators;

    for (size_t i = 0; i < expression.size(); i++) { // Changed to post-increment
        const string& token = expression[i];
        if (isdigit(token[0])) {
            operands.push(stoi(token));
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            while (!operators.empty() &&
                   ((token == "+" || token == "-") || 
                    (token == "*" || token == "/"))) {
                char op = operators.top();
                operators.pop();
                int b = operands.top(); operands.pop();
                int a = operands.top(); operands.pop();
                if (op == '+') operands.push(a + b);
                else if (op == '-') operands.push(a - b);
                else if (op == '*') operands.push(a * b);
                else if (op == '/') operands.push(a / b);
            }
            operators.push(token[0]);
        }
    }

    while (!operators.empty()) {
        char op = operators.top(); operators.pop();
        int b = operands.top(); operands.pop();
        int a = operands.top(); operands.pop();
        if (op == '+') operands.push(a + b);
        else if (op == '-') operands.push(a - b);
        else if (op == '*') operands.push(a * b);
        else if (op == '/') operands.push(a / b);
    }

    return operands.top();
}

// Function to process the arithmetic expression
vector<string> processArithmetic(vector<string>& tokens) {
    vector<string> expression;
    string beforeArithmetic;
    size_t i = 0;

    // Find the '=' operator
    while (i < tokens.size() && tokens[i] != "=") {
        i++; // Changed to post-increment
    }

    // Add everything before '=' to the result
    vector<string> result(tokens.begin(), tokens.begin() + i + 1);

    // Process the arithmetic part
    i++; // Move past '=' using post-increment
    vector<string> identifiers;
    while (i < tokens.size()) {
        if (isdigit(tokens[i][0])) {
            // Number found
            expression.push_back(tokens[i]);

            // Check the next token
            if (i + 1 < tokens.size() &&
                (tokens[i + 1] == "+" || tokens[i + 1] == "-" ||
                 tokens[i + 1] == "*" || tokens[i + 1] == "/")) {
                expression.push_back(tokens[i + 1]);
                i++; // Changed to post-increment
            } else {
                break; // Stop if no operator follows
            }
        } else if (isalpha(tokens[i][0])) {
            // Identifier found, store it separately
            identifiers.push_back(tokens[i]);
            // Remove the previous operator if exists
            if (!expression.empty() &&
                (expression.back() == "+" || expression.back() == "-" || 
                 expression.back() == "*" || expression.back() == "/")) {
                expression.pop_back();
            }
        } else if (tokens[i] == ";") {
            break;
        } else {
            // Handle beforeArithmetic operator
            if (beforeArithmetic.empty() && (tokens[i] == "+" || tokens[i] == "-")) {
                beforeArithmetic = tokens[i];
            }
        }
        i++; // Changed to post-increment
    }

    // Calculate the arithmetic value
    int arithmeticValue = evaluateArithmetic(expression);

    // Find and remove the semicolon
    auto it = find(tokens.begin(), tokens.end(), std::string(";"));
    if (it != tokens.end()) {
        tokens.erase(it);
    }

    // Append all identifiers to the result
    for (size_t j = 0; j < identifiers.size(); j++) { // Post-increment in loop
        result.push_back(identifiers[j]);
        if (j != identifiers.size() - 1) { // Avoid trailing '+' after the last identifier
            result.push_back("+");
        }
    }

    // Append the arithmetic value with beforeArithmetic
    if (!beforeArithmetic.empty() && !identifiers.empty()) {
        result.push_back(beforeArithmetic);
    }
    result.push_back(to_string(arithmeticValue));

    // Add the semicolon back
    result.push_back(";");

    return result;
}

int main() {
    string input;
    cout << "Enter the Input String: ";
    getline(cin, input);

    vector<string> tokens = tokenize(input);
    cout << "Tokens:" << endl;
    for (const auto& token : tokens) {
        cout << token << " ";
    }
    cout << endl;

    vector<string> result = processArithmetic(tokens);

    cout << "Processed Expression:" << endl;
    for (const auto& token : result) {
        cout << token;
    }
    cout << endl;

    return 0;
}
