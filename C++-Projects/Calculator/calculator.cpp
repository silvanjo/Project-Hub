#include <iostream>
#include <string>
#include <cctype>
#include <stdexcept>
#include <sstream>
#include <cmath>

class Calculator {
public:
    Calculator(const std::string& expression) : expr(expression), pos(0) {}

    double parse() {
        double result = parseExpression();
        skipWhitespace();
        if (pos != expr.length()) {
            throw std::runtime_error("Unexpected character: '" + std::string(1, expr[pos]) + "'");
        }
        return result;
    }

private:
    std::string expr;
    size_t pos;

    double parseExpression() {
        double result = parseTerm();
        while (true) {
            skipWhitespace();
            if (match('+')) {
                result += parseTerm();
            } else if (match('-')) {
                result -= parseTerm();
            } else {
                break;
            }
        }
        return result;
    }

    double parseTerm() {
        double result = parseFactor();
        while (true) {
            skipWhitespace();
            if (match('*')) {
                result *= parseFactor();
            } else if (match('/')) {
                double divisor = parseFactor();
                if (divisor == 0) {
                    throw std::runtime_error("Division by zero");
                }
                result /= divisor;
            } else {
                break;
            }
        }
        return result;
    }

    double parseFactor() {
        double result = parseExponent();
        return result;
    }

    double parseExponent() {
        double base = parseUnary();
        skipWhitespace();
        if (match('^')) {
            double exponent = parseExponent(); // Right-associative
            return std::pow(base, exponent);
        }
        return base;
    }

    double parseUnary() {
        skipWhitespace();
        if (match('+')) {
            return parseUnary();
        } else if (match('-')) {
            return -parseUnary();
        } else {
            return parsePrimary();
        }
    }

    double parsePrimary() {
        skipWhitespace();
        if (match('(')) {
            double result = parseExpression();
            skipWhitespace();
            if (!match(')')) {
                throw std::runtime_error("Missing closing parenthesis");
            }
            return result;
        } else if (std::isdigit(currentChar()) || currentChar() == '.') {
            return parseNumber();
        } else {
            throw std::runtime_error("Unexpected character: '" + std::string(1, currentChar()) + "'");
        }
    }

    double parseNumber() {
        skipWhitespace();
        size_t startPos = pos;
        while (std::isdigit(currentChar()) || currentChar() == '.') {
            advance();
        }
        std::string numberStr = expr.substr(startPos, pos - startPos);
        std::istringstream iss(numberStr);
        double value;
        iss >> value;
        if (iss.fail()) {
            throw std::runtime_error("Invalid number: " + numberStr);
        }
        return value;
    }

    char currentChar() const {
        if (pos < expr.length()) {
            return expr[pos];
        } else {
            return '\0';
        }
    }

    void advance() {
        if (pos < expr.length()) {
            pos++;
        }
    }

    bool match(char expected) {
        if (currentChar() == expected) {
            advance();
            return true;
        }
        return false;
    }

    void skipWhitespace() {
        while (std::isspace(currentChar())) {
            advance();
        }
    }
};

int main() {
    std::string input;

    std::cout << "Enter a mathematical expression: ";
    if (std::getline(std::cin, input)) {
        try {
            Calculator calc(input);
            double result = calc.parse();
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
            return 1;
        }
    } else {
        std::cerr << "Error reading input." << std::endl;
        return 1;
    }

    return 0;
}

