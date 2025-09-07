#ifndef COURSE_HPP
#define COURSE_HPP

class Course {
private:
    std::string code;
    std::string name;
    std::string description;
    std::string teachers;

    std::string COURSE_FILE = "course_data.txt",
        STUDENT_FILE = "student_data.txt",
        TEACHER_FILE = "teacher_data.txt",
        ADMIN_FILE = "administrator_data.txt",
        GRADE_FILE = "grade_data.txt";

public:
    bool add();
    bool del();
    bool edit();
    bool editMark(std::string id, std::string course, int markType, std::string mark);
    std::vector<std::string> getMarks(const std::string& studentId, const std::string& courseName);
    bool enroll(std::string studentId = "");
    bool disenroll(std::string studentId = "");
    bool assignTeacher();
    bool disassignTeacher();
    std::vector<std::string> getEnrolledCourses(std::string studentId = "");
    void generateReport(std::string studentId = "");
    void show();
};

#endif