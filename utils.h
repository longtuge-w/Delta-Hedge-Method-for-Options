#ifndef Utils_H
#define Utils_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

// Function to perform binary search on a vector of strings
int binary_search(const std::vector<std::string>& vec, std::string target);

// Function to write various vectors into a CSV file
void writeCSV(const std::vector<std::string>& dates,
              const std::vector<double>& op_prices,
              const std::vector<double>& values,
              const std::vector<double>& IVs,
              const std::vector<double>& deltas,
              const std::vector<double>& HEs,
              const std::vector<double>& PNLs,
              const std::vector<double>& PNLs_Hedge,
              const std::string& filename);

#endif // UTILS_H
