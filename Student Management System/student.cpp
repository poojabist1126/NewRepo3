#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "student.hpp"
#include "filemodification.hpp"
#include "common.hpp"

using namespace std;

static auto inputPrompt = [](const string& message, string& inputValue) -> bool {
    cout << message;
    getline(cin, inputValue);
    inputValue = trim(inputValue);
    return !inputValue.empty();
    };

static auto retryPrompt = []() -> bool {
    char response;
    cout << "Retry operation? (y/n): ";
    cin >> response;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return tolower(response) == 'y';
    };

bool Student::add() {
    while (true) {
        if (!inputPrompt("Student Name: ", name)) {
            if (!retryPrompt()) return false;
            continue;
        }

        if (!name.empty()) break;
        cout << "Name field cannot be blank.\n";
        if (!retryPrompt()) return false;
    }

    while (true) {
        if (!inputPrompt("Email Address: ", email)) {
            if (!retryPrompt()) return false;
            continue;
        }

        if (isValidEmail(email)) break;
        cout << "Invalid email format.\n";
        if (!retryPrompt()) return false;
    }

    while (true) {
        if (!inputPrompt("Password (minimum 6 characters): ", password)) {
            if (!retryPrompt()) return false;
            continue;
        }

        if (password.size() >= 6) break;
        cout << "Password does not meet requirements.\n";
        if (!retryPrompt()) return false;
    }

    string currentDate = getTodayDate();
    currentDate.erase(remove(currentDate.begin(), currentDate.end(), '-'), currentDate.end());
    size_t studentCount = readTxtFile(STUDENT_FILE).size();

    id = currentDate + to_string(studentCount + 1);

    vector<vector<string>> gradeRecords = readTxtFile(GRADE_FILE);
    vector<string> newStudentGrades = { id };
    for (int i = 0; i < gradeRecords[0].size() - 1; i++) {
        newStudentGrades.push_back("- -");
    }

    return writeTxtFile(STUDENT_FILE, { {id, name, email, password} }) &&
        writeTxtFile(GRADE_FILE, { newStudentGrades });
}

bool Student::del() {
    string studentId;

    cout << "Enter student identifier: ";
    getline(cin, studentId);

    if (!studentId.empty()) {
        if (readTxtFile(STUDENT_FILE, 0, studentId).empty()) {
            cout << "Student " << studentId << " not found in records." << endl;
            return false;
        }
        else {
            return deleteRowTxtFile(STUDENT_FILE, 0, studentId);
        }
    }
    else {
        cout << "Identifier field cannot be empty." << endl;
        return false;
    }
}

bool Student::edit() {
    string studentId;

    cout << "Enter student identifier: ";
    getline(cin, studentId);

    if (!studentId.empty()) {
        if (readTxtFile(STUDENT_FILE, 0, studentId).empty()) {
            cout << "Student " << studentId << " not found in records." << endl;
            return false;
        }
        else {
            while (true) {
                inputPrompt("Name: ", name);
                break;
            }

            while (true) {
                inputPrompt("Email: ", email);

                if (isValidEmail(email) || email.empty()) break;
                cout << "Invalid email format.\n";
                if (!retryPrompt()) return false;
            }

            while (true) {
                inputPrompt("Password (minimum 6 characters): ", password);

                if (password.size() >= 6 || password.empty()) break;
                cout << "Password does not meet requirements.\n";
                if (!retryPrompt()) return false;
            }

            return updateTxtFile(STUDENT_FILE, 0, studentId, { studentId, name, email, password });
        }
    }
    else {
        cout << "Identifier field cannot be empty." << endl;
        return false;
    }
}

void Student::show() {
    vector<vector<string>> studentRecords = readTxtFile(STUDENT_FILE);

    for (int i = 0; i < studentRecords.size(); i++) {
        studentRecords[i] = { studentRecords[i][0], studentRecords[i][1], studentRecords[i][2] };
    }

    printTable(studentRecords);
}