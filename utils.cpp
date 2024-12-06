#include "utils.h"
#include <sstream>

// Разделение строки на токены
std::vector<std::string> parse_input(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream stream(input);
    std::string token;

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}
