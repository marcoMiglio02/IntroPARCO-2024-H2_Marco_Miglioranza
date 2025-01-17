#include "file_operation.h"
#include <fstream>
#include <vector>
#include <string>
#include<iostream>

using namespace std;

void saveResultsToCSV(const vector<double>& results, const vector<string>& headers) {
    string directory = "../results";
    string filePath = directory + "/MPIresults.csv";
    ofstream file(filePath, ios::app);

    // Write headers only if the file is empty
    file.seekp(0, ios::end);
    if (file.tellp() == 0) {
        for (size_t i = 0; i < headers.size(); ++i) {
            file << headers[i];
            if (i < headers.size() - 1) file << ",";
        }
        file << endl;
    }

    // Write results, each row corresponding to a new test iteration
    for (size_t i = 0; i < results.size(); ++i) {
        
            file << results[i];
            if (i < results.size() - 1) file << ",";
    
        
    }
    file << endl;
    file.close();
}
