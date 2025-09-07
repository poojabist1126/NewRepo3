#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>   

#include "teacher.hpp"
#include "filemodification.hpp"
#include "common.hpp"

using namespace std;

static auto inputHandler = [](const string& message, string& inputValue) -> bool {
    cout << message;
    getline(cin, inputValue);
    inputValue = trim(inputValue);
    return !inputValue.empty();
    };

static auto retryHandler = []() -> bool {
    char response;
    cout << "Would you like to attempt again? (y/n): ";
    cin >> response;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return tolower(response) == 'y';
    };


bool Teacher::add() {
    while (true) {
        if (!inputHandler("Instructor Name: ", name)) {
            if (!retryHandler()) return false;
            continue;
        }

        if (!name.empty()) break;
        cout << "Name field cannot be blank.\n";
        if (!retryHandler()) return false;
    }

    while (true) {
        if (!inputHandler("Email Address: ", email)) {
            if (!retryHandler()) return false;
            continue;
        }

        if (isValidEmail(email)) break;
        cout << "Invalid email format provided.\n";
        if (!retryHandler()) return false;
    }

    while (true) {
        if (!inputHandler("Password (minimum 6 characters): ", password)) {
            if (!retryHandler()) return false;
            continue;
        }

        if (password.size() >= 6) break;
        cout << "Password does not meet length requirements.\n";
        if (!retryHandler()) return false;
    }

    srand(static_cast<unsigned int>(time(0)));
    string currentDate = getTodayDate();
    currentDate.erase(remove(currentDate.begin(), currentDate.end(), '-'), currentDate.end());
    size_t instructorCount = readTxtFile(TEACHER_FILE).size();

    id = "T" + to_string(rand() % 100 + 1) + to_string(instructorCount + 1) + currentDate;

    return writeTxtFile(TEACHER_FILE, { {id, name, email, password} });
}

bool Teacher::del() {
    string instructorId;

    cout << "Enter instructor identifier: ";
    getline(cin, instructorId);

    if (!instructorId.empty()) {
        if (readTxtFile(TEACHER_FILE, 0, instructorId).empty()) {
            cout << "Instructor " << instructorId << " not found in system." << endl;
            return false;
        }
        else {
            return deleteRowTxtFile(TEACHER_FILE, 0, instructorId);
        }
    }
    else {
        cout << "Identifier field cannot be empty." << endl;
        return false;
    }
}

bool Teacher::edit() {
    string instructorId;

    cout << "Enter instructor identifier: ";
    getline(cin, instructorId);

    if (!instructorId.empty()) {
        if (readTxtFile(TEACHER_FILE, 0, instructorId).empty()) {
            cout << "Instructor " << instructorId << " not found in system." << endl;
            return false;
        }
        else {
            while (true) {
                inputHandler("Name: ", name);
                break;
            }

            while (true) {
                inputHandler("Email: ", email);

                if (isValidEmail(email)) break;
                cout << "Invalid email format provided.\n";
                if (!retryHandler()) return false;
            }

            while (true) {
                inputHandler("Password (minimum 6 characters): ", password);

                if (password.size() >= 6) break;
                cout << "Password does not meet length requirements.\n";
                if (!retryHandler()) return false;
            }

            return updateTxtFile(TEACHER_FILE, 0, instructorId, { instructorId, name, email, password });
        }
    }
    else {
        cout << "Identifier field cannot be empty." << endl;
        return false;
    }
}

void Teacher::show() {
    vector<vector<string>> instructorRecords = readTxtFile(TEACHER_FILE);

    for (int i = 0; i < instructorRecords.size(); i++) {
        instructorRecords[i] = { instructorRecords[i][0], instructorRecords[i][1], instructorRecords[i][2] };
    }

    printTable(instructorRecords);
}