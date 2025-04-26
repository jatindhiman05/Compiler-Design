#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <functional>
#include <stack>

using namespace std;

class Production
{
public:
    string lhs;
    vector<string> rhs;
    function<void()> semanticRule;

    Production() {}

    Production(const string &lhs, const vector<string> &rhs, function<void()> semanticRule)
        : lhs(lhs), rhs(rhs), semanticRule(semanticRule) {}
};

stack<pair<string,int>> s;


map<int, Production> grammar;

void storeGrammar(map<int, Production> &grammar)
{
    grammar.insert({1, Production("L", {"E", "n"}, []()
                                  { cout <<"hello world"; })});

    grammar.insert({2, Production("E", {"E", "1", "+", "T"}, []()
                                  {})});

    grammar.insert({3, Production("E", {"T"}, []()
                                  {
                                  })});

    grammar.insert({4, Production("T", {"T", "1", "*", "F"}, []()
                                  { })});

    grammar.insert({5, Production("T", {"F"}, []()
                                  {
                                  })});

    grammar.insert({6, Production("F", {"(", "E", ")"}, []()
                                  {
                                  })});

    grammar.insert({7, Production("F", {"digit"}, []()
                                  { })});
}

int main()
{
    storeGrammar(grammar);

    grammar[1].semanticRule();

    string inputString = "2*3+5";

    int i = 0;
    // first -> parser stack
    // second -> second stack
    stack<pair<int,int>> st;

    while(i < inputString.length()){

    }
    return 0;
}
