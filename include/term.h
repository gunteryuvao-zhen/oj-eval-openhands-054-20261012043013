#pragma once
#include <vector>
#include <string>

struct Term {
    int a, b, c, d;  // coefficients and exponents
    
    Term(int a = 0, int b = 0, int c = 0, int d = 0) : a(a), b(b), c(c), d(d) {}
    
    bool operator==(const Term& other) const {
        return b == other.b && c == other.c && d == other.d;
    }
    
    bool operator!=(const Term& other) const {
        return !(*this == other);
    }
};