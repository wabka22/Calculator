#ifndef _POLYNOMIAL_H
#define _POLYNOMIAL_H

#include <iostream>
#include <map>
#include <variant>
#include <string>
#include <sstream>

class Polynomial {
private:
    std::map<int, std::variant<long long, double>> terms;

    void removeZeroCoefficients() {
        for (auto it = terms.begin(); it != terms.end();) {
            if (std::visit([](auto&& arg) { return arg == 0; }, it->second)) {
                it = terms.erase(it);
            }
            else {
                ++it;
            }
        }
    }

public:
    Polynomial() = default;

    Polynomial(int degree, const std::variant<long long, double>& coeff) {
        if (std::visit([](auto&& arg) { return arg != 0; }, coeff)) {
            terms[degree] = coeff;
        }
    }

    Polynomial(const std::map<int, std::variant<long long, double>>& _terms) {
        for (const auto& [degree, coeff] : _terms) {
            if (std::visit([](auto&& arg) { return arg != 0; }, coeff)) {
                terms[degree] = coeff;
            }
        }
        removeZeroCoefficients();
    }

    ~Polynomial() = default;

    Polynomial& operator=(const Polynomial& p) {
        if (this != &p) {
            terms = p.terms;
        }
        return *this;
    }

    Polynomial operator+(const Polynomial& p) const {
        Polynomial result(*this);
        for (const auto& term : p.terms) {
            result.terms[term.first] = std::visit([&](auto&& arg) { return arg + std::get<double>(term.second); },
                result.terms[term.first]);
        }
        result.removeZeroCoefficients();
        return result;
    }

    Polynomial operator+(double value) const {
        Polynomial result(*this);
        result.terms[0] = std::visit([&](auto&& arg) { return arg + value; }, result.terms[0]);
        result.removeZeroCoefficients();
        return result;
    }

    Polynomial operator+(long long value) const {
        return *this + value;
    }

    friend Polynomial operator+(long long value, const Polynomial& p) {
        return p + value;
    }

    friend Polynomial operator+(double value, const Polynomial& p) {
        return p + value;
    }

    Polynomial operator-(double value) const {
        return *this + (-value);
    }

    friend Polynomial operator-(double value, const Polynomial& p) {
        Polynomial result;
        result.terms[0] = value;
        result = result - p;
        return result;
    }

    Polynomial operator-(const Polynomial& p) const {
        Polynomial result(*this);
        for (const auto& term : p.terms) {
            result.terms[term.first] = std::visit([&](auto&& arg) { return arg - std::get<double>(term.second); },
                result.terms[term.first]);
        }
        result.removeZeroCoefficients();
        return result;
    }

    Polynomial operator-(long long value) const {
        return *this + (-value);
    }

    friend Polynomial operator-(long long value, const Polynomial& p) {
        Polynomial result;
        result.terms[0] = value;
        result = result - p;
        return result;
    }

    Polynomial operator*(const Polynomial& p) const {
        Polynomial result;
        for (const auto& term1 : terms) {
            for (const auto& term2 : p.terms) {
                result.terms[term1.first + term2.first] = std::visit(
                    [&](auto&& arg1) { return arg1 * std::get<double>(term2.second); }, term1.second);
            }
        }
        result.removeZeroCoefficients();
        return result;
    }

    Polynomial operator*(double value) const {
        Polynomial result(*this);
        for (auto& term : result.terms) {
            term.second = std::visit([&](auto&& arg) { return arg * value; }, term.second);
        }
        result.removeZeroCoefficients();
        return result;
    }

    friend Polynomial operator*(double value, const Polynomial& p) {
        return p * value;
    }

    Polynomial operator*(long long value) const {
        return *this * value;
    }

    friend Polynomial operator*(long long value, const Polynomial& p) {
        return p * value;
    }

    Polynomial operator/(double value) const {
        if (value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        Polynomial result(*this);
        for (auto& term : result.terms) {
            term.second = std::visit([&](auto&& arg) { return arg / value; }, term.second);
        }
        result.removeZeroCoefficients();
        return result;
    }

    Polynomial operator/(long long value) const {
        if (value == 0) {
            throw std::invalid_argument("Division by zero.");
        }
        return *this / value;
    }

    friend Polynomial operator/(double value, const Polynomial& p) {
        throw std::invalid_argument("Polynomial division is not supported.");
    }

    friend Polynomial operator/(long long value, const Polynomial& p) {
        throw std::invalid_argument("Polynomial division is not supported.");
    }

    Polynomial operator/(const Polynomial& p) const {
        throw std::invalid_argument("Polynomial division is not supported.");
    }

    std::string ToString() const {
        std::ostringstream oss;
        bool first = true;
        for (const auto& term : terms) {
            if (std::visit([](auto&& arg) { return arg != 0; }, term.second)) {
                if (!first) {
                    oss << (std::get<double>(term.second) > 0 ? " + " : " - ");
                }
                if (std::abs(std::get<double>(term.second)) != 1 || term.first == 0) {
                    oss << std::abs(std::get<double>(term.second));
                }
                if (term.first > 0) {
                    oss << "x";
                    if (term.first > 1) {
                        oss << "^" << term.first;
                    }
                }
                first = false;
            }
        }
        return oss.str().empty() ? "0" : oss.str();
    }

    static Polynomial Parse(const std::string& str) {
        Polynomial result;
        size_t pos = 0;
        bool isNegative = false;

        while (pos < str.size()) {
            if (str[pos] == '-') {
                isNegative = true;
                ++pos;
            }
            else if (str[pos] == '+') {
                isNegative = false;
                ++pos;
            }

            double coefficient = 1.0;
            size_t start = pos;
            while (pos < str.size() && (isdigit(str[pos]) || str[pos] == '.' || str[pos] == '-')) {
                ++pos;
            }
            if (pos > start) {
                coefficient = std::stod(str.substr(start, pos - start));
            }
            else if (pos < str.size() && (str[pos] == 'x' || str[pos] == 'X')) {
                coefficient = isNegative ? -1.0 : 1.0;
            }

            int degree = 0;
            if (pos < str.size() && (str[pos] == 'x' || str[pos] == 'X')) {
                ++pos;
                if (pos < str.size() && str[pos] == '^') {
                    ++pos;
                    start = pos;
                    while (pos < str.size() && isdigit(str[pos])) {
                        ++pos;
                    }
                    if (pos > start) {
                        degree = std::stoi(str.substr(start, pos - start));
                    }
                }
                else {
                    degree = 1;
                }
            }
            result.terms[degree] = std::visit(
                [&](auto&& arg) { return arg + (isNegative ? -coefficient : coefficient); },
                result.terms[degree]);
        }

        result.removeZeroCoefficients();
        return result;
    }

    friend std::ostream& operator<<(std::ostream& os, const Polynomial& p) {
        os << p.ToString();
        return os;
    }
};

#endif // _POLYNOMIAL_H