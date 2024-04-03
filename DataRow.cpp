#include "DataRow.h"

DataRow::DataRow(std::string d, std::string e, char c, int s)
    : date(d), exdate(e), cp_flag(c), strike_pr(s) {}  // Initialize a DataRow object with given values

std::vector<DataRow> constructDataRows(
    const std::vector<std::string>& dates,
    const std::vector<std::string>& exdates,
    const std::vector<char>& cp_flags,
    const std::vector<int>& strike_prs
) {
    std::vector<DataRow> dataRows;  // Vector to hold constructed DataRow objects

    // Ensure all input vectors have the same size
    if (dates.size() != exdates.size() || dates.size() != cp_flags.size() || dates.size() != strike_prs.size()) {
        std::cerr << "Error: All input vectors must be of the same length." << std::endl;
        return dataRows;
    }

    // Construct DataRow objects and add them to the dataRows vector
    for (size_t i = 0; i < dates.size(); ++i) {
        DataRow row(dates[i], exdates[i], cp_flags[i], strike_prs[i]);
        dataRows.push_back(row);
    }

    return dataRows;
}

int binary_search_row(const std::vector<DataRow>& data, const DataRow& target) {
    // Perform binary search on a vector of DataRow objects
    int left = 0;
    int right = data.size() - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        // Compare DataRow fields to determine position in search
        if (data[mid].date == target.date && 
            data[mid].exdate == target.exdate && 
            data[mid].cp_flag == target.cp_flag && 
            data[mid].strike_pr == target.strike_pr) {
            return mid;  // Target found
        } 
        else if (data[mid].date < target.date || 
                 (data[mid].date == target.date && data[mid].exdate < target.exdate) ||
                 (data[mid].date == target.date && data[mid].exdate == target.exdate && data[mid].cp_flag < target.cp_flag) ||
                 (data[mid].date == target.date && data[mid].exdate == target.exdate && data[mid].cp_flag == target.cp_flag && data[mid].strike_pr < target.strike_pr)) {
            left = mid + 1;  // Search in the right half
        } 
        else {
            right = mid - 1;  // Search in the left half
        }
    }

    return -1;  // Target not found
}
