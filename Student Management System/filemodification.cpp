#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

#include "filemodification.hpp"

using namespace std;

vector<vector<string>> readTxtFile(
    const string& filename,
    int colNumber,
    const string& colValue
) {
    vector<vector<string>> fileContents;
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        cerr << "Error: Could not locate the requested file." << endl;
        return fileContents;
    }

    string currentLine;
    while (getline(inputFile, currentLine)) {
        vector<string> rowData;
        stringstream lineParser(currentLine);
        string fieldData;

        while (getline(lineParser, fieldData, '|')) {
            rowData.push_back(fieldData);
        }

        if (colNumber == -1 ||
            (colNumber >= 0 && colNumber < rowData.size() && rowData[colNumber] == colValue)) {
            fileContents.push_back(rowData);
        }
    }

    inputFile.close();
    return fileContents;
}

bool writeTxtFile(
    const string& filename,
    const vector<vector<string>>& data,
    bool append
) {
    ofstream outputFile;

    if (append) {
        outputFile.open(filename, ios::out | ios::app);
    }
    else {
        outputFile.open(filename, ios::out | ios::trunc);
    }

    if (outputFile.is_open()) {
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                outputFile << row[i];
                if (i < row.size() - 1) {
                    outputFile << "|";
                }
            }
            outputFile << "\n";
        }
        outputFile.close();

        return true;
    }
    else
        return false;
}

bool addColumnToTxtFile(
    const string& filename,
    const vector<string>& newColumnData
) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        return false;
    }

    vector<vector<string>> existingData;
    string lineContent;
    size_t rowCount = 0;

    while (getline(inputFile, lineContent)) {
        vector<string> rowEntries;
        stringstream lineStream(lineContent);
        string cellValue;

        while (getline(lineStream, cellValue, '|')) {
            rowEntries.push_back(cellValue);
        }

        existingData.push_back(rowEntries);
        ++rowCount;
    }
    inputFile.close();

    if (newColumnData.size() != rowCount) {
        return false;
    }

    for (size_t i = 0; i < rowCount; ++i) {
        existingData[i].push_back(newColumnData[i]);
    }

    return writeTxtFile(filename, existingData, false);
}

bool deleteRowTxtFile(
    const string& filename,
    size_t matchColumnIndex,
    const string& matchValue
) {
    auto fileData = readTxtFile(filename);
    bool itemFound = false;

    vector<vector<string>> updatedContent;

    for (const auto& row : fileData) {
        if (row.size() > matchColumnIndex && row[matchColumnIndex] == matchValue) {
            itemFound = true;
            continue;
        }

        updatedContent.push_back(row);
    }

    if (itemFound) {
        return writeTxtFile(filename, updatedContent, false);
    }
    else {
        return false;
    }
}

bool updateTxtFile(
    const string& filename,
    size_t matchColumnIndex,
    const string& matchValue,
    const vector<string>& newRow
) {
    auto fileData = readTxtFile(filename);
    bool updatePerformed = false;

    for (auto& row : fileData) {
        if (row.size() > matchColumnIndex && row[matchColumnIndex] == matchValue) {
            if (row.size() < newRow.size()) {
                row.resize(newRow.size());
            }

            for (size_t i = 0; i < newRow.size(); ++i) {
                if (!newRow[i].empty())
                    row[i] = newRow[i];
            }

            updatePerformed = true;
            break;
        }
    }

    if (updatePerformed) {
        return writeTxtFile(filename, fileData, false);
    }
    else {
        return false;
    }
}