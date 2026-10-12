#pragma once
#include "poly.h"

class Frac {
private:
    Poly p, q;  // p/q
    
public:
    Frac();
    Frac(int x);
    Frac(const Term& term);
    Frac(const Poly& p_, const Poly& q_);
    
    Frac operator+(const Frac& other) const;
    Frac operator-(const Frac& other) const;
    Frac operator*(const Frac& other) const;
    Frac operator/(const Frac& other) const;
    
    Frac derivate() const;
    std::string output() const;
};