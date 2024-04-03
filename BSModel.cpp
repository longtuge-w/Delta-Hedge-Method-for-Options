#include "BSModel.h"
#include <cmath>

double norm_cdf(double x) {
    return 0.5 * std::erfc(-x * sqrt(0.5));
}

double norm_pdf(const double x) {
    return (1.0 / sqrt(2.0 * PI)) * std::exp(-0.5 * x * x);
}

double BlackScholesCall(double S, double K, double T, double r, double sigma) {
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    double d2 = d1 - sigma * sqrt(T);
    return S * 0.5 * (1.0 + erf(d1 / sqrt(2.0))) - K * exp(-r * T) * 0.5 * (1.0 + erf(d2 / sqrt(2.0)));
}

double DeltaBS(double S, double K, double T, double r, double sigma) {
    double d1 = (log(S / K) + (r + 0.5 * sigma * sigma) * T) / (sigma * sqrt(T));
    return norm_cdf(d1);
}

double vega(const double S, const double K, const double r, const double v, const double T) {
    double d1 = (std::log(S / K) + (r + 0.5 * v * v) * T) / (v * sqrt(T));
    return S * sqrt(T) * norm_pdf(d1);
}

double implied_volatility_binary_search(double marketPrice, double S, double K, double T, double r, double tolerance) {
    double lower_bound = 1e-3;
    double upper_bound = 5.0;
    double sigma;

    while (upper_bound - lower_bound > tolerance) {
        sigma = (upper_bound + lower_bound) / 2.0;
        double price = BlackScholesCall(S, K, T, r, sigma);

        if (price < marketPrice) {
            lower_bound = sigma;  // Price too low, increase sigma
        } else {
            upper_bound = sigma;  // Price too high, decrease sigma
        }
    }

    return sigma;  // Return the midpoint of final bracket
}
