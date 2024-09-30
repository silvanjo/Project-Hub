#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// Function declarations
double expression(char **str);
double term(char **str);
double factor(char **str);
void skip_whitespace(char **str);

int main() {
    char input[256];

    printf("Enter a mathematical expression: ");
    if (fgets(input, sizeof(input), stdin) != NULL) {
        // Remove newline character if present
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
            input[len - 1] = '\0';

        char *p = input;
        double result = expression(&p);

        // Check for any remaining invalid characters
        skip_whitespace(&p);
        if (*p != '\0') {
            printf("Error: Invalid syntax.\n");
            return 1;
        }

        printf("Result: %lf\n", result);
    } else {
        printf("Error reading input.\n");
        return 1;
    }

    return 0;
}

double expression(char **str) {
    double result = term(str);

    while (1) {
        skip_whitespace(str);
        if (**str == '+') {
            (*str)++;
            result += term(str);
        } else if (**str == '-') {
            (*str)++;
            result -= term(str);
        } else {
            break;
        }
    }

    return result;
}

double term(char **str) {
    double result = factor(str);

    while (1) {
        skip_whitespace(str);
        if (**str == '*') {
            (*str)++;
            result *= factor(str);
        } else if (**str == '/') {
            (*str)++;
            double divisor = factor(str);
            if (divisor == 0) {
                printf("Error: Division by zero.\n");
                exit(1);
            }
            result /= divisor;
        } else {
            break;
        }
    }

    return result;
}

double factor(char **str) {
    skip_whitespace(str);

    double result = 0.0;

    if (**str == '(') {
        (*str)++;  // Skip '('
        result = expression(str);
        skip_whitespace(str);
        if (**str == ')') {
            (*str)++;  // Skip ')'
        } else {
            printf("Error: Mismatched parentheses.\n");
            exit(1);
        }
    } else if (isdigit(**str) || **str == '.') {
        char *start = *str;
        result = strtod(*str, str);
        if (*str == start) {
            printf("Error: Invalid number.\n");
            exit(1);
        }
    } else if (**str == '+' || **str == '-') {
        // Handle unary plus and minus
        char op = **str;
        (*str)++;
        result = factor(str);
        if (op == '-')
            result = -result;
    } else {
        printf("Error: Unexpected character '%c'.\n", **str);
        exit(1);
    }

    return result;
}

void skip_whitespace(char **str) {
    while (isspace(**str)) {
        (*str)++;
    }
}

