#ifndef Test_H
#define Test_H

#include "BSModel.h"
#include "DataRow.h"
#include "utils.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <random>
#include <tuple>

using namespace std;

// Function to simulate stock price paths for Question 1
double Question1(double S0, double K, double T, double r, double mu, double sigma) {

    const int N = 100;   // Number of time steps
    double dt = T / N;  // Time step size

    // Initialize random number generator with a seed
    std::mt19937 generator(123); 
    std::normal_distribution<double> distribution(0.0, 1.0);

    // Generating stock price paths
    std::vector<std::vector<double>> paths(1000, std::vector<double>(N+1, S0));

    for (int i = 0; i < 1000; ++i) {
        for (int j = 1; j <= N; ++j) {
            double z = distribution(generator);
            paths[i][j] = paths[i][j-1] * (1 + mu*dt + sigma*sqrt(dt)*z);
        }
    }

    // Saving first 100 paths to a CSV file for inspection
    std::ofstream pathFile("Q1path.csv");
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j <= N; ++j) {
            pathFile << paths[i][j];
            if (j != N) pathFile << ",";
        }
        pathFile << "\n";
    }
    pathFile.close();

    std::vector<std::vector<double>> HEs(1000, std::vector<double>(N, 0.0));

    // Implementing delta hedging for each path
    for (int i = 0; i < 1000; ++i) {
        // Variables to hold data for delta hedging
        double B_prev = 0, B_current = 0, HE = 0, delta_prev = 0, delta_current = 0;
        double option_price = BlackScholesCall(paths[i][0], K, T, r, sigma);
        delta_prev = DeltaBS(paths[i][0], K, T, r, sigma);
        B_prev = option_price - delta_prev * paths[i][0];

        for (int j = 1; j < N; ++j) {
            // Calculate option price and deltas
            double option_price = BlackScholesCall(paths[i][j], K, T - j * dt, r, sigma);
            delta_current = DeltaBS(paths[i][j], K, T - j * dt, r, sigma);
            // Calculate B_current and Hedge Error (HE)
            B_current = delta_prev * paths[i][j] + B_prev * exp(r * dt) - delta_current * paths[i][j];
            double HE = delta_prev * paths[i][j] + B_prev * exp(r * dt) - option_price;
            // cout << delta_prev * paths[i][j] << endl;
            // cout << B_prev * exp(r * dt) << endl;
            // cout << option_price << endl;
            HEs[i][j] = HE;
            // Update deltas and Bs for next iteration
            delta_prev = delta_current;
            B_prev = B_current;
        }
    }

    // Saving the Hedge Errors to a CSV file
    std::ofstream HEFile("Q1HE.csv");
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j <= N-1; ++j) {
            HEFile << HEs[i][j];
            if (j != N) HEFile << ",";
        }
        HEFile << "\n";
    }
    HEFile.close();

    return 0.0;  // Placeholder return value
}

// Function to simulate stock and option prices and hedging for Question 2
double Question2(string t0, string tn, string T, double K) {
    // Load data from CSVs
    std::vector<std::string> dates;
    std::vector<double> rates;
    std::string date, tempStr;
    double rate;
    
    // Reading from interest.csv and parsing the rates and dates
    std::ifstream rateInFile("project1_data/data/interest.csv");
    std::string rateLine;

    const double tolerance = 1e-12;

    // Skip the first row (header)
    getline(rateInFile, rateLine);

    while (getline(rateInFile, rateLine)) {
        std::stringstream ss(rateLine);
        getline(ss, date, ',');
        getline(ss, tempStr);
        rate = stod(tempStr);
        rate = rate / 100.0;

        dates.push_back(date);
        rates.push_back(rate);
    }

    rateInFile.close();

    std::vector<double> close_adjs;
    std::string se_date;
    double close_adj;

    std::ifstream seInFile("project1_data/data/sec_GOOG.csv");
    std::string seLine;

    getline(seInFile, seLine);

    while (getline(seInFile, seLine)) {
        std::stringstream ss(seLine);
        getline(ss, se_date, ',');  // Read data until the comma
        getline(ss, tempStr);
        close_adj = stod(tempStr);

        close_adjs.push_back(close_adj);
    }

    seInFile.close();

    std::ifstream opInFile("project1_data/data/op_GOOG.csv");
    std::string opLine;

    std::vector<std::string> op_dates;
    std::vector<std::string> exdates;
    std::vector<char> cp_flag;
    std::vector<int> strike_pr;
    std::vector<double> best_bid;
    std::vector<double> best_offer;

    std::string line, tempDate, tempExdate;
    char tempFlag;
    double tempStrike, tempBid, tempOffer;

    getline(opInFile, opLine);  // Skip the header line

    while (getline(opInFile, line)) {
        std::stringstream ss(line);
        getline(ss, tempDate, ',');
        getline(ss, tempExdate, ',');
        getline(ss, tempStr, ',');
        tempFlag = tempStr[0];
        getline(ss, tempStr, ',');
        tempStrike = stod(tempStr); // Convert string to double
        getline(ss, tempStr, ',');
        tempBid = stod(tempStr); // Convert string to double
        getline(ss, tempStr);
        tempOffer = stod(tempStr); // Convert string to double

        op_dates.push_back(tempDate);
        exdates.push_back(tempExdate);
        cp_flag.push_back(tempFlag);
        strike_pr.push_back(tempStrike);
        best_bid.push_back(tempBid);
        best_offer.push_back(tempOffer);
    }

    opInFile.close();

    std::vector<DataRow> op_data = constructDataRows(op_dates, exdates, cp_flag, strike_pr);

    int i0 = binary_search(dates, t0);
    int in = binary_search(dates, tn);
    int it = binary_search(dates, T);

    std::vector<string> Ds;
    std::vector<double> Ss;
    std::vector<double> values;
    std::vector<double> IVs;
    std::vector<double> deltas;
    std::vector<double> HEs;
    std::vector<double> Bs;
    std::vector<double> PNLs;
    std::vector<double> PNLs_Hedge;

    double cum_HE;

    for (int i=i0; i<=in; i++) {

        string tempDate = dates[i];
        DataRow target(tempDate, T, 'C', K);

        int temp_index = binary_search_row(op_data, target);

        double interval = (it - i) / 252.0;
        double rate_i = rates[i];
        double close_i = close_adjs[i];
        double op_price_i = (best_bid[temp_index] + best_offer[temp_index]) / 2.0;
        double IV_i = implied_volatility_binary_search(op_price_i, close_i, K, interval, rate_i, tolerance);
        double delta_i = DeltaBS(close_i, K, interval, rate_i, IV_i);

        double B_i, HE_i, PNL_i;
        if (i == i0) {
            B_i = op_price_i - delta_i * close_i;
            HE_i = 0.0;
            PNL_i = 0.0;
            cum_HE = 0.0;
        } else {
            B_i = deltas.back() * close_i + Bs.back() * exp(rates.back() / 252) - delta_i * close_i;
            HE_i = deltas.back() * close_i + Bs.back() * exp(rates.back() / 252) - op_price_i;
            PNL_i = values.front() - op_price_i;
        }

        cum_HE += HE_i;

        Ds.push_back(tempDate);
        Ss.push_back(close_i);
        values.push_back(op_price_i);
        IVs.push_back(IV_i);
        deltas.push_back(delta_i);
        HEs.push_back(HE_i);
        Bs.push_back(B_i);
        PNLs.push_back(PNL_i);
        PNLs_Hedge.push_back(cum_HE);

    }

    // Write the results to CSV
    writeCSV(Ds, Ss, values, IVs, deltas, HEs, PNLs, PNLs_Hedge, "Q2result.csv");

    return 0;
}

void tests() {
    // Execute Question1 function as a test
    {
        std::cout << "Executing Question 1..." << std::endl;
        const double S0 = 100.0, K = 105.0, T = 0.4, r = 0.025, mu = 0.05, sigma = 0.24;
        double result = Question1(S0, K, T, r, mu, sigma);
        std::cout << "Question 1 Test Finished! " << std::endl;
    }

    // Execute Question2 function as a test
    {
        std::cout << "Executing Question 2..." << std::endl;
        double result = Question2("2011-07-05", "2011-07-29", "2011-09-17", 500.0); // You can change the arguments as needed
        std::cout << "Question 2 Test Finished!" << std::endl;
    }
}

void test_BSModel() {
    double S0 = 100.0;
    double K = 105.0;
    double r = 0.05;
    double sigma = 0.2;
    double T = 2.0;
    double V = BlackScholesCall(S0, K, T, r, sigma);
    {
        std::cout << "The Price of this Call Option by BS Model should be 13.63, get " << V << endl;
        std::cout << "The Delta of this Call Option by BS Model should be 0.6265, get " << DeltaBS(S0, K, T, r, sigma) << endl;
        std::cout << "The Implied Volatility of this Call Option by BS Model should be " << sigma << ", get " << implied_volatility_binary_search(V, S0, K, T, r, 1e-12) << endl;
    }

    S0 = 50.0;
    K = 55.0;
    r = 0.03;
    sigma = 0.1;
    T = 2.5;
    V = BlackScholesCall(S0, K, T, r, sigma);
    {
        std::cout << "The Price of this Call Option by BS Model should be 2.696, get " << V << endl;
        std::cout << "The Delta of this Call Option by BS Model should be 0.48, get " << DeltaBS(S0, K, T, r, sigma) << endl;
        std::cout << "The Implied Volatility of this Call Option by BS Model should be " << sigma << ", get " << implied_volatility_binary_search(V, S0, K, T, r, 1e-12) << endl;
    }
}

#endif
