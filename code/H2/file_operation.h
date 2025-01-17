#ifndef FILE_OPERATION_H
#define FILE_OPERATION_H

#include <vector>
#include <string>

// Function declaration for saving results to a CSV file
void saveResultsToCSV(const std::vector<double>& results, const std::vector<std::string>& headers);

#endif // FILE_OPERATION_H