#include <iostream>
#include <cctype>
#include <string>
#include <vector>
#include <limits>

#include "common.hpp"
#include "filemodification.hpp"
#include "student.hpp"
#include "course.hpp"
#include "teacher.hpp"
#include "admin.hpp"

using namespace std;

struct UserAccount {
    int accountType = 0;
    string identifier = "";

    UserAccount(int t = 4, const string& i = "") : accountType(t), identifier(i) {}
};

UserAccount initializeUserAccount(string studentDataFile, string instructorDataFile, string adminDataFile) {
    UserAccount currentUser;
    int selectedType;
    string inputId, inputPassword;
    vector<vector<string>> userInformation;

    auto getUserInput = [](const string& message, string& inputValue) -> bool {
        cout << message;
        getline(cin, inputValue);
        inputValue = trim(inputValue);
        return !inputValue.empty();
        };

    auto promptRetry = []() -> bool {
        char response;
        cout << "Retry? (y/n): ";
        cin >> response;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return tolower(response) == 'y';
        };

    while (true) {
        cout << "Select account type (1=Admin, 2=Teacher, 3=Student): ";
        cin >> selectedType;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        if (selectedType >= 1 && selectedType <= 3) break;
        if (!promptRetry()) return currentUser;
    }

    while (true) {
        if (!getUserInput("Enter ID: ", inputId)) {
            if (!promptRetry()) return currentUser;
            continue;
        }
        string targetFile = (selectedType == 1) ? adminDataFile : (selectedType == 2) ? instructorDataFile : studentDataFile;
        userInformation = readTxtFile(targetFile, 0, inputId);
        if (!userInformation.empty()) break;
        cout << "ID not found in system.\n";
        if (!promptRetry()) return currentUser;
    }

    while (true) {
        if (!getUserInput("Enter Password: ", inputPassword)) {
            if (!promptRetry()) return currentUser;
            continue;
        }
        if (inputPassword == userInformation[0][3]) break;
        cout << "Incorrect password.\n";
        if (!promptRetry()) return currentUser;
    }

    currentUser.accountType = selectedType;
    currentUser.identifier = inputId;
    return currentUser;
}

int main()
{
    UserAccount currentUser;
    Student studentManager;
    Course courseManager;
    Teacher teacherManager;
    Admin adminManager;

    std::string COURSE_FILE = "course_data.txt",
        STUDENT_FILE = "student_data.txt",
        TEACHER_FILE = "teacher_data.txt",
        ADMIN_FILE = "administrator_data.txt",
        GRADE_FILE = "grade_data.txt",
        commandDataFile = "defined_command.txt";

    string userCommand;
    vector<string> commandParts;
    vector<vector<string>> availableCommands = readTxtFile(commandDataFile);

    cout << "Academic Management Portal" << endl;

    while (true) {
        if (currentUser.identifier.empty()) {
            currentUser = initializeUserAccount(STUDENT_FILE, TEACHER_FILE, ADMIN_FILE);
            continue;
        }

        cout << ">> ";
        getline(cin, userCommand);
        userCommand = trim(userCommand);
        commandParts = split(userCommand);

        if (!checkCommandValidity(userCommand, availableCommands, currentUser.accountType)) continue;

        if (commandParts[0] == "help") {
            printTable(availableCommands);
        }
        else if (commandParts[0] == "exit") {
            cout << "Session terminated." << endl;
            break;
        }
        else if (commandParts[0] == "logout") {
            if (currentUser.identifier.empty()) {
                cout << "Already logged out." << endl;
                continue;
            }

            currentUser.identifier = "";
            currentUser.accountType = 0;
            cout << "Logout complete." << endl;
        }
        else if (commandParts[0] == "add") {
            if (commandParts[1] == "student" && (currentUser.accountType == 1 || currentUser.accountType == 2) && commandParts.size() == 2) {
                if (studentManager.add())
                    cout << "Student registration successful." << endl;
                else
                    cout << "Student registration failed." << endl;
            }
            else if (commandParts[1] == "teacher" && currentUser.accountType == 1 && commandParts.size() == 2) {
                if (teacherManager.add())
                    cout << "Teacher registration successful." << endl;
                else
                    cout << "Teacher registration failed." << endl;
            }
            else if (commandParts[1] == "course" && (currentUser.accountType == 1 || currentUser.accountType == 2) && commandParts.size() == 2) {
                if (courseManager.add())
                    cout << "Course creation successful." << endl;
                else
                    cout << "Course creation failed." << endl;
            }
            else if (commandParts[1] == "admin" && currentUser.accountType == 1) {
                if (adminManager.add())
                    cout << "Admin registration successful." << endl;
                else
                    cout << "Admin registration failed." << endl;
            }
        }
        else if (commandParts[0] == "delete") {
            if (commandParts[1] == "student" && (currentUser.accountType == 1 || currentUser.accountType == 2) && commandParts.size() == 2) {
                if (studentManager.del())
                    cout << "Student removal successful." << endl;
                else
                    cout << "Student removal failed." << endl;
            }
            else if (commandParts[1] == "teacher" && currentUser.accountType == 1 && commandParts.size() == 2) {
                if (teacherManager.del())
                    cout << "Teacher removal successful." << endl;
                else
                    cout << "Teacher removal failed." << endl;
            }
            else if (commandParts[1] == "course" && (currentUser.accountType == 1 || currentUser.accountType == 2) && commandParts.size() == 2) {
                if (courseManager.del())
                    cout << "Course removal successful." << endl;
                else
                    cout << "Course removal failed." << endl;
            }
        }
        else if (commandParts[0] == "show") {
            if (commandParts[1] == "enroll-course") {
                vector<string> enrolledCourses;

                if (currentUser.accountType == 1 || currentUser.accountType == 2) enrolledCourses = courseManager.getEnrolledCourses();
                else if (currentUser.accountType == 3) enrolledCourses = courseManager.getEnrolledCourses(currentUser.identifier);

                if (!enrolledCourses.empty()) {
                    cout << "Current enrollments: " << endl;
                    for (auto course : enrolledCourses) {
                        cout << course << endl;
                    }
                }
            }
            else if (commandParts[1] == "report") {
                if (currentUser.accountType == 1 || currentUser.accountType == 2) courseManager.generateReport();
                else if (currentUser.accountType == 3) courseManager.generateReport(currentUser.identifier);
            }
            else if (commandParts[1] == "teacher" && (currentUser.accountType == 1 || currentUser.accountType == 2 || currentUser.accountType == 3)) {
                teacherManager.show();
            }
            else if (commandParts[1] == "course" && (currentUser.accountType == 1 || currentUser.accountType == 2 || currentUser.accountType == 3)) {
                courseManager.show();
            }
            else if (commandParts[1] == "student" && (currentUser.accountType == 1 || currentUser.accountType == 2 || currentUser.accountType == 3)) {
                studentManager.show();
            }
        }
        else if (commandParts[0] == "edit") {
            if (commandParts[1] == "student" && (currentUser.accountType == 1 || currentUser.accountType == 2 || currentUser.accountType == 3) && commandParts.size() == 2) {
                if (studentManager.edit())
                    cout << "Student update successful." << endl;
                else
                    cout << "Student update failed." << endl;
            }
            else if (commandParts[1] == "teacher" && (currentUser.accountType == 1 || currentUser.accountType == 2) && commandParts.size() == 2) {
                if (teacherManager.edit())
                    cout << "Teacher update successful." << endl;
                else
                    cout << "Teacher update failed." << endl;
            }
            else if (commandParts[1] == "course" && (currentUser.accountType == 1 || currentUser.accountType == 2) && commandParts.size() == 2) {
                if (courseManager.edit())
                    cout << "Course update successful." << endl;
                else
                    cout << "Course update failed." << endl;
            }
            else if (commandParts[1] == "mark" && (currentUser.accountType == 1 || currentUser.accountType == 2) && commandParts.size() == 2) {
                string studentId, courseCode;
                int assessmentType, scoreValue;
                vector<string> currentMarks;

                cout << "Student ID: ";
                getline(cin, studentId);
                studentId = trim(studentId);

                cout << "Course Code: ";
                getline(cin, courseCode);
                courseCode = trim(courseCode);

                cout << "Assessment type (1 - internal, 2 - final):";
                cin >> assessmentType;

                if (cin.fail())
                    cout << "Invalid input." << endl;

                cout << "Score: ";
                cin >> scoreValue;

                if (cin.fail())
                    cout << "Invalid input." << endl;

                currentMarks = courseManager.getMarks(studentId, courseCode);

                if (currentMarks.size() == 2) {
                    if (currentMarks[0] == "-" && currentMarks[1] == "-") {
                        cout << "Student " << studentId << " not enrolled in " << courseCode << "." << endl;
                    }
                    else {
                        if (courseManager.editMark(studentId, courseCode, assessmentType, to_string(scoreValue)))
                            cout << "Score updated." << endl;
                        else
                            cout << "Score update failed." << endl;
                    }
                }
                else if (currentMarks.size() == 1) {
                    cout << currentMarks[0] << endl;
                }
            }
        }
        else if (commandParts[0] == "enroll") {
            if (commandParts[1] == "course") {
                if (currentUser.accountType == 1 || currentUser.accountType == 2)
                    if (courseManager.enroll())
                        cout << "Enrollment complete." << endl;
                    else
                        cout << "Enrollment failed." << endl;
                else if (currentUser.accountType == 3)
                    if (courseManager.enroll(currentUser.identifier))
                        cout << "Enrollment complete." << endl;
                    else
                        cout << "Enrollment failed." << endl;
            }
        }
        else if (commandParts[0] == "disenroll") {
            if (commandParts[1] == "course") {
                if (currentUser.accountType == 1 || currentUser.accountType == 2)
                    if (courseManager.disenroll())
                        cout << "Disenrollment complete." << endl;
                    else
                        cout << "Disenrollment failed." << endl;
                else if (currentUser.accountType == 3)
                    if (courseManager.disenroll(currentUser.identifier))
                        cout << "Disenrollment complete." << endl;
                    else
                        cout << "Disenrollment failed." << endl;
            }
        }
        else if (commandParts[0] == "assign") {
            if (commandParts[1] == "teacher" && currentUser.accountType == 1) {
                if (courseManager.assignTeacher())
                    cout << "Instructor assigned." << endl;
                else
                    cout << "Assignment failed." << endl;
            }
        }
        else if (commandParts[0] == "disassign") {
            if (commandParts[1] == "teacher" && currentUser.accountType == 1) {
                if (courseManager.disassignTeacher())
                    cout << "Instructor unassigned." << endl;
                else
                    cout << "Unassignment failed." << endl;
            }
        }
    }
}