#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

struct User {   
    std::string uID;
    std::string name;
    std::string password;
    double balance;
    std::string course;
    double duePayment;


    User() : uID("None"), name("None"), balance(0.0), password("123"), course("None"), duePayment(0.0) {} // Default Constructor
    User(std::string uID, std::string name, std::string password, double balance, std::string course, double duePayment)
        : uID(uID), name(name), password(password), balance(balance), course(course), duePayment(duePayment) {}
};

void parseLineCSV(const std::string& line, std::string* cells);
bool promptUser(std::unordered_map<std::string, User>& users, std::string& id);
bool displayInterface(std::unordered_map<std::string, User>& users, const std::string& id);

int main() {

    // https://www.geeksforgeeks.org/how-to-read-data-from-csv-file-to-a-2d-array-in-cpp/
    std::ifstream rFile("assets/Info.csv");

    if (!rFile.is_open()) {
        std::cout << "error in reading the csv file." << "\n";
        return 1;
    }

    std::unordered_map<std::string, User> users;
    std::string line;
    for (int row = 0; std::getline(rFile, line); row++) {
        if (row == 0)
            continue;
        // std::cout << line;
        std::string cells[7];
        parseLineCSV(line, cells);
        std::string full_name = cells[2] + ", " + cells[1];
        users[cells[0]] = User(cells[0], full_name, cells[3], std::stod(cells[4]), cells[5], std::stod(cells[6]));
    }
    rFile.close();

    std::string session_id;
    
    while (true) {
        if (!promptUser(users, session_id)) return 1;
        if (!displayInterface(users, session_id)) break;
    }

    return 0;
}

void parseLineCSV(const std::string& line, std::string* cells) {
    bool insideQuotes = false;
    std::string cell = "";
    int col = 0;

    for (int i = 0; i < line.length(); i++) {
        if (line[i] == '"') {
            insideQuotes = !insideQuotes; // Toggle the insideQuotes flag
        } else if (line[i] == ',' && !insideQuotes) {
            cells[col] = cell; // End of a field
            cell.clear();
            col++;
        }
        else {
            if (line[i] != ',')
                cell += line[i]; // Append character to the current field
        }
    }
    if (!cell.empty()) {
        cells[col] = cell; // Add the last field
    }
}

bool promptUser(std::unordered_map<std::string, User>& users, std::string& id) {
    std::string input;
    std::cout << "User ID: ";
    std::getline(std::cin, input);
    id = input;

    if (users.find(id) == users.end()) {
        std::cout << "user not found." << "\n";
        return false;
    }

    unsigned short int attempts = 0;
    do {
        if (attempts > 2) {
            std::cout << "Too many incorrect attempts. Try again later.\n";
            return false;
        } else if (attempts > 0) {
            std::cout << "Wrong password. Try again.\n";
        }
        std::cout << "Enter your password: ";
        std::getline(std::cin, input);
        attempts++;
    } while (input != users[id].password);

    return true;
}

bool displayInterface(std::unordered_map<std::string, User>& users, const std::string& id) {
    std::cout << "\nAccess Granted!\n\n";
    std::cout << "|| Account ID: " << users[id].uID << "\n";
    std::cout << "|| Name: " << users[id].name << "\n";
    std::cout << "|| Course: " << users[id].course << "\n";
    std::cout << "|| Bank Account Balance: " << users[id].balance << "\n";

    std::cout << "\nChoose a transaction.\n";
    std::cout << "1) Pay\n2) Switch Account\n3) Cancel\n";
    
    int choice = 0; 
    std::cin >> choice;
    std::cin.ignore();
    switch (choice) {
        case 1: {
            std::cout << "\n|| Payment Due: " << users[id].duePayment << "\n";
            std::cout << "Enter your amount: ";
            double tmp = 0;
            double due_payment = users[id].duePayment;
            std::cin >> tmp;
            std::cin.ignore();
            if (tmp < due_payment) {
                users[id].duePayment -= tmp;
                users[id].balance -= tmp;
            } else {
                users[id].duePayment = 0;
                users[id].balance -= due_payment;
            }
            std::cout << "Payment Completed!\n";
            return displayInterface(users, id);
            break;
        }
        case 2: {
            return true;
            break;
        }
        default:
            std::cout << "Session Ended.\n";
            return false;
            break;
    }
}