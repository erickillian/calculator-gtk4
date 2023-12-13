#include <vector>
#include <string>


class ExpressionParser {
public:
    double parseAndCalculate(const std::string& expression);

private:
    std::vector<std::string> tokenize(const std::string& expression);
    std::vector<std::string> toRPN(const std::vector<std::string>& tokens);
    double evaluateRPN(const std::vector<std::string>& rpn);
    int getPrecedence(const std::string& op);
    bool isOperator(const std::string& token);
};

