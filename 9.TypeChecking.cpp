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
        cout << "Enter declaration " << i + 1 << " (e.g., 'int a,b;' or 'float c,d;'): ";
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
                cout << "Type mismatch: '" << lhs << "' (" << lhsType << ") does not match '" << rhsVar << "'(" << rhsType << ")." << endl; 
                typeMismatch = true;
            }
        }
        if (!typeMismatch)
        {
            cout << "Types of all variables in the expression '" << expression << "'match." << endl;
        }
    }
    return 0;
}