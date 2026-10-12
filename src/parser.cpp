#include <string>
#include <vector>
#include <cctype>
#include <algorithm>
#include "../include/frac.h"

// Forward declarations
Frac parse_expression(const std::string& s, int& pos, int end);
Frac parse_term(const std::string& s, int& pos, int end);
Frac parse_factor(const std::string& s, int& pos, int end);
int get_number(const std::string& s, int& pos, int end);

int get_number(const std::string& s, int& pos, int end) {
    if (pos >= end) return 1;
    
    int sign = 1;
    if (s[pos] == '-') {
        sign = -1;
        pos++;
    } else if (s[pos] == '+') {
        pos++;
    }
    
    if (pos >= end || !std::isdigit(s[pos])) {
        return sign;  // No digits, return ±1
    }
    
    int num = 0;
    while (pos < end && std::isdigit(s[pos])) {
        num = num * 10 + (s[pos] - '0');
        pos++;
    }
    
    return sign * num;
}

Frac parse_factor(const std::string& s, int& pos, int end) {
    if (pos >= end) return Frac(0);
    
    // Handle parentheses
    if (s[pos] == '(') {
        pos++;  // skip '('
        Frac result = parse_expression(s, pos, end);
        if (pos < end && s[pos] == ')') {
            pos++;  // skip ')'
        }
        return result;
    }
    
    // Handle numbers and variables
    int coeff = 1;
    if (std::isdigit(s[pos]) || s[pos] == '+' || s[pos] == '-') {
        coeff = get_number(s, pos, end);
    }
    
    // Parse a complete term (which may include x, sin, cos components)
    int x_exp = 0, sin_exp = 0, cos_exp = 0;
    bool has_component = false;
    
    while (pos < end) {
        if (s[pos] == 'x') {
            pos++;  // consume 'x'
            x_exp = 1;
            if (pos < end && s[pos] == '^') {
                pos++;  // consume '^'
                x_exp = get_number(s, pos, end);
            }
            has_component = true;
        }
        else if (pos + 2 < end && s.substr(pos, 3) == "sin") {
            pos += 3;  // consume 'sin'
            sin_exp = 1;
            if (pos < end && s[pos] == '^') {
                pos++;  // consume '^'
                sin_exp = get_number(s, pos, end);
            }
            if (pos < end && s[pos] == 'x') {
                pos++;  // consume 'x'
            }
            has_component = true;
        }
        else if (pos + 2 < end && s.substr(pos, 3) == "cos") {
            pos += 3;  // consume 'cos'
            cos_exp = 1;
            if (pos < end && s[pos] == '^') {
                pos++;  // consume '^'
                cos_exp = get_number(s, pos, end);
            }
            if (pos < end && s[pos] == 'x') {
                pos++;  // consume 'x'
            }
            has_component = true;
        }
        else {
            break;  // No more components
        }
    }
    
    // If no component was found, it's just a constant
    if (!has_component) {
        return Frac(coeff);
    }
    
    return Frac(Term(coeff, x_exp, sin_exp, cos_exp));
}

Frac parse_term(const std::string& s, int& pos, int end) {
    Frac result = parse_factor(s, pos, end);
    
    // Handle multiplication and division
    while (pos < end && (s[pos] == '*' || s[pos] == '/')) {
        char op = s[pos];
        pos++;  // consume operator
        
        Frac rhs = parse_factor(s, pos, end);
        
        if (op == '*') {
            result = result * rhs;
        } else {  // op == '/'
            result = result / rhs;
        }
    }
    
    return result;
}

Frac parse_expression(const std::string& s, int& pos, int end) {
    Frac result = parse_term(s, pos, end);
    
    // Handle addition and subtraction
    while (pos < end && (s[pos] == '+' || s[pos] == '-')) {
        char op = s[pos];
        pos++;  // consume operator
        
        Frac rhs = parse_term(s, pos, end);
        
        if (op == '+') {
            result = result + rhs;
        } else {  // op == '-'
            result = result - rhs;
        }
    }
    
    return result;
}

Frac parse(const std::string& s) {
    int pos = 0;
    return parse_expression(s, pos, s.length());
}