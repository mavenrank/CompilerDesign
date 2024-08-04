#include <iostream>
#include <cstdlib>
#include <string>
using namespace std;

bool checkEmail(string email){
    const string validDomains[] = {
    ".com", ".org", ".net", ".edu", ".gov", ".mil", ".int",
    ".co.uk", ".uk", ".us", ".ca", ".de", ".fr", ".jp",
    ".au", ".it", ".es", ".ru", ".cn", ".in", ".br",
    ".za", ".mx", ".nl", ".se", ".no", ".fi", ".dk",
    ".pl", ".ch", ".at", ".be", ".ie", ".nz", ".sg",
    ".hk", ".kr", ".tw", ".my", ".ae", ".sa", ".eg",
    ".il", ".tr", ".gr", ".pt", ".cz", ".hu", ".ro",
    ".sk", ".bg", ".lt", ".lv", ".ee", ".si", ".hr",
    ".rs", ".ua", ".by", ".kz", ".uz", ".pk", ".bd",
    ".lk", ".np", ".lk", ".pk", ".bd", ".vn", ".ph",
    ".id", ".th", ".mm", ".kh", ".la", ".mn", ".np",
    ".np", ".af", ".gh", ".ke", ".ng", ".za"
    };
    
    size_t atSymbolPosition = email.find('@');
    if (atSymbolPosition == string::npos || email.find('@', atSymbolPosition + 1) != string::npos) {
        if (atSymbolPosition == string::npos){
            cout << "@ symbol is not present in the email address" << endl;
        }
        if (email.find('@', atSymbolPosition + 1) != string::npos){
            cout << "More than one @ symbols are not allowed in email" << endl;
        }
        return false;
    }
    if (atSymbolPosition == 0 || atSymbolPosition == email.length() - 1) {
        cout << "@ canont be at the start or end of the email address"<< endl;
        return false;
    }

    if (isdigit(email[0])) {
        cout << "Email cannot start with a number" << endl;
        return false;
    }

    size_t dotSymbolPosition = email.find('.', atSymbolPosition);
    if (dotSymbolPosition == string::npos || dotSymbolPosition == atSymbolPosition + 1) {
        if (dotSymbolPosition == string::npos){
        cout << ". does not exist in email address" << endl;
        }
        if (dotSymbolPosition == atSymbolPosition + 1){
            cout << "domain name is missing in email address" << endl;
        }
        return false;
    }
    
    
    bool hasAlphaBetween = false;
    for (size_t i = atSymbolPosition + 1; i < dotSymbolPosition; ++i) {
        if (isalpha(email[i])) {
            hasAlphaBetween = true;
            break;
        }
    }
    if (!hasAlphaBetween) {
        cout << "@ and . have no domain between them" << endl;
        return false;
    }

    string domainPart = email.substr(dotSymbolPosition);
    bool validDomain = false;
    for (size_t i = 0; i < sizeof(validDomains) / sizeof(validDomains[0]); ++i) {
        if (domainPart == validDomains[i]) {
            validDomain = true;
            break;
        }
    }
    if (!validDomain){
        cout << "Invalid Domain" << endl;
    }
    return validDomain;
}

bool checkMobileNumber(string mobile) {
    if (mobile.empty()) {
        cout << "Mobile Number not given" << endl;
        return false;
    }

    if (mobile.length() > 10) {
        cout << "Invalid Length" << endl;
        return false;
    }
    
    if (!isdigit(mobile[0])) {
        cout << "Starting number must be a digit" << endl;
        return false;
    }

    if (mobile[0] < '6' || mobile[0] > '9') {
        cout << "Starting number can be between 6 and 9 only" << endl;
        return false;
    }

    for (char c : mobile) {
        if (!isdigit(c)) {
            cout << "Mobile Number must contain only digits" << endl;
            return false;
        }
    }

    return true;
}




int main(){
    cout << "Enter your email : ";
    string email;
    cin >> email;
    if (checkEmail(email)){
        cout << "Valid Email Address" << endl;
    }
    else{
        cout << "not a valid Email address" << endl;
    }
    string mobile;
    cout << "Enter your Mobile Number : " << endl;
    cin >> mobile;
    
    if (checkMobileNumber(mobile)){
        cout << "Valid Mobile Number" << endl;
    }
    else{
        cout << "Not a valid mobile number" << endl;
    }
    
}