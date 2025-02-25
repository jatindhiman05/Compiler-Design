#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <unordered_map>
#include <iomanip>

using namespace std;

regex keyword_regex(R"(\b(auto|break|case|char|const|continue|default|do|double|else|enum|extern|"
                    "float|for|goto|if|inline|int|long|namespace|operator|private|protected|public|"
                    "register|return|short|signed|sizeof|static|struct|switch|template|this|throw|"
                    "try|typedef|union|unsigned|using|virtual|void|volatile|while)\b)");

regex identifier_regex(R"([_a-zA-Z][_a-zA-Z0-9]*)");
regex constant_regex(R"(\d+)");
regex operator_regex(R"([+\*/%=<>!&|^\-]+|==|!=|<=|>=|<<|>>)");
regex relational_operator_regex(R"(\b(==|!=|<=|>=|<|>)\b)");
regex delimiter_regex(R"([()\{\}\[\];,.:\?=])");
regex string_literal_regex(R"("([^"\\]|\\.)*")");

unordered_map<string, string> symbol_table_type;
unordered_map<string, int> symbol_table_occurrence;
unordered_map<string, int> symbol_table_length;

void lexical_tokenize(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Could not open file " << filename << endl;
        return;
    }

    string code((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    cout << "Source Code: " << endl;
    cout << code << endl;

    regex token_regex(R"(\S+)");

    sregex_iterator string_begin(code.begin(), code.end(), string_literal_regex);
    sregex_iterator string_end;

    for (sregex_iterator it = string_begin; it != string_end; ++it)
    {
        string token = it->str();
        symbol_table_type[token] = "string_literal";
        symbol_table_occurrence[token]++;
        symbol_table_length[token] = token.length();
    }

    sregex_iterator words_begin(code.begin(), code.end(), token_regex);
    sregex_iterator words_end;

    for (sregex_iterator it = words_begin; it != words_end; ++it)
    {
        string token = it->str();

        if (regex_match(token, string_literal_regex))
            continue;

        if (regex_match(token, keyword_regex))
        {
            symbol_table_type[token] = "keyword";
        }
        else if (regex_match(token, identifier_regex))
        {
            symbol_table_type[token] = "identifier";
        }
        else if (regex_match(token, constant_regex))
        {
            symbol_table_type[token] = "constant";
        }
        else if (regex_match(token, relational_operator_regex))
        {
            symbol_table_type[token] = "operator";
        }
        else if (regex_match(token, operator_regex))
        {
            symbol_table_type[token] = "operator";
        }
        else if (regex_match(token, delimiter_regex))
        {
            symbol_table_type[token] = "delimiter";
        }

        symbol_table_occurrence[token]++;
        symbol_table_length[token] = token.length();
    }
}

void print_symbol_table()
{
    cout << left << setw(20) << "Token"
         << setw(20) << "Type"
         << setw(20) << "Length"
         << setw(15) << "Occurrence" << endl;

    for (const auto &pair : symbol_table_type)
    {
        cout << left << setw(20) << pair.first
             << setw(20) << pair.second
             << setw(20) << symbol_table_length[pair.first]
             << setw(20) << symbol_table_occurrence[pair.first] << endl;
    }
}

int main()
{
    string code_file = "hello.cpp";

    lexical_tokenize(code_file);

    cout << "\nSymbol Table:\n";
    print_symbol_table();

    return 0;
}