#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <iomanip>
using namespace std;

unordered_map<char, vector<string>> grammar;                   
unordered_map<char, unordered_set<char>> firstSet;             
unordered_map<char, unordered_set<char>> followSet;           
unordered_map<char, unordered_map<char, string>> parsingTable;
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

void constructParsingTable()
{
    for (auto &rule : grammar)
    {
        char nonTerminal = rule.first;

        for (const string &production : rule.second)
        {
            unordered_set<char> firstAlpha;

            for (char ch : production)
            {
                if (isupper(ch))
                {                               
                    firstAlpha = firstSet[ch]; 
                    if (!firstAlpha.count('#'))
                        break;
                }
                else
                {
                    firstAlpha.insert(ch);
                    break;
                }
            }

            for (char terminal : firstAlpha)
            {
                if (terminal != '#')
                {
                    parsingTable[nonTerminal][terminal] = production;
                }
            }

            if (firstAlpha.count('#'))
            {
                for (char terminal : followSet[nonTerminal])
                {
                    parsingTable[nonTerminal][terminal] = production;
                }
            }
        }
    }
}

// Print the Parsing Table
void printParsingTable()
{
    cout << "\nLL(1) Parsing Table:\n";
    cout << setw(10) << " ";
    unordered_set<char> terminals;

    // Gather all terminals for table headers
    for (auto &rule : grammar)
    {
        for (auto &production : rule.second)
        {
            for (char ch : production)
            {
                if (!isupper(ch) && ch != '#') // Terminals
                    terminals.insert(ch);
            }
        }
    }
    terminals.insert('$'); // Add the end-of-input marker

    for (char terminal : terminals)
        cout << setw(10) << terminal;
    cout << endl;

    for (auto &row : parsingTable)
    {
        cout << setw(10) << row.first;
        for (char terminal : terminals)
        {
            if (parsingTable[row.first].count(terminal))
                cout << setw(10) << parsingTable[row.first][terminal];
            else
                cout << setw(10) << " ";
        }
        cout << endl;
    }
}

int main()
{
    grammar['E'] = {"TR"};
    grammar['R'] = {"+TR", "#"};
    grammar['T'] = {"FY"};
    grammar['Y'] = {"*FY", "#"};
    grammar['F'] = {"(E)", "i"};
    startSymbol = 'E';

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

    constructParsingTable();
    printParsingTable();

    return 0;
}
