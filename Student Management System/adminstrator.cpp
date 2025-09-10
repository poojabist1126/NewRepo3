#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "adminstrator.hpp"
#include "filemodification.hpp"
#include "common.hpp"

using namespace std;

static auto getInput = [](const string& prompt, string& value) -> bool {
    cout << prompt;
    getline(cin, value);
    value = trim(value);
    return !value.empty();
    };

static auto askRetry = []() -> bool {
    char response;
    cout << "Retry operation? (y/n): ";
    cin >> response;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return tolower(response) == 'y';
    };

bool Adminstrator::create() {
    while (true) {
        if (!getInput("Administrator Name: ", name)) {
            if (!askRetry()) return false;
            continue;
        }

        if (!name.empty()) break;
        cout << "Name field cannot be empty.\n";
        if (!askRetry()) return false;
    }

    while (true) {
        if (!getInput("Email Address: ", email)) {
            if (!askRetry()) return false;
            continue;
        }

        if (isValidEmail(email)) break;
        cout << "Email format is invalid.\n";
        if (!askRetry()) return false;
    }

    while (true) {
        if (!getInput("Password (6 characters minimum): ", password)) {
            if (!askRetry()) return false;
            continue;
        }

        if (password.size() >= 6) break;
        cout << "Password does not meet length requirements.\n";
        if (!askRetry()) return false;
    }

    srand(static_cast<unsigned int>(time(0)));
    string currentDate = getTodayDate();
    currentDate.erase(remove(currentDate.begin(), currentDate.end(), '-'), currentDate.end());
    size_t adminCount = readTxtFile(ADMIN_FILE).size();

    id = "A" + to_string(rand() % 100 + 1) + to_string(adminCount + 1) + currentDate;

    return writeTxtFile(ADMIN_FILE, { {id, name, email, password} });
}

bool Adminstrator::del() {
    string inputId;

    cout << "Enter administrator ID: ";
    getline(cin, inputId);

    if (!inputId.empty()) {
        if (readTxtFile(ADMIN_FILE, 0, inputId).empty()) {
            cout << "Administrator " << inputId << " does not exist." << endl;
            return false;
        }
        else {
            return deleteRowTxtFile(ADMIN_FILE, 0, inputId);
        }
    }
    else {
        cout << "ID field cannot be empty." << endl;
        return false;
    }
}

bool Adminstrator::update() {
    string inputId;

    cout << "Enter administrator ID: ";
    getline(cin, inputId);

    if (!inputId.empty()) {
        if (readTxtFile(ADMIN_FILE, 0, inputId).empty()) {
            cout << "Administrator " << inputId << " does not exist." << endl;
            return false;
        }
        else {
            while (true) {
                getInput("Name: ", name);
                break;
            }

            while (true) {
                getInput("Email: ", email);

                if (isValidEmail(email)) break;
                cout << "Email format is invalid.\n";
                if (!askRetry()) return false;
            }

            while (true) {
                getInput("Password (6 characters minimum): ", password);

                if (password.size() >= 6) break;
                cout << "Password does not meet length requirements.\n";
                if (!askRetry()) return false;
            }

            return updateTxtFile(ADMIN_FILE, 0, inputId, { inputId, name, email, password });
        }
    }
    else {
        cout << "ID field cannot be empty." << endl;
        return false;
    }
}

void Adminstrator::view() {
    vector<vector<string>> administratorData = readTxtFile(ADMIN_FILE);

    for (int i = 0; i < administratorData.size(); i++) {
        administratorData[i] = { administratorData[i][0], administratorData[i][1], administratorData[i][2] };
    }

    printTable(administratorData);
}