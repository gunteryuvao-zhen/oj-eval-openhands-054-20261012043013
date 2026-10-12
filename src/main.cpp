#include <iostream>
#include <string>
#include "../include/frac.h"
#include "parser.cpp"  // Include parser implementation

int main() {
    std::string expr;
    std::getline(std::cin, expr);
    
    // Parse the expression into a fraction
    Frac f = parse(expr);
    
    // Output the original expression (simplified)
    std::cout << f.output() << std::endl;
    
    // Output the derivative
    Frac df = f.derivate();
    std::cout << df.output() << std::endl;
    
    return 0;
}