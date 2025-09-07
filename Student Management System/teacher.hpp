#ifndef TEACHER_HPP
#define TEACHER_HPP

class Teacher {
private:
    std::string id;
    std::string name;
    std::string email;
    std::string password;

    std::string COURSE_FILE = "course_data.txt",
        STUDENT_FILE = "student_data.txt",
        TEACHER_FILE = "teacher_data.txt",
        ADMIN_FILE = "administrator_data.txt",
        GRADE_FILE = "grade_data.txt";

public:
    bool add();
    bool del();
    bool edit();
    void show();
};

#endif