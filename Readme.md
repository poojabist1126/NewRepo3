# 🎓 Student Management System

The **Student Management System** is a **C++14 console-based application** designed to manage academic operations in an educational institution.  
It supports **role-based access** for administrators, teachers, and students, and provides a **command-driven interface** for managing students, teachers, courses, enrollments, grades, and reports.

---

## 📑 Table of Contents

- [✨ Features](#-features)  
- [👥 User Stories](#-user-stories)  
- [📂 Project Structure](#-project-structure)  
- [⚙️ Getting Started](#️-getting-started)  
- [🖥 Usage](#-usage)  
- [📁 Data Files](#-data-files)  
- [📖 Commands Reference](#-commands-reference)  

---

## ✨ Features

- **Role-Based Access**: Secure login for Admin, Teacher, and Student accounts.  
- **Student Management**: Add, update, view, and delete student records.  
- **Teacher Management**: Manage teacher accounts and records.  
- **Course Management**: Add, update, and remove courses.  
- **Enrollment Management**: Enroll and disenroll students in courses.  
- **Grade Management**: Assign and update student marks.  
- **Reporting**: Generate reports for students, teachers, courses, and enrollments.  
- **Command-Line Interface**: Simple text-based interaction.  
- **Persistent Data Storage**: Data stored in plain text files for easy backup and management.  

---

## 👥 User Stories

### 🔑 Administrator
- Manage students, teachers, courses, and admin accounts.  
- Assign/unassign teachers to courses.  
- View and generate reports.  
- Manage student grades.  
- Secure login access.  

### 👨‍🏫 Teacher
- Manage students and courses.  
- Assign/update grades for students.  
- Generate course reports.  
- Secure login access.  

### 👩‍🎓 Student
- View personal information, enrolled courses, and grades.  
- Enroll/disenroll in courses.  
- Update personal information.  
- Secure login access.  

---

## 📂 Project Structure

```pgsql
Student Management System/
├── main.cpp                # Main application logic & command loop
├── student.hpp             # Student management
├── teacher.hpp             # Teacher management
├── course.hpp              # Course management
├── administrator.hpp       # Administrator management
├── common.hpp              # Utility functions
├── filemodification.hpp    # File I/O utilities
├── student_data.txt        # Student records
├── teacher_data.txt        # Teacher records
├── course_data.txt         # Course records
├── administrator_data.txt  # Admin records
├── grade_data.txt          # Student grades
├── defined_command.txt     # List of available commands
```
---

## 🖥 Usage

1. **Login** → Choose account type (Admin/Teacher/Student) and enter credentials.  
2. **Commands** → Enter commands at the prompt (`>>`).  
3. **Access Control** → Only role-permitted commands are accepted.  
4. **Persistence** → All changes are saved automatically to text files.  

---

## 📁 Data Files

| File Name                | Description            |
|---------------------------|------------------------|
| `student_data.txt`        | Student records        |
| `teacher_data.txt`        | Teacher records        |
| `course_data.txt`         | Course records         |
| `administrator_data.txt`  | Admin records          |
| `grade_data.txt`          | Student grades         |
| `defined_command.txt`     | Available commands     |

---

## 📖 Commands Reference

### 🔐 Authentication
| Command | Description |
|---------|-------------|
| `help`  | Show all commands |
| `exit`  | Exit the application |
| `logout`| Logout from current session |

### 👩‍🎓 Student Management
| Command | Description | Role |
|---------|-------------|------|
| `create student` | Add new student | Admin, Teacher |
| `del student`    | Remove student | Admin, Teacher |
| `update student` | Update student info | Admin, Teacher, Student |
| `view student`   | View all students | All |

### 👨‍🏫 Teacher Management
| Command | Description | Role |
|---------|-------------|------|
| `create teacher` | Add new teacher | Admin |
| `del teacher`    | Remove teacher | Admin |
| `update teacher` | Update teacher info | Admin, Teacher |
| `view teacher`   | View all teachers | All |

### 📚 Course Management
| Command | Description | Role |
|---------|-------------|------|
| `create course` | Add new course | Admin, Teacher |
| `del course`    | Remove course | Admin, Teacher |
| `update course` | Update course info | Admin, Teacher |
| `view course`   | View all courses | All |

### 📝 Enrollment
| Command | Description | Role |
|---------|-------------|------|
| `enroll course`   | Enroll student in a course | Admin, Teacher, Student |
| `disenroll course`| Remove student from a course | Admin, Teacher, Student |
| `view enroll-course` | View enrollments | All |

### 📊 Grade Management
| Command | Description | Role |
|---------|-------------|------|
| `assign mark` | Assign/update student marks | Admin, Teacher |
| `view report` | View reports | All |

### 🛡 Admin Management
| Command | Description | Role |
|---------|-------------|------|
| `create admin` | Add new administrator | Admin |

### 📌 Course Assignment
| Command | Description | Role |
|---------|-------------|------|
| `assign teacher`   | Assign teacher to a course | Admin |
| `disassign teacher`| Unassign teacher from course | Admin |

---

⚠️ **Note**:  
- All commands are **case-sensitive**.  
- Only permitted roles can execute specific commands.  
