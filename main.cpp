
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

using namespace std;

// ---------------- BOOK CLASS ----------------

class Book {
public:
    int id;
    string title;
    string author;
    bool available;

    Book(int bookId, string bookTitle, string bookAuthor, bool bookAvailable = true) {
        id = bookId;
        title = bookTitle;
        author = bookAuthor;
        available = bookAvailable;
    }
};

// ---------------- MEMBER CLASS ----------------

class Member {
public:
    int id;
    string name;

    Member(int memberId, string memberName) {
        id = memberId;
        name = memberName;
    }
};

// ---------------- LOAN CLASS ----------------

class Loan {
public:
    int bookId;
    int memberId;

    Loan(int bId, int mId) {
        bookId = bId;
        memberId = mId;
    }
};

// ---------------- LIBRARY DATA ----------------

vector<Book> books;
vector<Member> members;
vector<Loan> loans;

// ---------------- FIND BOOK ----------------

Book* findBook(int id) {
    for (Book& book : books) {
        if (book.id == id) {
            return &book;
        }
    }

    return nullptr;
}

// ---------------- FIND MEMBER ----------------

Member* findMember(int id) {
    for (Member& member : members) {
        if (member.id == id) {
            return &member;
        }
    }

    return nullptr;
}

// ---------------- SAVE DATA ----------------

void saveData() {
    ofstream file("library_data.txt");

    if (!file) {
        cout << "Error: Could not save data.\n";
        return;
    }

    for (const Book& book : books) {
        file << "BOOK|"
             << book.id << "|"
             << book.title << "|"
             << book.author << "|"
             << book.available << "\n";
    }

    for (const Member& member : members) {
        file << "MEMBER|"
             << member.id << "|"
             << member.name << "\n";
    }

    for (const Loan& loan : loans) {
        file << "LOAN|"
             << loan.bookId << "|"
             << loan.memberId << "\n";
    }

    file.close();
}

// ---------------- LOAD DATA ----------------

void loadData() {
    ifstream file("library_data.txt");

    if (!file) {
        return;
    }

    string line;

    while (getline(file, line)) {
        stringstream ss(line);

        string type;
        getline(ss, type, '|');

        if (type == "BOOK") {
            string idText;
            string title;
            string author;
            string availableText;

            getline(ss, idText, '|');
            getline(ss, title, '|');
            getline(ss, author, '|');
            getline(ss, availableText, '|');

            int id = stoi(idText);
            bool available = (availableText == "1");

            books.emplace_back(id, title, author, available);
        }
        else if (type == "MEMBER") {
            string idText;
            string name;

            getline(ss, idText, '|');
            getline(ss, name, '|');

            members.emplace_back(stoi(idText), name);
        }
        else if (type == "LOAN") {
            string bookIdText;
            string memberIdText;

            getline(ss, bookIdText, '|');
            getline(ss, memberIdText, '|');

            loans.emplace_back(stoi(bookIdText), stoi(memberIdText));
        }
    }

    file.close();
}

// ---------------- ADD BOOK ----------------

void addBook() {
    int id;
    string title;
    string author;

    cout << "\nEnter book ID: ";
    cin >> id;

    if (findBook(id) != nullptr) {
        cout << "A book with that ID already exists.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter title: ";
    getline(cin, title);

    cout << "Enter author: ";
    getline(cin, author);

    books.emplace_back(id, title, author);

    saveData();

    cout << "Book added successfully.\n";
}

// ---------------- ADD MEMBER ----------------

void addMember() {
    int id;
    string name;

    cout << "\nEnter member ID: ";
    cin >> id;

    if (findMember(id) != nullptr) {
        cout << "A member with that ID already exists.\n";
        return;
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter member name: ";
    getline(cin, name);

    members.emplace_back(id, name);

    saveData();

    cout << "Member added successfully.\n";
}

// ---------------- SEARCH BOOK ----------------

void searchBook() {
    string search;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "\nEnter title or author to search: ";
    getline(cin, search);

    bool found = false;

    for (const Book& book : books) {
        if (book.title.find(search) != string::npos ||
            book.author.find(search) != string::npos) {

            cout << "\nBook ID: " << book.id << "\n";
            cout << "Title: " << book.title << "\n";
            cout << "Author: " << book.author << "\n";
            cout << "Availability: "
                 << (book.available ? "Available" : "On loan")
                 << "\n";

            found = true;
        }
    }

    if (!found) {
        cout << "No matching books found.\n";
    }
}

// ---------------- ISSUE BOOK ----------------

void issueBook() {
    int bookId;
    int memberId;

    cout << "\nEnter book ID: ";
    cin >> bookId;

    Book* book = findBook(bookId);

    if (book == nullptr) {
        cout << "Book not found.\n";
        return;
    }

    if (!book->available) {
        cout << "That book is currently unavailable.\n";
        return;
    }

    cout << "Enter member ID: ";
    cin >> memberId;

    Member* member = findMember(memberId);

    if (member == nullptr) {
        cout << "Member not found.\n";
        return;
    }

    book->available = false;
    loans.emplace_back(bookId, memberId);

    saveData();

    cout << "Book issued successfully.\n";
}

// ---------------- RETURN BOOK ----------------

void returnBook() {
    int bookId;

    cout << "\nEnter book ID: ";
    cin >> bookId;

    Book* book = findBook(bookId);

    if (book == nullptr) {
        cout << "Book not found.\n";
        return;
    }

    if (book->available) {
        cout << "That book is already available.\n";
        return;
    }

    book->available = true;

    for (auto it = loans.begin(); it != loans.end(); ++it) {
        if (it->bookId == bookId) {
            loans.erase(it);
            break;
        }
    }

    saveData();

    cout << "Book returned successfully.\n";
}

// ---------------- LIST BOOKS ----------------

void listBooks() {
    if (books.empty()) {
        cout << "\nNo books available.\n";
        return;
    }

    cout << "\n========== BOOKS ==========\n";

    for (const Book& book : books) {
        cout << "ID: " << book.id << "\n";
        cout << "Title: " << book.title << "\n";
        cout << "Author: " << book.author << "\n";
        cout << "Status: "
             << (book.available ? "Available" : "On loan")
             << "\n";
        cout << "---------------------------\n";
    }
}

// ---------------- MAIN MENU ----------------

int main() {
    loadData();

    int choice;

    while (true) {
        cout << "\n=================================\n";
        cout << "     LIBRARY MANAGEMENT SYSTEM\n";
        cout << "=================================\n";
        cout << "1. Add Book\n";
        cout << "2. Add Member\n";
        cout << "3. List Books\n";
        cout << "4. Search Book\n";
        cout << "5. Issue Book\n";
        cout << "6. Return Book\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";

        cin >> choice;

        if (cin.fail()) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addBook();
                break;

            case 2:
                addMember();
                break;

            case 3:
                listBooks();
                break;

            case 4:
                searchBook();
                break;

            case 5:
                issueBook();
                break;

            case 6:
                returnBook();
                break;

            case 7:
                saveData();
                cout << "Goodbye!\n";
                return 0;

            default:
                cout << "Invalid choice. Please choose 1-7.\n";
        }
    }

    return 0;
}

