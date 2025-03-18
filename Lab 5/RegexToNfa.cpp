#include <iostream>
#include <stack>
#include <vector>
#include <set>
using namespace std;

struct State
{
    int id;
    State *out1 = nullptr; // first transition
    State *out2 = nullptr; // second transition (for | and *)
    char symbol;
    State(int id, char symbol = 0){
        this->id = id;
        this->symbol = symbol;
    }
};

struct NFA
{
    State *start;
    State *accept;
};

class ThompsonNFA
{
private:
    int stateCount = 0;
    stack<NFA> nfaStack;

    State *createState(char symbol = 0)
    {
        return new State(stateCount++, symbol);
    }

public:
    NFA regexToNFA(string regex)
    {
        for (char c : regex)
        {
            if (c == '*')
            {
                NFA nfa = nfaStack.top();
                nfaStack.pop();
                State *start = createState();
                State *accept = createState();

                start->out1 = nfa.start;
                start->out2 = accept;
                nfa.accept->out1 = nfa.start;
                nfa.accept->out2 = accept;

                nfaStack.push({start, accept});
            }
            else if (c == '.')
            {
                NFA nfa2 = nfaStack.top();
                nfaStack.pop();
                NFA nfa1 = nfaStack.top();
                nfaStack.pop();

                nfa1.accept->out1 = nfa2.start;

                nfaStack.push({nfa1.start, nfa2.accept});
            }
            else if (c == '|')
            {
                NFA nfa2 = nfaStack.top();
                nfaStack.pop();
                NFA nfa1 = nfaStack.top();
                nfaStack.pop();

                State *start = createState();
                State *accept = createState();

                start->out1 = nfa1.start;
                start->out2 = nfa2.start;
                nfa1.accept->out1 = accept;
                nfa2.accept->out1 = accept;

                nfaStack.push({start, accept});
            }
            else
            {
                State *start = createState();
                State *accept = createState();
                start->out1 = accept;
                start->symbol = c;

                nfaStack.push({start, accept});
            }
        }
        return nfaStack.top();
    }

    void printNFA(State *state, set<int> &visited)
    {
        if (!state || visited.count(state->id))
            return;
        visited.insert(state->id);

        cout << "State " << state->id;
        if (state->symbol)
        {
            cout << " ('" << state->symbol << "')";
        }
        else
        {
            cout << " (eps)";
        }
        cout << endl;

        if (state->out1)
        {
            cout << "  -> " << state->out1->id;
            if (!state->out1->symbol)
                cout << " (eps)";
            cout << endl;
        }
        if (state->out2)
        {
            cout << "  -> " << state->out2->id << " (eps)" << endl;
        }

        printNFA(state->out1, visited);
        printNFA(state->out2, visited);
    }
};

int main()
{
    string regex = "ab|*";
    ThompsonNFA thompson;
    NFA nfa = thompson.regexToNFA(regex);
    set<int> visited;
    thompson.printNFA(nfa.start, visited);
    return 0;
}
