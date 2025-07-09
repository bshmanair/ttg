#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <sstream>
#include <set>
#include <cmath>
#include <map>
#include <stack>
#include <algorithm>

std::vector<std::string> tokenize(const std::string &input)
{
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string word;

    while (ss >> word)
    {
        std::string token;
        for (char c : word)
        {
            if (c == '(' || c == ')')
            {
                if (!token.empty())
                {
                    tokens.push_back(token);
                    token.clear();
                }
                tokens.push_back(std::string(1, c));
            }
            else
            {
                token += c;
            }
        }
        if (!token.empty())
        {
            tokens.push_back(token);
        }
    }
    return tokens;
}

int precedence(const std::string &op)
{
    if (op == "NOT")
        return 3;
    if (op == "AND" || op == "NAND")
        return 2;
    if (op == "OR" || op == "NOR" || op == "XOR")
        return 1;
    return 0;
}

bool isRightAssociative(const std::string &op)
{
    return op == "NOT";
}

bool isOperator(const std::string &token)
{
    return token == "AND" || token == "OR" || token == "NOT" ||
           token == "XOR" || token == "NAND" || token == "NOR";
}

std::vector<std::string> toPostfix(const std::vector<std::string> &tokens)
{
    std::vector<std::string> output;
    std::vector<std::string> ops;

    for (const std::string &token : tokens)
    {
        if (token == "(")
        {
            ops.push_back(token);
        }
        else if (token == ")")
        {
            while (!ops.empty() && ops.back() != "(")
            {
                output.push_back(ops.back());
                ops.pop_back();
            }
            if (!ops.empty())
                ops.pop_back();
        }
        else if (isOperator(token))
        {
            while (!ops.empty() && isOperator(ops.back()) &&
                   ((precedence(ops.back()) > precedence(token)) ||
                    (precedence(ops.back()) == precedence(token) && !isRightAssociative(token))))
            {
                output.push_back(ops.back());
                ops.pop_back();
            }
            ops.push_back(token);
        }
        else
        {
            output.push_back(token);
        }
    }

    while (!ops.empty())
    {
        output.push_back(ops.back());
        ops.pop_back();
    }

    return output;
}

bool evalPostfix(const std::vector<std::string> &postfix)
{
    std::stack<bool> stk;

    for (const std::string &tok : postfix)
    {
        if (tok == "0" || tok == "1")
        {
            stk.push(tok == "1");
        }
        else if (tok == "NOT")
        {
            if (stk.empty())
                throw std::runtime_error("Invalid expression");
            bool a = stk.top();
            stk.pop();
            stk.push(!a);
        }
        else
        {
            if (stk.size() < 2)
                throw std::runtime_error("Invalid expression");
            bool b = stk.top();
            stk.pop();
            bool a = stk.top();
            stk.pop();

            if (tok == "AND")
                stk.push(a && b);
            else if (tok == "OR")
                stk.push(a || b);
            else if (tok == "XOR")
                stk.push(a ^ b);
            else if (tok == "NAND")
                stk.push(!(a && b));
            else if (tok == "NOR")
                stk.push(!(a || b));
            else
                throw std::runtime_error("Unknown operator: " + tok);
        }
    }

    if (stk.size() != 1)
        throw std::runtime_error("Malformed expression");
    return stk.top();
}

int main()
{
    std::string input;

    std::cout << "Enter a boolean expression: ";
    std::getline(std::cin, input);

    for (char &c : input)
    {
        if (std::isalpha(c))
        {
            c = std::toupper(c);
        }
    }

    std::vector<std::string> tokens = tokenize(input);

    std::set<char> variables;
    for (const std::string &tok : tokens)
    {
        if (tok.length() == 1 && std::isalpha(tok[0]))
        {
            variables.insert(tok[0]);
        }
    }

    std::vector<char> varList(variables.begin(), variables.end());
    std::sort(varList.begin(), varList.end());

    int numVars = varList.size();
    int numRows = 1 << numVars;

    std::cout << "\nTruth Table:\n";
    for (char var : varList)
        std::cout << var << " ";
    std::cout << "| Result\n";
    for (int i = 0; i < numVars * 2 + 9; ++i)
        std::cout << "-";
    std::cout << "\n";

    for (int i = 0; i < numRows; ++i)
    {
        std::map<char, bool> row;
        for (int j = 0; j < numVars; ++j)
        {
            bool val = (i >> (numVars - j - 1)) & 1;
            row[varList[j]] = val;
            std::cout << val << " ";
        }
        std::cout << "| ";

        std::vector<std::string> replacedTokens;
        for (const std::string &tok : tokens)
        {
            if (tok.length() == 1 && std::isalpha(tok[0]))
            {
                replacedTokens.push_back(row[tok[0]] ? "1" : "0");
            }
            else
            {
                replacedTokens.push_back(tok);
            }
        }

        try
        {
            std::vector<std::string> postfix = toPostfix(replacedTokens);
            bool result = evalPostfix(postfix);
            std::cout << result << "\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
