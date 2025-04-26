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

// Calculate Follow of a symbol
void calculateFollow(char symbol)
{
    if (followSet.count(symbol))
        return;

    unordered_set<char> result;
    if (symbol == startSymbol)
        result.insert('$'); // End marker

    for (auto &rule : grammar)
    {
        char lhs = rule.first;
        const vector<string> &productions = rule.second;

        for (const string &production : productions)
        {
            for (size_t i = 0; i < production.size(); ++i)
            {
                if (production[i] == symbol)
                {
                    size_t k = i + 1;
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

// Construct Parsing Table
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

    for (auto &rule : grammar)
    {
        for (auto &production : rule.second)
        {
            for (char ch : production)
            {
                if (!isupper(ch) && ch != '#')
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

// Simulate Parsing
void parseString(const string &input)
{
    cout << "\nParsing Input: " << input << endl;
    string augmentedInput = input + "$";
    vector<char> stack;
    stack.push_back('$');
    stack.push_back(startSymbol);

    int i = 0;
    while (!stack.empty())
    {
        char top = stack.back();
        char current = augmentedInput[i];

        cout << "Stack: ";
        for (char c : stack)
            cout << c;
        cout << " | Input: " << augmentedInput.substr(i) << " | Action: ";

        if (top == current)
        {
            cout << "Match '" << top << "'" << endl;
            stack.pop_back();
            i++;
        }
        else if (!isupper(top))
        {
            cout << "Error (Expected '" << top << "', got '" << current << "')" << endl;
            return;
        }
        else if (parsingTable[top].count(current))
        {
            string production = parsingTable[top][current];
            cout << top << " -> " << production << endl;
            stack.pop_back();
            if (production != "#")
            {
                for (int j = production.size() - 1; j >= 0; j--)
                    stack.push_back(production[j]);
            }
        }
        else
        {
            cout << "Error (No rule for [" << top << ", " << current << "])" << endl;
            return;
        }
    }

    cout << "\nString accepted by the grammar.\n";
}

// Main Function
int main()
{
    grammar['E'] = {"TR"};
    grammar['R'] = {"+TR", "#"};
    grammar['T'] = {"FY"};
    grammar['Y'] = {"*FY", "#"};
    grammar['F'] = {"(E)", "i"};
    startSymbol = 'E';

    // Compute First sets
    for (auto &rule : grammar)
        calculateFirst(rule.first);

    cout << "First Sets:\n";
    for (auto &entry : firstSet)
    {
        cout << "First(" << entry.first << ") = { ";
        for (char ch : entry.second)
            cout << ch << " ";
        cout << "}\n";
    }

    // Compute Follow sets
    for (auto &rule : grammar)
        calculateFollow(rule.first);

    cout << "\nFollow Sets:\n";
    for (auto &entry : followSet)
    {
        cout << "Follow(" << entry.first << ") = { ";
        for (char ch : entry.second)
            cout << ch << " ";
        cout << "}\n";
    }

    // Construct Parsing Table
    constructParsingTable();
    printParsingTable();
    parseString("i+i*i");

    return 0;
}
