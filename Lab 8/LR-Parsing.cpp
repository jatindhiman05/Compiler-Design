#include <iostream>
#include <unordered_map>
#include <string>
#include <stack>
#include <vector>

using namespace std;

void storeActions(unordered_map<int, unordered_map<string, string>> &parsingTable);
void storeProductions(unordered_map<int, pair<string, vector<string>>> &productions);

int main()
{
    unordered_map<int, unordered_map<string, string>> parsingTable;
    unordered_map<int, pair<string, vector<string>>> productions;
    stack<string> st;

    st.push("0");

    storeActions(parsingTable);
    storeProductions(productions);

    vector<string> stringTokens = {"id", "*", "id", "+", "id", "$"};
    int idx = 0;

    while (idx < stringTokens.size())
    {
        int state = stoi(st.top());
        string token = stringTokens[idx];

        string what = parsingTable[state][token];

        if (what == "")
        {
            cout << "Error"<< endl;
            break;
        }

        if (what[0] == 's')
        {
            //shift
            string nextState = what.substr(1);
            st.push(token);
            st.push(nextState);
            idx++; 
        }
        else if (what[0] == 'r')
        {
            // reduce
            int num = stoi(what.substr(1));
            pair<string, vector<string>> prod = productions[num];
            string lhs = prod.first;
            vector<string> rhs = prod.second;

            for (int i = 0; i < rhs.size() * 2; i++)
                st.pop();

            int newState = stoi(st.top());

            st.push(lhs);
            string goto_ = parsingTable[newState][lhs];

            st.push(goto_);
        }
        else if (what == "Acc")
        {
            cout << "Accepted String\n";
            break;
        }
    }

    return 0;
}

void storeProductions(unordered_map<int, pair<string, vector<string>>> &productions)
{
    productions[1] = {"E", {"E", "+", "T"}};
    productions[2] = {"E", {"T"}};
    productions[3] = {"T", {"T", "*", "F"}};
    productions[4] = {"T", {"F"}};
    productions[5] = {"F", {"(", "E", ")"}};
    productions[6] = {"F", {"id"}};
}

void storeActions(unordered_map<int, unordered_map<string, string>> &parsingTable)
{
    parsingTable[0]["id"] = "s5";
    parsingTable[0]["("] = "s4";
    parsingTable[0]["E"] = "1";
    parsingTable[0]["T"] = "2";
    parsingTable[0]["F"] = "3";

    parsingTable[1]["+"] = "s6";
    parsingTable[1]["$"] = "Acc";

    parsingTable[2]["+"] = "r2";
    parsingTable[2]["*"] = "s7";
    parsingTable[2][")"] = "r2";
    parsingTable[2]["$"] = "r2";

    parsingTable[3]["+"] = "r4";
    parsingTable[3]["*"] = "r4";
    parsingTable[3][")"] = "r4";
    parsingTable[3]["$"] = "r4";

    parsingTable[4]["id"] = "s5";
    parsingTable[4]["("] = "s4";
    parsingTable[4]["E"] = "8";
    parsingTable[4]["T"] = "2";
    parsingTable[4]["F"] = "3";

    parsingTable[5]["+"] = "r6";
    parsingTable[5]["*"] = "r6";
    parsingTable[5][")"] = "r6";
    parsingTable[5]["$"] = "r6";

    parsingTable[6]["id"] = "s5";
    parsingTable[6]["("] = "s4";
    parsingTable[6]["T"] = "9";
    parsingTable[6]["F"] = "3";

    parsingTable[7]["id"] = "s5";
    parsingTable[7]["("] = "s4";
    parsingTable[7]["F"] = "10";

    parsingTable[8]["+"] = "s6";
    parsingTable[8][")"] = "s11";

    parsingTable[9]["+"] = "r1";
    parsingTable[9]["*"] = "s7";
    parsingTable[9][")"] = "r1";
    parsingTable[9]["$"] = "r1";

    parsingTable[10]["+"] = "r3";
    parsingTable[10]["*"] = "r3";
    parsingTable[10][")"] = "r3";
    parsingTable[10]["$"] = "r3";

    parsingTable[11]["+"] = "r5";
    parsingTable[11]["*"] = "r5";
    parsingTable[11][")"] = "r5";
    parsingTable[11]["$"] = "r5";
}
