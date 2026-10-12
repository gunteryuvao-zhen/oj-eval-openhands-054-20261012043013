#include "../include/frac.h"
#include <sstream>
#include <iomanip>

Frac::Frac() {
    p = Poly();
    q = Poly();
}

Frac::Frac(int x) {
    if (x == 0) {
        p = Poly();
    } else {
        p = Poly({Term(x, 0, 0, 0)});
    }
    q = Poly({Term(1, 0, 0, 0)});
}

Frac::Frac(const Term& term) {
    p = Poly({term});
    q = Poly({Term(1, 0, 0, 0)});
}

Frac::Frac(const Poly& p_, const Poly& q_) : p(p_), q(q_) {}

Frac Frac::operator+(const Frac& other) const {
    // p1/q1 + p2/q2 = (p1*q2 + p2*q1) / (q1*q2)
    Poly new_p = p * other.q + other.p * q;
    Poly new_q = q * other.q;
    return Frac(new_p, new_q);
}

Frac Frac::operator-(const Frac& other) const {
    // p1/q1 - p2/q2 = (p1*q2 - p2*q1) / (q1*q2)
    Poly new_p = p * other.q - other.p * q;
    Poly new_q = q * other.q;
    return Frac(new_p, new_q);
}

Frac Frac::operator*(const Frac& other) const {
    // p1/q1 * p2/q2 = (p1*p2) / (q1*q2)
    Poly new_p = p * other.p;
    Poly new_q = q * other.q;
    return Frac(new_p, new_q);
}

Frac Frac::operator/(const Frac& other) const {
    // p1/q1 / p2/q2 = (p1*q2) / (q1*p2)
    Poly new_p = p * other.q;
    Poly new_q = q * other.p;
    return Frac(new_p, new_q);
}

Frac Frac::derivate() const {
    // (p/q)' = (p' * q - q' * p) / (q * q)
    Poly p_der = p.derivate();
    Poly q_der = q.derivate();
    Poly new_p = p_der * q - q_der * p;
    Poly new_q = q * q;
    return Frac(new_p, new_q);
}

std::string Frac::output() const {
    std::ostringstream oss;
    
    // Handle special case: numerator is 0
    if (p.size() == 0) {
        return "0";
    }
    
    // Output numerator
    std::string p_str = "";
    for (size_t i = 0; i < p.size(); ++i) {
        const Term& term = p[i];
        
        // Handle sign
        if (i == 0) {
            // First term: no + sign for positive
            if (term.a < 0) {
                p_str += "-";
            }
        } else {
            // Subsequent terms: explicit + or -
            if (term.a > 0) {
                p_str += "+";
            } else {
                p_str += "-";
            }
        }
        
        // Handle coefficient
        int abs_a = std::abs(term.a);
        if (i == 0 && abs_a == 1 && (term.b > 0 || term.c > 0 || term.d > 0)) {
            // First term with coefficient ±1 (and not a constant): omit 1
        } else if (abs_a != 1 || (term.b == 0 && term.c == 0 && term.d == 0)) {
            // Not ±1, or it's a constant term: include coefficient
            p_str += std::to_string(abs_a);
        }
        // If coefficient is 1 and it's not a constant, we don't output anything
        
        // Handle x^b
        if (term.b > 0) {
            p_str += "x";
            if (term.b > 1) {
                p_str += "^" + std::to_string(term.b);
            }
        }
        
            // Handle sin^c x
        if (term.c > 0) {
            p_str += "sin";
            if (term.c > 1) {
                p_str += "^" + std::to_string(term.c);
            }
            p_str += "x";  // Always add x after sin function
        }
        
        // Handle cos^d x
        if (term.d > 0) {
            p_str += "cos";
            if (term.d > 1) {
                p_str += "^" + std::to_string(term.d);
            }
            p_str += "x";  // Always add x after cos function
        }
        
        // Special case: constant term
        if (term.b == 0 && term.c == 0 && term.d == 0) {
            if (i == 0) {
                p_str += std::to_string(term.a);
            } else {
                if (term.a < 0) {
                    p_str += std::to_string(-term.a);
                } else {
                    p_str += std::to_string(term.a);
                }
            }
        }
    }
    
    // Handle denominator
    if (q.size() == 1 && q[0].a == 1 && q[0].b == 0 && q[0].c == 0 && q[0].d == 0) {
        // Denominator is 1
        return p_str;
    }
    
    // Denominator has more than one term or is not 1
    std::string q_str = "";
    for (size_t i = 0; i < q.size(); ++i) {
        const Term& term = q[i];
        
        // Handle sign
        if (i == 0) {
            // First term: no + sign for positive
            if (term.a < 0) {
                q_str += "-";
            }
        } else {
            // Subsequent terms: explicit + or -
            if (term.a > 0) {
                q_str += "+";
            } else {
                q_str += "-";
            }
        }
        
        // Handle coefficient
        int abs_a = std::abs(term.a);
        if (i == 0 && abs_a == 1 && (term.b > 0 || term.c > 0 || term.d > 0)) {
            // First term with coefficient ±1 (and not a constant): omit 1
        } else if (abs_a != 1 || (term.b == 0 && term.c == 0 && term.d == 0)) {
            // Not ±1, or it's a constant term: include coefficient
            q_str += std::to_string(abs_a);
        }
        // If coefficient is 1 and it's not a constant, we don't output anything
        
        // Handle x^b
        if (term.b > 0) {
            q_str += "x";
            if (term.b > 1) {
                q_str += "^" + std::to_string(term.b);
            }
        }
        
        // Handle sin^c x
        if (term.c > 0) {
            q_str += "sin";
            if (term.c > 1) {
                q_str += "^" + std::to_string(term.c);
            }
            q_str += "x";  // Always add x after sin function
        }
        
        // Handle cos^d x
        if (term.d > 0) {
            q_str += "cos";
            if (term.d > 1) {
                q_str += "^" + std::to_string(term.d);
            }
            q_str += "x";  // Always add x after cos function
        }
        
        // Special case: constant term
        if (term.b == 0 && term.c == 0 && term.d == 0) {
            if (i == 0) {
                q_str += std::to_string(term.a);
            } else {
                if (term.a < 0) {
                    q_str += std::to_string(-term.a);
                } else {
                    q_str += std::to_string(term.a);
                }
            }
        }
    }
    
    // Format as (p)/(q) if needed
    if (p.size() > 1) {
        if (q.size() > 1) {
            return "(" + p_str + ")/(" + q_str + ")";
        } else {
            return "(" + p_str + ")/" + q_str;
        }
    } else {
        if (q.size() > 1) {
            return p_str + "/(" + q_str + ")";
        } else {
            return p_str + "/" + q_str;
        }
    }
}