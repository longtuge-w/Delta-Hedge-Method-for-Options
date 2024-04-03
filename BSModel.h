#ifndef BSModel_H
#define BSMdoel_H

// Constant value for PI
const double PI = 3.14159265358979323846;

// Function to compute the cumulative distribution function of a standard normal distribution
double norm_cdf(double x);

// Function to compute the probability density function of a standard normal distribution
double norm_pdf(const double x);

// Function to calculate the Black-Scholes option price for a European call option
double BlackScholesCall(double S, double K, double T, double r, double sigma);

// Function to calculate the Delta of an option using the Black-Scholes model
double DeltaBS(double S, double K, double T, double r, double sigma);

// Function to calculate the Vega of an option, representing sensitivity to volatility
double vega(const double S, const double K, const double r, const double v, const double T);

// Function to compute the implied volatility using a binary search method
double implied_volatility_binary_search(double marketPrice, double S, double K, double T, double r, double tolerance);

#endif  // BSMODEL_H
