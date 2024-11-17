#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <cmath>

struct Monom {
    double coefficient;
    int degree;

    Monom(double coeff = 0, int deg = 0) : coefficient(coeff), degree(deg) {}

    std::string ToString() const noexcept {
        if (coefficient == 0) return "";
        std::ostringstream os;
        if (degree == 0) {
            os << coefficient;
        }
        else if (degree == 1) {
            os << (std::abs(coefficient) == 1 ? (coefficient < 0 ? "-" : "") : std::to_string(coefficient)) << "x";
        }
        else {
            os << (std::abs(coefficient) == 1 ? (coefficient < 0 ? "-" : "") : std::to_string(coefficient))
                << "x^" << degree;
        }
        return os.str();
    }

    bool operator<(const Monom& other) const {
        return degree < other.degree;
    }

};

class Polynomial {
private:
    std::vector<Monom> monoms;
public:
    void transformation() {
        std::sort(monoms.begin(), monoms.end(), [](const Monom& a, const Monom& b) {return a.degree > b.degree;});

        std::vector<Monom> simplified;
        for (size_t i = 0; i < monoms.size(); ++i) {
            if (simplified.empty() || simplified.back().degree != monoms[i].degree) {
                simplified.push_back(monoms[i]);
            }
            else {
                double newCoeff = simplified.back().coefficient + monoms[i].coefficient;
                simplified.back().coefficient=newCoeff;
            }
        }
        monoms = simplified;
    }

    Polynomial(const std::vector<Monom>& mons) : monoms(mons) { transformation(); }

    Polynomial operator+(const Polynomial& other) const {
        std::vector<Monom> result = monoms;
        result.insert(result.end(), other.monoms.begin(), other.monoms.end());
        return Polynomial(result);
    }

    Polynomial operator-(const Polynomial& other) const {
        std::vector<Monom> result = monoms;
        for (const auto& monom : other.monoms) {
            result.push_back(Monom(-monom.coefficient, monom.degree));
        }
        return Polynomial(result);
    }

    Polynomial operator*(const Polynomial& other) const {
        std::vector<Monom> result;
        for (const auto& monom1 : monoms) {
            for (const auto& monom2 : other.monoms) {
                result.push_back(Monom(monom1.coefficient * monom2.coefficient,
                    monom1.degree + monom2.degree));
            }
        }
        return Polynomial(result);
    }

    std::string ToString() const noexcept {
        if (monoms.empty()) return "0";
        std::string result;
        for (const auto& monom : monoms) {
            std::string monomStr = monom.ToString();
            if (!monomStr.empty()) {
                if (!result.empty() && monom.coefficient > 0) {
                    result += " + ";
                }
                result += monomStr;
            }
        }
        return result.empty() ? "0" : result;
    }
};

#endif // _POLYNOMIAL_H
