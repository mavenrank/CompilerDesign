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