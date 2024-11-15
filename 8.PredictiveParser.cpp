#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct Grammar {
    map<char, vector<string>> rules;
};

void eliminateLeftRecursion(Grammar &grammar) {
    map<char, vector<string>> newRules;

    for (auto &[nonTerminal, productions] : grammar.rules) {
        vector<string> alpha, beta;

        // Separate productions into alpha and beta
        for (auto &production : productions) {
            if (production[0] == nonTerminal) {
                alpha.push_back(production.substr(1)); // Direct left recursion
            } else {
                beta.push_back(production);
            }
        }

        if (!alpha.empty()) {
            char newNonTerminal = nonTerminal + '\'';

            // Add beta productions
            for (auto &b : beta) {
                newRules[nonTerminal].push_back(b + newNonTerminal);
            }

            // Add alpha productions
            for (auto &a : alpha) {
                newRules[newNonTerminal].push_back(a + newNonTerminal);
            }

            // Add epsilon production
            newRules[newNonTerminal].push_back("ε");
        } else {
            newRules[nonTerminal] = productions;
        }
    }

    grammar.rules = newRules;
}

void performLeftFactoring(Grammar &grammar) {
    map<char, vector<string>> newRules;

    for (auto &[nonTerminal, productions] : grammar.rules) {
        map<string, vector<string>> prefixGroups;

        // Group productions by common prefix
        for (auto &production : productions) {
            string prefix;
            for (char ch : production) {
                if (isupper(ch)) break;
                prefix += ch;
            }
            prefixGroups[prefix].push_back(production);
        }

        // Handle each prefix group
        for (auto &[prefix, group] : prefixGroups) {
            if (group.size() > 1) {
                char newNonTerminal = nonTerminal + '\'';
                newRules[nonTerminal].push_back(prefix + newNonTerminal);

                for (auto &prod : group) {
                    string suffix = prod.substr(prefix.size());
                    newRules[newNonTerminal].push_back(suffix.empty() ? "ε" : suffix);
                }
            } else {
                newRules[nonTerminal].push_back(group[0]);
            }
        }
    }

    grammar.rules = newRules;
}

void computeFirst(Grammar &grammar, map<char, set<char>> &first) {
    bool changed = true;

    while (changed) {
        changed = false;

        for (auto &[nonTerminal, productions] : grammar.rules) {
            for (auto &production : productions) {
                for (char symbol : production) {
                    if (isupper(symbol)) {
                        size_t beforeSize = first[nonTerminal].size();
                        first[nonTerminal].insert(first[symbol].begin(), first[symbol].end());
                        size_t afterSize = first[nonTerminal].size();

                        if (afterSize > beforeSize) changed = true;

                        if (first[symbol].find('ε') == first[symbol].end()) break;
                    } else {
                        if (first[nonTerminal].insert(symbol).second) {
                            changed = true;
                        }
                        break;
                    }
                }
            }
        }
    }
}

void computeFollow(Grammar &grammar, map<char, set<char>> &first, map<char, set<char>> &follow) {
    follow['S'].insert('$');
    bool changed = true;

    while (changed) {
        changed = false;

        for (auto &[nonTerminal, productions] : grammar.rules) {
            for (auto &production : productions) {
                for (size_t i = 0; i < production.size(); i++) {
                    char symbol = production[i];
                    if (isupper(symbol)) {
                        size_t j = i + 1;

                        while (j < production.size()) {
                            char nextSymbol = production[j];
                            if (isupper(nextSymbol)) {
                                set<char> tempFirst = first[nextSymbol];
                                tempFirst.erase('ε');
                                size_t beforeSize = follow[symbol].size();
                                follow[symbol].insert(tempFirst.begin(), tempFirst.end());
                                size_t afterSize = follow[symbol].size();

                                if (afterSize > beforeSize) changed = true;

                                if (first[nextSymbol].find('ε') == first[nextSymbol].end()) break;
                            } else {
                                if (follow[symbol].insert(nextSymbol).second) {
                                    changed = true;
                                }
                                break;
                            }
                            j++;
                        }

                        if (j == production.size()) {
                            size_t beforeSize = follow[symbol].size();
                            follow[symbol].insert(follow[nonTerminal].begin(), follow[nonTerminal].end());
                            size_t afterSize = follow[symbol].size();

                            if (afterSize > beforeSize) changed = true;
                        }
                    }
                }
            }
        }
    }
}

void constructParsingTable(Grammar &grammar, map<char, set<char>> &first,
                           map<char, set<char>> &follow, map<char, map<char, string>> &parsingTable) {
    for (auto &[nonTerminal, productions] : grammar.rules) {
        for (auto &production : productions) {
            set<char> firstSet;
            bool containsEpsilon = false;

            for (char symbol : production) {
                if (isupper(symbol)) {
                    firstSet.insert(first[symbol].begin(), first[symbol].end());
                    if (first[symbol].find('ε') == first[symbol].end()) break;
                    containsEpsilon = true;
                } else {
                    firstSet.insert(symbol);
                    break;
                }
            }

            firstSet.erase('ε');
            for (char terminal : firstSet) {
                parsingTable[nonTerminal][terminal] = nonTerminal + string("->") + production;
            }

            if (containsEpsilon || firstSet.empty()) {
                for (char terminal : follow[nonTerminal]) {
                    parsingTable[nonTerminal][terminal] = nonTerminal + string("->") + production;
                }
            }
        }
    }
}

void printParsingTable(map<char, map<char, string>> &parsingTable) {
    cout << "\nPredictive Parsing Table:\n";
    cout << setw(15) << left << "Non-Terminal"
         << setw(15) << left << "Terminal"
         << setw(30) << left << "Production" << endl;
    cout << "---------------------------------------------------------------\n";

    for (auto &[nonTerminal, row] : parsingTable) {
        for (auto &[terminal, production] : row) {
            cout << setw(15) << left << nonTerminal
                 << setw(15) << left << terminal
                 << setw(30) << left << production << endl;
        }
    }
}

int main() {
    Grammar grammar;
    int numRules;

    cout << "Enter the number of grammar rules: ";
    cin >> numRules;
    cin.ignore();

    cout << "Enter the grammar rules (in the form A->BCD, where A is a non-terminal and B, C, D are symbols):\n";
    for (int i = 0; i < numRules; i++) {
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
