#include <iostream>
#include <vector>
#include <cctype>
#include <string>

using namespace std;

enum TokenType
{
    IDENTIFIER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    OPEN_PAREN,
    CLOSE_PAREN
};

struct Token
{
    TokenType type;
    string value;
};

vector<Token> tokens;
Token *next_tok;

bool match(TokenType expected)
{
    if (next_tok < &tokens[tokens.size()] && next_tok->type == expected)
    {
        next_tok++;
        return true;
    }
    return false;
}

bool E();
bool Eprime();
bool T();
bool Tprime();
bool F();

// <E> → <T> <E'>
bool E()
{
    return T() && Eprime();
}

// <E'> → + <T> <E'> | - <T> <E'> | ε
bool Eprime()
{
    Token *save = next_tok;
    if (match(PLUS) && T() && Eprime())
    {
        return true;
    }
    next_tok = save;
    if (match(MINUS) && T() && Eprime())
    {
        return true;
    }
    next_tok = save;
    return true; // ε (empty string)
}

// <T> → <F> <T'>
bool T()
{
    return F() && Tprime();
}

// <T'> → * <F> <T'> | / <F> <T'> | ε
bool Tprime()
{
    Token *save = next_tok;
    if (match(MULTIPLY) && F() && Tprime())
    {
        return true;
    }
    next_tok = save;
    if (match(DIVIDE) && F() && Tprime())
    {
        return true;
    }
    next_tok = save;
    return true; // ε (empty string)
}

// <F> → (<E>) | identifier
bool F()
{
    Token *save = next_tok;
    if (match(OPEN_PAREN) && E() && match(CLOSE_PAREN))
    {
        return true;
    }
    next_tok = save;
    if (match(IDENTIFIER))
    {
        return true;
    }
    return false;
}

vector<Token> tokenize(const string &input)
{
    vector<Token> tokens;
    size_t i = 0;
    while (i < input.size())
    {
        if (isspace(input[i]))
        {
            i++;
            continue;
        }
        if (isalpha(input[i]))
        {
            tokens.push_back({IDENTIFIER, string(1, input[i])});
        }
        else if (input[i] == '+')
        {
            tokens.push_back({PLUS, "+"});
        }
        else if (input[i] == '-')
        {
            tokens.push_back({MINUS, "-"});
        }
        else if (input[i] == '*')
        {
            tokens.push_back({MULTIPLY, "*"});
        }
        else if (input[i] == '/')
        {
            tokens.push_back({DIVIDE, "/"});
        }
        else if (input[i] == '(')
        {
            tokens.push_back({OPEN_PAREN, "("});
        }
        else if (input[i] == ')')
        {
            tokens.push_back({CLOSE_PAREN, ")"});
        }
        else
        {
            cout << "Error: Unknown character '" << input[i] << "' at position " << i << endl;
            exit(1);
        }
        i++;
    }
    return tokens;
}

bool parse(const string &input)
{
    tokens = tokenize(input);
    next_tok = &tokens[0];
    return (E() && next_tok == &tokens[tokens.size()]);
}

int main()
{
    string input = "x + (y * z) / w - u";
    cout << "Parsing: " << input << endl;
    if (parse(input))
    {
        cout << "Valid Expression!" << endl;
    }
    else
    {
        cout << "Invalid Expression!" << endl;
    }
    return 0;
}
