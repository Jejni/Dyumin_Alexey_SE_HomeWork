#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

class initial {
public:
    string name;

    void set_name(const string &get_name) { name = get_name; }

    const string get_name() const &{ return name; }
};

class book;

class user : public initial {
    vector<book *> owns;
public:
    user(const string &get_name) {
        set_name(get_name);
    }

    void add_book(book *get_book) { owns.push_back(get_book); }

    void return_book(book *get_book) {
        for (auto it = owns.begin(); it != owns.end(); it++) {
            if (*it == get_book) {
                owns.erase(it);
                break;
            }
        }
    }

    int get_books_number() const { return owns.size(); }
};

class book : public initial {
    user *owner;
    time_t owned_date;
public:
    book(const string &get_name) {
        set_name(get_name);
        assign_owner(0, 0);
    }

    user *get_owner() const { return owner; };

    const string get_owner_name() const &{
        if (is_free()) return "free";
        else {
            //string mm = owner->get_name();
            return owner->name;
        }
    }

    void set_owned_date(const time_t &get_time) { owned_date = get_time; }

    const string get_owned_date() const &{
        int max_size = 11;
        char *temp = new char(max_size);
        strftime(temp, max_size, "%e.%m.%Y", localtime(&owned_date));
        string temp_str = temp;
        return temp_str;
    }

    void assign_owner(user *get_user, const time_t &get_time) {
        owner = get_user;
        owned_date = get_time;
    }

    void assign_owner(user *get_user) { owner = get_user; }

    void free() {
        owner = 0;
        owned_date = 0;
    }

    bool is_free() const { return owner == 0; }
};


class library : public initial {
protected:
    list<book> books;
    list<user> users;
public:
    bool can_give(const user *get_user, const book *get_book) const {
        return (get_user->get_books_number() < 3) && get_book->is_free();
    }
};


class controller : private library {
public:
    controller() {
        read_from_disc();
    }

    bool return_book(int book_index){
        if (book_index > books.size()) return false;

        books[book_index]
    }

    bool give_book(user *get_user, book *get_book) const {
        if (can_give(get_user, get_book)) {
            get_user->add_book(get_book);
            get_book->assign_owner(get_user, time(0));
            return true;
        }
        return false;
    }

    void read_from_disc(string path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data.txt") {
        fstream file(path, ios::in);
        if (!file.is_open()) {
            cout << "Error, can't open file!";
            return;
        }
        for (string line; getline(file, line);) {

            if (line.length() == 0) continue;

            string type = line.substr(0, line.find("="));
            string get_data = line.substr(line.find("=") + 1, line.length());

            if (type == "user") {
                user temp(get_data);
                users.push_back(temp);
            } else if (type == "book") {
                book temp(get_data);
                books.push_back(temp);
                books.back().assign_owner(&users.back());
                users.back().add_book(&books.back());
            } else if (type == "time") {
                stringstream ss(get_data);
                vector<string> temp;
                string temp_str;
                while (getline(ss, temp_str, '.')) temp.push_back(temp_str);

                struct tm *mtm = new tm;
                mktime(mtm);
                mtm->tm_mday = stoi(temp[0]);
                mtm->tm_mon = stoi(temp[1]) - 1;
                mtm->tm_year = stoi(temp[2]) - 1900;
                books.back().set_owned_date(mktime(mtm));
            } else if (type == "book_free_book") {
                book temp(get_data);
                books.push_back(temp);
            }
        }
        file.close();
    }

    void write_on_disc(string path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data.txt") {
        fstream file(path, ios::out);
        if (!file.is_open()) {
            cout << "Error, can't write file!";
            return;
        }

        for (auto it = users.begin(); it != users.end(); it++) {
            file << "\nuser=" << (*it).get_name() << "\n";
            for (auto it2 = books.begin(); it2 != books.end(); it2++)
                if ((*it2).get_owner() == &*it) {

                    file << "book=" << (*it2).get_name() << "\n" << "time=" << (*it2).get_owned_date() << "\n";
                }
        }

        for (auto it = books.begin(); it != books.end(); it++)
            if ((*it).is_free())
                file << "\nbook_free_book=" << (*it).get_name();
        file.close();
    }

    virtual void show_users() const {
        int i = 1;
        for (auto it = users.begin(); it != users.end(); it++, i++)
            cout << i << ": " << (*it).get_name() << " (" << (*it).get_books_number() << ")" << endl;
    }

    virtual void show_books() const {
        int i = 1;
        for (auto it = books.begin(); it != books.end(); it++, i++)
            cout << i << ": " << (*it).get_name() << " (" << (*it).get_owner_name() << ")" << endl;
    }
};

class view : private controller {
public:
    view() : controller() {};

    void show_users() const { controller::show_users(); }

    void show_books() const { controller::show_books(); }

    string books_select_menu() const {
        string book_number;
        cout << "\nBook number: ";
        cin >> book_number;

        if (book_number)

        string choice;
        cout << "\nBook number: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                show_books();
                break;
            }
            case '2': {

                break;
            }
            case 'q': {
                return "books_menu_back";
            }
            default: {
                cout << "Error!\n";
                return books_menu();
            }
        }
        return "books_menu";
    }

    string books_menu() const {
        string choice;
        cout << "\nList books - 1, Select book - 2, back - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                show_books();
                break;
            }
            case '2': {

                break;
            }
            case 'q': {
                return "books_menu_back";
            }
            default: {
                cout << "Error!\n";
                return books_menu();
            }
        }
        return "books_menu";
    }

    string users_menu() const {
        string choice;
        cout << "\nList users - 1, Select user - 2, back - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                show_users();
                break;
            }
            case '2': {

                break;
            }
            case 'q': {
                return "users_menu_back";
            }
            default: {
                cout << "Error!\n";
                return users_menu();
            }
        }
        return "users_menu";
    }

    string main_menu() const {
        string choice;
        cout << "\nUsers - 1, Books - 2, quit - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                return "users_menu";
            }
            case '2': {
                return "books_menu";
            }
            case 'q': {
                return "quit";
            }
            default: {
                cout << "Error!\n";
                return main_menu();
            }
        }
    }

    void start_routine() {
        string menu = "main";
        for (;;) {
            if (menu == "main" || menu == "users_menu_back" || menu == "books_menu_back") menu = main_menu();
            else if (menu == "users_menu") menu = users_menu();
            else if (menu == "books_menu") menu = books_menu();
            else if (menu == "quit") {
                write_on_disc();
                return;
            }
        }
    }
};


int main() {

    view new_view;
    new_view.start_routine();

    std::cout << std::endl;
    return 0;
}