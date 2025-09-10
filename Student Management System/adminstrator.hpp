#ifndef ADMINSTRATOR_HPP
#define ADMINSTRATOR_HPP

class Adminstrator {
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
    bool create();
    bool del();
    bool update();
    void view();
};

#endif