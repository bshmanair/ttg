#include <iostream>
#include <string>
#include <sstream> // helps treat input like a source stream
#include <vector>
#include "parser.hpp"

int main()
{
    std::string input;
    std::cout << "Enter a boolean expression: ";
    std::cin >> input;
    std::cout << std::endl;

    std::vector<std::string> tokens = parse(input);
    for (const auto &token : tokens)
    {
        std::cout << "[" << token << "] ";
    }
    std::cout << std::endl;

    return 0;
}