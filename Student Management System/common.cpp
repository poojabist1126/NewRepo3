#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <regex>

#include "common.hpp"

using namespace std;

vector<string> split(string inputString, char separator) {
    if (inputString.empty())
        return { "" };

    vector<string> resultParts;
    string currentPart;
    stringstream streamProcessor(inputString);

    while (getline(streamProcessor, currentPart, separator)) {
        resultParts.push_back(currentPart);
    }

    return resultParts;
}

string trim(const string& inputString) {
    if (inputString.empty())
        return "";

    auto startPosition = find_if(inputString.begin(), inputString.end(), [](unsigned char character) {
        return !isspace(character);
        });

    auto endPosition = find_if(inputString.rbegin(), inputString.rend(), [](unsigned char character) {
        return !isspace(character);
        }).base();

    return (startPosition < endPosition) ? string(startPosition, endPosition) : "";
}

bool isValidEmail(const string& emailAddress) {
    if (emailAddress.empty())
        return false;

    const regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return regex_match(emailAddress, emailPattern);
}

string getTodayDate() {
    time_t currentTime = time(nullptr);
    tm timeStructure{};
    localtime_s(&timeStructure, &currentTime);

    ostringstream dateStream;
    dateStream << put_time(&timeStructure, "%Y-%m-%d");
    return dateStream.str();
}

bool checkCommandValidity(string userCommand, vector<vector<string>> commandDefinitions, int userRole) {
    int commandIndex = 0;
    for (commandIndex = 0; commandIndex < commandDefinitions.size(); commandIndex++) {
        if (commandDefinitions[commandIndex][0] == userCommand || userCommand == "") {
            break;
        }
    }

    if (commandIndex >= commandDefinitions.size()) {
        cout << "Invalid command entered. Use 'help' to view available commands." << endl;
        return false;
    }

    vector<string> authorizedRoles = split(commandDefinitions[commandIndex][2], ' ');

    if (find(authorizedRoles.begin(), authorizedRoles.end(), to_string(userRole)) == authorizedRoles.end() && authorizedRoles[0] != "0") {
        for (auto roleCode : authorizedRoles) {
            cout << "Authorization required: ";

            if (roleCode == "1")
                cout << "Administrator";
            else if (roleCode == "2")
                cout << "Instructor";
            else if (roleCode == "3")
                cout << "Learner";

            cout << " privileges needed." << endl;
        }
        return false;
    }

    return true;
}

void printTable(const vector<vector<string>>& dataTable, int sortColumn, bool sortOrder) {
    if (dataTable.empty()) return;

    size_t columnCount = 0;
    for (const auto& tableRow : dataTable)
        columnCount = max(columnCount, tableRow.size());

    vector<vector<string>> sortedData = dataTable;

    if (sortColumn >= 0 && sortColumn < columnCount) {
        sort(sortedData.begin() + 1, sortedData.end(), [&](const vector<string>& rowA, const vector<string>& rowB) {
            string valueA = (sortColumn < rowA.size()) ? rowA[sortColumn] : "";
            string valueB = (sortColumn < rowB.size()) ? rowB[sortColumn] : "";
            return sortOrder ? (valueA < valueB) : (valueA > valueB);
            });
    }

    vector<size_t> columnWidths(columnCount, 0);
    for (const auto& tableRow : sortedData)
        for (size_t i = 0; i < tableRow.size(); ++i)
            columnWidths[i] = max(columnWidths[i], tableRow[i].length());

    auto drawBorder = [&]() {
        cout << "+";
        for (auto width : columnWidths)
            cout << string(width + 2, '-') << "+";
        cout << '\n';
        };

    auto displayRow = [&](const vector<string>& rowData) {
        cout << "|";
        for (size_t i = 0; i < columnCount; ++i) {
            string cellContent = (i < rowData.size()) ? rowData[i] : "";
            cout << " " << left << setw(columnWidths[i]) << cellContent << " |";
        }
        cout << '\n';
        };

    drawBorder();
    bool firstRow = true;
    for (const auto& rowData : sortedData) {
        displayRow(rowData);
        if (firstRow) {
            drawBorder();
            firstRow = false;
        }
    }
    drawBorder();
}