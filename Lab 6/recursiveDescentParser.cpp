#include <iostream>
#include <vector>
#include <cctype>
#include <string>

using namespace std;

// Token types
enum TokenType
{
    IDENTIFIER,
    NUMBER,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    OPEN_PAREN,
    CLOSE_PAREN,
    END
};

// Token structure
struct Token
{
    TokenType type;
    string value;
};

// Global token list and index
vector<Token> tokens;
int currentIndex = 0;

// Function to match and consume a token
bool match(TokenType expected)
{
    if (currentIndex < tokens.size() && tokens[currentIndex].type == expected)
    {
        currentIndex++;
        return true;
    }
    return false;
}

// Forward declarations for recursive functions
bool E();
bool EPrime();
bool T();
bool TPrime();
bool F();

// <E> → <T> <E'>
bool E()
{
    if (!T())
        return false;
    return EPrime();
}

// <E'> → + <T> <E'> | - <T> <E'> | ε
bool EPrime()
{
    if (match(PLUS) || match(MINUS))
    {
        if (!T())
        {
            cout << " Missing operand after operator at index " << currentIndex << endl;
            return false;
        }
        return EPrime();
    }
    return true; // Epsilon (empty) transition
}

// <T> → <F> <T'>
bool T()
{
    if (!F())
        return false;
    return TPrime();
}

// <T'> → * <F> <T'> | / <F> <T'> | ε
bool TPrime()
{
    if (match(MULTIPLY) || match(DIVIDE))
    {
        if (!F())
        {
            cout << "Syntax Error: Missing operand after operator at index " << currentIndex << endl;
            return false;
        }
        return TPrime();
    }
    return true; // Epsilon (empty) transition
}

// <F> → ( <E> ) | number | identifier
bool F()
{
    if (match(OPEN_PAREN))
    {
        if (!E())
        {
            cout << "Syntax Error: Mismatched parentheses at index " << currentIndex << endl;
            return false;
        }
        if (!match(CLOSE_PAREN))
        {
            cout << "Syntax Error: Missing closing parenthesis at index " << currentIndex << endl;
            return false;
        }
        return true;
    }
    if (match(NUMBER) || match(IDENTIFIER))
    {
        return true;
    }
    cout << "Syntax Error: Unexpected token at index " << currentIndex << endl;
    return false;
}

// Function to tokenize input string
vector<Token> tokenize(const string &input)
{
    vector<Token> tokens;
    int i = 0;
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
        else if (isdigit(input[i]))
        {
            tokens.push_back({NUMBER, string(1, input[i])});
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
    tokens.push_back({END, ""});
    return tokens;
}

// Main function to parse input
bool parse(const string &input)
{
    tokens = tokenize(input);
    currentIndex = 0;
    if (!E())
        return false;
    if (tokens[currentIndex].type != END)
    {
        cout << "Syntax Error: Unexpected token at end of input" << endl;
        return false;
    }
    return true;
}

// Driver code
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
