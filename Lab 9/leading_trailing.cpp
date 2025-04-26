#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <stack>
using namespace std;

map<pair<char, char>, char> table;

vector<string> productions;
map<char, set<char>> trailing;

map<char, set<char>> leading;

bool isTerminal(char c)
{
    return (c >= 'a' && c <= 'z') || c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '9';
}

void leading_fun(char nonter, set<char> &lead1, set<char> &visited)
{
    if (visited.count(nonter))
        return;
    visited.insert(nonter);

    for (const string &prod : productions)
    {
        if (prod[0] == nonter)
        {
            bool terminal_found = false;

            for (int i = 3; i < prod.length(); i++)
            {
                char symbol = prod[i];
                if (isTerminal(symbol))
                {
                    lead1.insert(symbol);
                    terminal_found = true;
                    break;
                }
            }

            if (!terminal_found && prod.length() > 3)
            {
                char firstSymbol = prod[3];
                if (firstSymbol >= 'A' && firstSymbol <= 'Z')
                {
                    if (leading.find(firstSymbol) == leading.end())
                    {
                        set<char> temp;
                        leading_fun(firstSymbol, temp, visited);
                        leading[firstSymbol] = temp;
                    }
                    lead1.insert(leading[firstSymbol].begin(), leading[firstSymbol].end());
                }
            }
        }
    }
}

void trailing_fun(char nonter, set<char> &trail1, set<char> &visited)
{
    if (visited.count(nonter))
        return;
    visited.insert(nonter);

    for (const string &prod : productions)
    {
        if (prod[0] == nonter)
        {
            bool terminal_found = false;

            for (int i = prod.length(); i >= 3; i--)
            {
                char symbol = prod[i];
                if (isTerminal(symbol))
                {
                    trail1.insert(symbol);
                    terminal_found = true;
                    break;
                }
            }

            if (!terminal_found && prod.length() > 3)
            {
                char firstSymbol = prod[3];
                if (firstSymbol >= 'A' && firstSymbol <= 'Z')
                {
                    if (trailing.find(firstSymbol) == trailing.end())
                    {
                        set<char> temp;
                        trailing_fun(firstSymbol, temp, visited);
                        trailing[firstSymbol] = temp;
                    }
                    trail1.insert(trailing[firstSymbol].begin(), trailing[firstSymbol].end());
                }
            }
        }
    }
}

bool isOperator(char symbol)
{
    return (symbol == '+' || symbol == '*' || symbol == '(' || symbol == ')' || symbol == '9');
}

void create_operator_precedence_table()
{
    for (const string &prod : productions)
    {
        string rhs = prod.substr(3);

        for (size_t i = 0; i < rhs.size(); ++i)
        {
            char curr = rhs[i];

            if (isOperator(curr) && i + 1 < rhs.size() && isupper(rhs[i + 1]))
            {
                char nonterm = rhs[i + 1];
                for (char lead : leading[nonterm])
                {
                    if (isOperator(lead) || lead == '9' || lead == '(')
                    {
                        table[{curr, lead}] = '<';
                    }
                }
            }

            if (isupper(curr) && i + 1 < rhs.size() && isOperator(rhs[i + 1]))
            {
                char next = rhs[i + 1];
                for (char trail : trailing[curr])
                {
                    if (isOperator(trail) || trail == '9' || trail == ')')
                    {
                        table[{trail, next}] = '>';
                    }
                }
            }

            if (i + 2 < rhs.size() && isOperator(rhs[i]) &&
                isupper(rhs[i + 1]) && isOperator(rhs[i + 2]))
            {
                table[{rhs[i], rhs[i + 2]}] = '=';
            }

            if (i + 1 < rhs.size() && isOperator(curr) && isOperator(rhs[i + 1]))
            {
                table[{curr, rhs[i + 1]}] = '>';
            }
        }
    }

    char startSymbol = productions[0][0];
    for (char lead : leading[startSymbol])
    {
        if (isOperator(lead) || lead == '9' || lead == '(')
        {
            table[{'$', lead}] = '<';
        }
    }

    for (char trail : trailing[startSymbol])
    {
        if (isOperator(trail) || trail == '9' || trail == ')')
        {
            table[{trail, '$'}] = '>';
        }
    }
}

void print_precedence_table()
{
    for (const auto &entry : table)
    {
        cout << "table[{'" << entry.first.first << "', '" << entry.first.second << "'}] = '" << entry.second << "';" << endl;
    }
}

void operator_parser(string input)
{
    stack<char> s;
    s.push('$');
    int i = 0;
    while (i < input.length())
    {
        char move = table[{s.top(), input[i]}];

        if (move == '<' || move == '=')
        {
            cout << "shift move!" << endl;
            s.push(input[i]);
            i++;
        }
        if (move == '>')
        {
            cout << "reduce move!" << endl;
            while (table[{s.top(), input[i]}] == '>')
            {
                s.pop();
            }
        }
        if (s.top() == '$' && input[i] == '$')
        {
            cout << "string is valid" << endl;
            return;
        }
    }
}

int main()
{

    productions = {"E->E+T", "E->T", "T->T*F", "T->F", "F->(E)", "F->9"};
    int n = productions.size();

    for (int i = 0; i < n; i++)
    {
        char non_term = productions[i][0];

        if (leading.find(non_term) == leading.end())
        {
            set<char> lead_1;
            set<char> visited;
            leading_fun(non_term, lead_1, visited);
            leading[non_term] = lead_1;
        }
    }

    for (const auto &i : leading)
    {
        cout << "Leading(" << i.first << ") - { ";
        for (char ch : i.second)
        {
            cout << ch << " ";
        }
        cout << "}" << endl;
    }

    for (int i = 0; i < n; i++)
    {
        char non_term = productions[i][0];

        if (trailing.find(non_term) == trailing.end())
        {
            set<char> trail_1;
            set<char> visited1;
            trailing_fun(non_term, trail_1, visited1);
            trailing[non_term] = trail_1;
        }
    }

    for (const auto &i : trailing)
    {
        cout << "trailing(" << i.first << ") - { ";
        for (char ch : i.second)
        {
            cout << ch << " ";
        }
        cout << "}" << endl;
    }

    create_operator_precedence_table();
    print_precedence_table();
    string input = "9+9*9$";
    operator_parser(input);

    return 0;
}