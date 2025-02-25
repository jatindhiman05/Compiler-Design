#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace std;

unordered_set<char> operators = {'+', '-', '*', '/', '<', '>', '=', '&', '|', ',', '(', ')', '[', ']', '{', '}', '#'};
unordered_set<string> keywords = {"int", "return", "using", "namespace", "std", "include", "iostream", "main"};

unordered_set<char> operators_;
unordered_set<string> reservedWords;
unordered_set<string> vars_nums;

bool isNumber(string word)
{
    for (int i = 0; i < word.size(); i++)
    {
        if (!(word[i] >= '0' && word[i] <= '9'))
        {
            return false;
        }
    }
    return true;
}

bool isVariable(string word)
{
    if ((word[0] >= '0' && word[0] <= '9'))
    {
        return false;
    }

    for (char &ch : word)
    {
        if (!((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9') || ch == '_'))
        {
            return false;
        }
    }

    return true;
}
void print()
{
    cout << "Reserved Keywords:  ";
    for (auto &kw : reservedWords)
    {
        cout << kw << "  ";
    }

    cout << endl;
    cout << "\nVariables and Numbers:  ";
    for (auto &var : vars_nums)
    {
        cout << var << "  ";
    }

    cout << endl;
    cout << "\nOperators:  ";
    for (auto op : operators_)
    {
        cout << op << "  ";
    }
}
int main()
{
    fstream inFile("MyFile.txt", ios::in);

    string currLine;
    while (getline(inFile, currLine))
    {
        int bp = 0, fp = 0;
        while (fp <= currLine.size())
        {
            if (fp < currLine.size() && operators.find(currLine[fp]) == operators.end() && !(currLine[fp] == ' ' || currLine[fp] == '\t' || currLine[fp] == '\n' || currLine[fp] == ';'))
            {
                fp++;
            }
            else
            {
                string token = currLine.substr(bp, fp - bp);

                if (keywords.find(token) != keywords.end())
                {
                    reservedWords.insert(token);
                }
                else if (isNumber(token) || isVariable(token))
                {
                    vars_nums.insert(token);
                }

                if (operators.find(currLine[fp]) != operators.end())
                {
                    operators_.insert(currLine[fp]);
                }

                fp++;
                bp = fp;
            }
        }
    }

    print();
    return 0;
}
