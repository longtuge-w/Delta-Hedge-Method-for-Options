#include "utils.h"

int binary_search(const std::vector<std::string>& vec, std::string target) {
    int left = 0;
    int right = vec.size() - 1;

    // Continue searching as long as the left boundary does not exceed the right boundary
    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Check if the middle element is the target
        if (vec[mid] == target) {
            return mid;  // Found the target, return its index
        } else if (vec[mid] < target) {
            left = mid + 1;  // Adjust the left boundary if the target is to the right
        } else {
            right = mid - 1;  // Adjust the right boundary if the target is to the left
        }
    }

    return left;  // Target was not found in the vector, return where it would be inserted
}

void writeCSV(const std::vector<std::string>& dates,
              const std::vector<double>& op_prices,
              const std::vector<double>& values,
              const std::vector<double>& IVs,
              const std::vector<double>& deltas,
              const std::vector<double>& HEs,
              const std::vector<double>& PNLs,
              const std::vector<double>& PNLs_Hedge,
              const std::string& filename) {
    
    // Ensure all vectors are of the same size for consistent CSV writing
    if (dates.size() != values.size() || dates.size() != IVs.size() || dates.size() != deltas.size() ||
        dates.size() != HEs.size() || dates.size() != PNLs.size() || dates.size() != PNLs_Hedge.size()) {
        std::cerr << "Vectors have different sizes!";
        return;
    }

    // Open a file for writing
    std::ofstream csvFile(filename);

    if (!csvFile.is_open()) {
        std::cerr << "Failed to open the CSV file!";
        return;
    }

    // Write headers to the CSV file
    csvFile << "date,S,V,Implied volatility,delta,hedging error,PNL,PNL (with hedge)" << "\n";

    // Loop through each element of the vectors and write them to the CSV file
    for (size_t i = 0; i < dates.size(); ++i) {
        csvFile << dates[i] << "," << op_prices[i] << "," << values[i] << "," << IVs[i] << "," << deltas[i] 
        << "," << HEs[i] << "," << PNLs[i] << "," << PNLs_Hedge[i] << "\n";
    }

    csvFile.close();  // Close the file after writing
}
