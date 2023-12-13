#ifndef CALCULATOR_H
#define CALCULATOR_H

class Calculator {
public:
    Calculator() = default;

    double add(double a, double b);
    double subtract(double a, double b);
    double multiply(double a, double b);
    double divide(double a, double b);
    double power(double a, double b);
    double modulus(double a, double b);
};

#endif // CALCULATOR_H
