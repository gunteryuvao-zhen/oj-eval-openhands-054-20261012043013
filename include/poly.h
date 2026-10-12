#pragma once
#include <vector>
#include "term.h"

class Poly {
private:
    std::vector<Term> terms;
    
public:
    Poly() = default;
    Poly(const std::vector<Term>& terms_);
    Poly(int size);
    
    size_t size() const { return terms.size(); }
    const Term& operator[](size_t i) const { return terms[i]; }
    Term& operator[](size_t i) { return terms[i]; }
    
    void simplify();
    Poly operator+(const Poly& other) const;
    Poly operator-(const Poly& other) const;
    Poly operator*(const Poly& other) const;
    Poly derivate() const;
    
    void add_term(const Term& term);
    void clear() { terms.clear(); }
};