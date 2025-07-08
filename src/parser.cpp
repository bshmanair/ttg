#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> parse(const std::string &input)
{
    std::vector<std::string> res;
    std::istringstream iss(input);
    std::string token;
    while (std::getline(iss, token, ' '))
    {
        if (!token.empty()) // in case of extra spaces
        {
            res.push_back(token);
        }
    }
    return res;
}