#include "calculator.h"
#include <cmath>

double Calculator::add(double a, double b) {
    return a + b;
}

double Calculator::subtract(double a, double b) {
    return a - b;
}

double Calculator::multiply(double a, double b) {
    return a * b;
}

double Calculator::divide(double a, double b) {
    // Handle division by zero
    if (b == 0) {
        throw "Division by zero error";
    }
    return a / b;
}

// Power function
double Calculator::power(double a, double b) {
    return pow(a, b);
}

// Modulus function
double Calculator::modulus(double a, double b) {
    // Handle division by zero in modulus
    if (b == 0) {
        throw "Modulus by zero error";
    }
    return fmod(a, b);
}
