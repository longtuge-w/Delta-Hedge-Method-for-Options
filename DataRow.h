#ifndef DataRow_H
#define DataRow_H

#include <string>
#include <vector>
#include <iostream>

// Struct to represent a row of data containing information about option contracts
struct DataRow {
    std::string date;      // Date of the data point
    std::string exdate;    // Expiry date of the option contract
    char cp_flag;          // Call or put flag ('C' for call, 'P' for put)
    double strike_pr;      // Strike price of the option contract

    // Constructor to initialize a DataRow object
    DataRow(std::string d, std::string e, char c, int s);
};

// Function to construct a vector of DataRow objects from given input vectors
std::vector<DataRow> constructDataRows(
    const std::vector<std::string>& dates,
    const std::vector<std::string>& exdates,
    const std::vector<char>& cp_flags,
    const std::vector<int>& strike_prs
);

// Function to perform binary search on a vector of DataRow objects to find a given target row
int binary_search_row(const std::vector<DataRow>& data, const DataRow& target);

#endif // DATAROW_H