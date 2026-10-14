#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <limits>
using namespace std;

struct Student {
    int id;
    string name;
    int age;
    string course;
    float marks;
};

vector<Student> students;
const string FILE_NAME = "students.txt";

// Find student by ID
int findStudent(int id) {
    for (int i = 0; i < (int)students.size(); i++) {
        if (students[i].id == id)
            return i;
    }
    return -1;
}

// Safely read an integer
int readInt(string message) {
    int value;
    while (true) {
        cout << message;
        if (cin >> value) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input! Enter a number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Safely read marks
float readMarks() {
    float marks;
    while (true) {
        cout << "Enter Marks (0-100): ";
        if (cin >> marks && marks >= 0 && marks <= 100) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return marks;
        }
        cout << "Invalid marks! Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Calculate grade
string calculateGrade(float marks) {
    if (marks >= 90) return "A+";
    if (marks >= 80) return "A";
    if (marks >= 70) return "B";
    if (marks >= 60) return "C";
    if (marks >= 50) return "D";
    return "F";
}

// Save all records to file
void saveToFile() {
    ofstream file(FILE_NAME);

    if (!file) {
        cout << "Error saving file!\n";
        return;
    }

    for (const Student &s : students) {
        file << s.id << '|'
             << s.name << '|'
             << s.age << '|'
             << s.course << '|'
             << s.marks << '\n';
    }
}

// Load records when program starts
void loadFromFile() {
    ifstream file(FILE_NAME);
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        Student s;
        string marksText, idText, ageText;

        if (getline(ss, idText, '|') &&
            getline(ss, s.name, '|') &&
            getline(ss, ageText, '|') &&
            getline(ss, s.course, '|') &&
            getline(ss, marksText)) {
            try {
                s.id = stoi(idText);
                s.age = stoi(ageText);
                s.marks = stof(marksText);

                if (s.id > 0 && s.age > 0 &&
                    s.marks >= 0 && s.marks <= 100 &&
                    findStudent(s.id) == -1) {
                    students.push_back(s);
                }
            } catch (...) {
                // Skip an invalid record
            }
        }
    }
}

// Add student
void addStudent() {
    Student s;

    s.id = readInt("Enter Student ID: ");

    if (s.id <= 0 || findStudent(s.id) != -1) {
        cout << "Invalid or duplicate ID!\n";
        return;
    }

    cout << "Enter Name: ";
    getline(cin, s.name);

    if (s.name.empty() || s.name.find('|') != string::npos) {
        cout << "Invalid name!\n";
        return;
    }

    s.age = readInt("Enter Age: ");

    if (s.age <= 0) {
        cout << "Age must be positive!\n";
        return;
    }

    cout << "Enter Course: ";
    getline(cin, s.course);

    if (s.course.empty() || s.course.find('|') != string::npos) {
        cout << "Invalid course!\n";
        return;
    }

    s.marks = readMarks();

    students.push_back(s);
    saveToFile();

    cout << "\nStudent added successfully!\n";
}

// Display a single student
void displayStudent(const Student &s) {
    cout << left
         << setw(8) << s.id
         << setw(24) << s.name
         << setw(8) << s.age
         << setw(18) << s.course
         << setw(10) << fixed << setprecision(2) << s.marks
         << setw(8) << calculateGrade(s.marks)
         << '\n';
}

// Display all students
void displayAllStudents() {
    if (students.empty()) {
        cout << "No student records found.\n";
        return;
    }

    cout << "\n";
    cout << left
         << setw(8) << "ID"
         << setw(24) << "Name"
         << setw(8) << "Age"
         << setw(18) << "Course"
         << setw(10) << "Marks"
         << setw(8) << "Grade"
         << '\n';

    cout << string(76, '-') << '\n';

    for (const Student &s : students)
        displayStudent(s);

    cout << "\nTotal Students: " << students.size() << '\n';
}

// Search student
void searchStudent() {
    int id = readInt("Enter Student ID to search: ");
    int index = findStudent(id);

    if (index == -1) {
        cout << "Student not found!\n";
        return;
    }

    cout << "\nStudent Found:\n";
    cout << left
         << setw(8) << "ID"
         << setw(24) << "Name"
         << setw(8) << "Age"
         << setw(18) << "Course"
         << setw(10) << "Marks"
         << setw(8) << "Grade" << '\n';

    displayStudent(students[index]);
}

// Update student
void updateStudent() {
    int id = readInt("Enter Student ID to update: ");
    int index = findStudent(id);

    if (index == -1) {
        cout << "Student not found!\n";
        return;
    }

    Student &s = students[index];

    cout << "Current Name: " << s.name << '\n';
    cout << "Enter New Name (blank to keep current): ";
    string value;
    getline(cin, value);

    if (!value.empty() && value.find('|') == string::npos)
        s.name = value;
    else if (value.find('|') != string::npos) {
        cout << "Invalid name. Keeping old name.\n";
    }

    int age = readInt("Enter New Age (0 to keep current): ");
    if (age > 0)
        s.age = age;

    cout << "Current Course: " << s.course << '\n';
    cout << "Enter New Course (blank to keep current): ";
    getline(cin, value);

    if (!value.empty() && value.find('|') == string::npos)
        s.course = value;
    else if (value.find('|') != string::npos) {
        cout << "Invalid course. Keeping old course.\n";
    }

    cout << "Current Marks: " << s.marks << '\n';
    cout << "Update marks? (1 = Yes, 0 = No): ";
    int choice = readInt("");

    if (choice == 1)
        s.marks = readMarks();

    saveToFile();
    cout << "Student updated successfully!\n";
}

// Delete student
void deleteStudent() {
    int id = readInt("Enter Student ID to delete: ");
    int index = findStudent(id);

    if (index == -1) {
        cout << "Student not found!\n";
        return;
    }

    cout << "Delete record of " << students[index].name
         << "? (1 = Yes, 0 = No): ";
    int choice = readInt("");

    if (choice == 1) {
        students.erase(students.begin() + index);
        saveToFile();
        cout << "Student deleted successfully!\n";
    } else {
        cout << "Deletion cancelled.\n";
    }
}

// Sort students by marks
void sortStudents() {
    sort(students.begin(), students.end(),
         [](const Student &a, const Student &b) {
             return a.marks > b.marks;
         });

    saveToFile();
    cout << "Students sorted by marks (highest first).\n";
    displayAllStudents();
}

// Main menu
int main() {
    loadFromFile();

    int choice;

    do {
        cout << "\n";
        cout << "====================================\n";
        cout << "     STUDENT MANAGEMENT SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student\n";
        cout << "5. Delete Student\n";
        cout << "6. Sort Students by Marks\n";
        cout << "7. Exit\n";
        cout << "====================================\n";

        choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1:
                addStudent();
                break;
            case 2:
                displayAllStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                sortStudents();
                break;
            case 7:
                saveToFile();
                cout << "Thank you for using the system!\n";
                break;
            default:
                cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 7);

    return 0;
}
