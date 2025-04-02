#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <cctype>
using namespace std;

unordered_map<char, vector<string>> grammar;        
unordered_map<char, unordered_set<char>> firstSet;  
unordered_map<char, unordered_set<char>> followSet;
char startSymbol;

// Calculate First of a symbol
void calculateFirst(char symbol)
{
    if (firstSet.count(symbol))
        return;

    unordered_set<char> result;

    for (const string &production : grammar[symbol])
    {
        for (char ch : production)
        {
            if (isupper(ch))
            { // Non-terminal
                calculateFirst(ch);
                result.insert(firstSet[ch].begin(), firstSet[ch].end());
                if (!firstSet[ch].count('#'))
                    break;
            }
            else
            { // Terminal
                result.insert(ch);
                break;
            }
        }
    }

    firstSet[symbol] = result;
}

// Calculate Follow of a symbol
void calculateFollow(char symbol)
{
    if (followSet.count(symbol))
        return;

    unordered_set<char> result;
    if (symbol == startSymbol)
        result.insert('$');

    for (auto it = grammar.begin(); it != grammar.end(); ++it)
    {
        char lhs = it->first;
        const vector<string> &productions = it->second;

        for (const string &production : productions)
        {
            for (int i = 0; i < production.size(); ++i)
            {
                if (production[i] == symbol)
                {
                    int k = i + 1;
                    while (k < production.size())
                    {
                        char next = production[k];
                        if (isupper(next))
                        {
                            result.insert(firstSet[next].begin(), firstSet[next].end());
                            result.erase('#');
                            if (!firstSet[next].count('#'))
                                break;
                        }
                        else
                        {
                            result.insert(next);
                            break;
                        }
                        k++;
                    }

                    if (k == production.size() && lhs != symbol)
                    {
                        calculateFollow(lhs);
                        result.insert(followSet[lhs].begin(), followSet[lhs].end());
                    }
                }
            }
        }
    }

    followSet[symbol] = result;
}

int main()
{
    grammar['E'] = {"TR"};
    grammar['R'] = {"+TR", "#"};
    grammar['T'] = {"FY"};
    grammar['Y'] = {"*FY", "#"};
    grammar['F'] = {"(E)", "i"};
    startSymbol = 'E';

    // Calculate First sets
    for (auto it = grammar.begin(); it != grammar.end(); ++it)
        calculateFirst(it->first);

    cout << "First Sets:\n";
    for (auto it = firstSet.begin(); it != firstSet.end(); ++it)
    {
        cout << "First(" << it->first << ") = { ";
        for (char ch : it->second)
            cout << ch << " ";
        cout << "}\n";
    }

    // Calculate Follow sets
    for (auto it = grammar.begin(); it != grammar.end(); ++it)
        calculateFollow(it->first);

    cout << "\nFollow Sets:\n";
    for (auto it = followSet.begin(); it != followSet.end(); ++it)
    {
        cout << "Follow(" << it->first << ") = { ";
        for (char ch : it->second)
            cout << ch << " ";
        cout << "}\n";
    }

    return 0;
}
