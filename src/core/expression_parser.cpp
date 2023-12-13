#include "expression_parser.h"
#include "calculator.h"
#include <stack>
#include <sstream>
#include <cctype>
#include <iostream>

double ExpressionParser::parseAndCalculate(const std::string& expression) {
    auto tokens = tokenize(expression);
    auto rpn = toRPN(tokens);
    return evaluateRPN(rpn);
}

// Tokenize the expression
std::vector<std::string> ExpressionParser::tokenize(const std::string& expression) {
    std::vector<std::string> tokens;
    std::string number;

    for (char ch : expression) {
        if (std::isdigit(ch) || ch == '.') {
            // Accumulate digits and decimal point for a number
            number += ch;
        } else {
            if (!number.empty()) {
                // Add the accumulated number to the tokens
                tokens.push_back(number);
                number.clear();
            }
            if (!std::isspace(ch)) {
                // Add the operator or parenthesis as a separate token
                tokens.push_back(std::string(1, ch));
            }
        }
    }

    // Add the last number if there is one
    if (!number.empty()) {
        tokens.push_back(number);
    }

    return tokens;
}

// Convert to Reverse Polish Notation using the Shunting Yard algorithm
std::vector<std::string> ExpressionParser::toRPN(const std::vector<std::string>& tokens) {
    std::vector<std::string> outputQueue;
    std::stack<std::string> operatorStack;

    for (const auto& token : tokens) {
        if (isdigit(token[0]) || (token.size() > 1 && isdigit(token[1]))) {
            // Number
            outputQueue.push_back(token);
        } else if (isOperator(token)) {
            // Operator
            while (!operatorStack.empty() && isOperator(operatorStack.top()) && 
                   getPrecedence(operatorStack.top()) >= getPrecedence(token)) {
                outputQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }
            operatorStack.push(token);
        } else if (token == "(") {
            // Left parenthesis
            operatorStack.push(token);
        } else if (token == ")") {
            // Right parenthesis
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                outputQueue.push_back(operatorStack.top());
                operatorStack.pop();
            }
            if (!operatorStack.empty()) {
                operatorStack.pop(); // Pop the left parenthesis
            }
        }
    }

    // Pop any remaining operators from the stack to the output queue
    while (!operatorStack.empty()) {
        outputQueue.push_back(operatorStack.top());
        operatorStack.pop();
    }

    return outputQueue;
}

// Evaluate RPN expression
double ExpressionParser::evaluateRPN(const std::vector<std::string>& rpn) {
    std::stack<double> stack;
    Calculator calc;

    for (const auto& token : rpn) {
        if (isdigit(token[0]) || (token[0] == '.' && token.size() > 1)) {
            // Push numbers onto the stack
            stack.push(std::stod(token));
        } else if (isOperator(token)) {
            // Operator: pop two values from the stack, apply the operator, and push the result back
            if (stack.size() < 2) {
                throw std::runtime_error("Invalid RPN expression: not enough operands");
            }

            double right = stack.top(); stack.pop();
            double left = stack.top(); stack.pop();
            double result = 0;

            if (token == "+") {
                result = calc.add(left, right);
            } else if (token == "-") {
                result = calc.subtract(left, right);
            } else if (token == "*") {
                result = calc.multiply(left, right);
            } else if (token == "/") {
                result = calc.divide(left, right);
            } else if (token == "^") {
                result = calc.power(left, right);
            } else if (token == "%") {
                result = calc.modulus(left, right);
            }

            stack.push(result);
        }
    }

    if (stack.size() != 1) {
        throw std::runtime_error("Invalid RPN expression: improper number of operands");
    }

    return stack.top();
}


// Helper functions to determine precedence and if a token is an operator
int ExpressionParser::getPrecedence(const std::string& op) {
    if (op == "^") return 3;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

bool ExpressionParser::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^" || token == "%";
}
