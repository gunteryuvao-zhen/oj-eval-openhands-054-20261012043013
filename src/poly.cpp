#include "../include/poly.h"
#include <algorithm>
#include <numeric>

Poly::Poly(const std::vector<Term>& terms_) : terms(terms_) {}

Poly::Poly(int size) {
    terms.reserve(size);
}

void Poly::add_term(const Term& term) {
    if (term.a != 0) {
        terms.push_back(term);
    }
}

void Poly::simplify() {
    if (terms.empty()) return;
    
    // Sort by exponents (b, c, d) in descending order
    std::sort(terms.begin(), terms.end(), [](const Term& t1, const Term& t2) {
        if (t1.b != t2.b) return t1.b > t2.b;
        if (t1.c != t2.c) return t1.c > t2.c;
        return t1.d > t2.d;
    });
    
    // Merge like terms
    std::vector<Term> result;
    result.push_back(terms[0]);
    
    for (size_t i = 1; i < terms.size(); ++i) {
        if (terms[i] == result.back()) {
            result.back().a += terms[i].a;
        } else {
            if (result.back().a != 0) {
                result.push_back(terms[i]);
            }
        }
    }
    
    // Handle last term
    if (!result.empty() && result.back().a == 0 && result.size() > 1) {
        result.pop_back();
    }
    
    terms = result;
}

Poly Poly::operator+(const Poly& other) const {
    Poly result;
    result.terms.reserve(terms.size() + other.terms.size());
    
    // Copy terms from both polynomials
    result.terms.insert(result.terms.end(), terms.begin(), terms.end());
    result.terms.insert(result.terms.end(), other.terms.begin(), other.terms.end());
    
    result.simplify();
    return result;
}

Poly Poly::operator-(const Poly& other) const {
    Poly result;
    result.terms.reserve(terms.size() + other.terms.size());
    
    // Copy terms from first polynomial
    result.terms.insert(result.terms.end(), terms.begin(), terms.end());
    
    // Copy terms from second polynomial with negated coefficients
    for (const auto& term : other.terms) {
        result.add_term(Term(-term.a, term.b, term.c, term.d));
    }
    
    result.simplify();
    return result;
}

Poly Poly::operator*(const Poly& other) const {
    Poly result;
    result.terms.reserve(terms.size() * other.terms.size());
    
    // Multiply each term from first polynomial with each term from second
    for (const auto& t1 : terms) {
        for (const auto& t2 : other.terms) {
            Term product(
                t1.a * t2.a,  // coefficient
                t1.b + t2.b,  // x exponent
                t1.c + t2.c,  // sin exponent
                t1.d + t2.d   // cos exponent
            );
            result.add_term(product);
        }
    }
    
    result.simplify();
    return result;
}

Poly Poly::derivate() const {
    Poly result;
    result.terms.reserve(3 * terms.size());  // At most 3 terms per term
    
    for (const auto& term : terms) {
        if (term.a == 0) continue;
        
        // Derivative of a*x^b*sin^c(x)*cos^d(x)
        // Using product rule: (uvw)' = u'vw + uv'w + uvw'
        
        // Term 1: derivative of x^b
        if (term.b > 0) {
            result.add_term(Term(
                term.a * term.b,      // coefficient
                term.b - 1,           // x exponent
                term.c,               // sin exponent
                term.d                // cos exponent
            ));
        }
        
        // Term 2: derivative of sin^c(x)
        if (term.c > 0) {
            result.add_term(Term(
                term.a * term.c,      // coefficient
                term.b,               // x exponent
                term.c - 1,           // sin exponent
                term.d + 1            // cos exponent (derivative of sin is cos)
            ));
        }
        
        // Term 3: derivative of cos^d(x)
        if (term.d > 0) {
            result.add_term(Term(
                -term.a * term.d,     // coefficient (negative because derivative of cos is -sin)
                term.b,               // x exponent
                term.c + 1,           // sin exponent (derivative of cos is -sin)
                term.d - 1            // cos exponent
            ));
        }
    }
    
    result.simplify();
    return result;
}