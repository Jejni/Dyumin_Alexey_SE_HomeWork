#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>

using namespace std;

class obj_name {
    string name, obj_id;
public:
    obj_name() {
        name = "default_name";
        obj_id = "";
    }

    obj_name(const string &get_name) { set_name(get_name); }

    void set_name(const string &get_name) { name = get_name; }

    void set_id(const string &get_id) { obj_id = get_id; }

    const string &get_name() const { return name; }

    const string &get_id() const { return obj_id; }
};

class book;

class user : public obj_name {
    vector<string> owns;
public:

    user(const string &get_name) : obj_name(get_name) {}

    void add_book(const string &get_book_id) { owns.push_back(get_book_id); }

    vector<string> &get_owns() { return owns; }

    void return_book(const string &get_book_id) {
        for (auto it = owns.begin(); it != owns.end(); it++) {
            if (*it == get_book_id) {
                owns.erase(it);
                return;
            }
        }
    }

    int get_books_number() const { return owns.size(); }
};

class book : public obj_name {
    string owner;
    time_t owned_date;
public:

    book(const string get_name) : obj_name(get_name) { assign_owner("", 0); }

    const string get_owner() const {
        if (is_free()) return "free";
        else { return owner; }
    }

    void set_owned_date(const time_t &get_time) { owned_date = get_time; }

    const time_t &get_owned_date_time_t() const { return owned_date; }

    const string get_owned_date() const {
        int max_size = 11;
        char *temp = new char(max_size);
        strftime(temp, max_size, "%e.%m.%Y", localtime(&owned_date));
        string temp_str = temp;
        return temp_str;
    }

    void assign_owner(const string &get_user, const time_t &get_time) {
        owner = get_user;
        owned_date = get_time;
    }

    void assign_owner(const string &get_user) { owner = get_user; }

    void free() {
        owner = "";
        owned_date = 0;
    }

    bool is_free() const { return owner.length() == 0; }
};


class library : public obj_name {
    list <book> books;
    list <user> users;
public:
    library() : obj_name("default_library") {}

    bool can_give(user &get_user, const book &get_book) const {
        return check_date(get_user) && check_book_number(get_user) && check_book_free(get_book);
    }

    bool check_date(user &get_user) const {
        for (auto owns_id = get_user.get_owns().begin(); owns_id != get_user.get_owns().end(); owns_id++) {
            for (auto book = books.begin(); book != books.end(); book++) {
                if (book->get_owner() == get_user.get_id()) {
                    struct tm *current_time = new tm;
                    time_t raw_time;
                    time(&raw_time);
                    current_time = localtime(&raw_time);
                    struct tm *book_time = new tm;
                    book_time = localtime(&book->get_owned_date_time_t());
                    cout << "Curr=" << current_time->tm_mon << " saved=" << book_time->tm_mon << endl;
                    if (current_time->tm_mon - book_time->tm_mon > 0) return false;
                }
            }
        }
        return true;
    }

    bool check_book_number(const user &get_user) const {
        return get_user.get_books_number() < 3;
    }

    bool check_book_free(const book &get_book) const {
        return get_book.is_free();
    }

    list <book> &get_books() { return books; }

    list <user> &get_users() { return users; }
};


class controller {
    library lib;
public:

    controller() { read_from_disc(); }

    bool bad_book_number(int get_index) { return get_index > lib.get_books().size(); }

    bool bad_user_number(int get_index) { return get_index > lib.get_users().size(); }

    bool give_book(int book_index, int user_index) {
        if (bad_book_number(book_index) || bad_user_number(user_index)) return false;
        book_index -= 1;
        user_index -= 1;

        auto book_it = lib.get_books().begin();
        auto user_it = lib.get_users().begin();
        for (int i = 0; (i < book_index) && (book_it != lib.get_books().end()); i++, book_it++);
        for (int i = 0; (i < user_index) && (user_it != lib.get_users().end()); i++, user_it++);

        if (lib.can_give(*user_it, *book_it)) {
            user_it->add_book(book_it->get_id());
            book_it->assign_owner(user_it->get_id(), time(0));
        } else return false;
        write_on_disc();
        return true;
    }

    bool return_book(int book_index) {
        if (book_index > lib.get_books().size()) return false;

        book_index -= 1;

        auto it = lib.get_books().begin();
        for (int i = 0; (i < book_index) && (it != lib.get_books().end()); i++, it++);
        if (it->is_free()) return false;
        get_user_ref(it->get_owner()).return_book(it->get_id());
        it->free();
        write_on_disc();
        return true;
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
                lib.get_users().push_back(temp);
            } else if (type == "book") {
                book temp(get_data);
                lib.get_books().push_back(temp);
                lib.get_books().back().assign_owner(lib.get_users().back().get_id());
                lib.get_users().back().add_book(lib.get_books().back().get_id());
            } else if (type == "user_id") {
                lib.get_users().back().set_id(get_data);
            } else if (type == "book_id") {
                lib.get_books().back().set_id(get_data);
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
                lib.get_books().back().set_owned_date(mktime(mtm));
            } else if (type == "book_free_book") {
                book temp(get_data);
                lib.get_books().push_back(temp);
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

        for (auto it = lib.get_users().begin(); it != lib.get_users().end(); it++) {
            file << "\nuser=" << it->get_name() << "\nuser_id=" << it->get_id() << "\n";
            for (auto it2 = lib.get_books().begin(); it2 != lib.get_books().end(); it2++)
                if (it2->get_owner() == it->get_id()) {
                    file << "book=" << it2->get_name() << "\nbook_id=" << it2->get_id() << "\ntime="
                         << it2->get_owned_date() << "\n";
                }
        }

        for (auto it = lib.get_books().begin(); it != lib.get_books().end(); it++)
            if (it->is_free())
                file << "\nbook_free_book=" << it->get_name() << "\nbook_id=" << it->get_id();
        file.close();
    }

    void show_users() {
        int i = 1;
        for (auto it = lib.get_users().begin(); it != lib.get_users().end(); it++, i++)
            cout << i << ": " << it->get_name() << " (" << it->get_books_number() << ")" << endl;
    }

    user &get_user_ref(const string &id) {
        for (auto it = lib.get_users().begin(); it != lib.get_users().end(); it++) if (it->get_id() == id) return *it;
        user *temp = new user("free");
        return *temp;
    }

    void show_books() {
        int i = 1;
        for (auto it = lib.get_books().begin(); it != lib.get_books().end(); it++, i++)
            cout << i << ": " << it->get_name() << " (" << get_user_ref(it->get_owner()).get_name() << ")" << endl;
    }

};

class view {
    controller con;
public:

    void show_users() { con.show_users(); }

    void show_books() { con.show_books(); }

    string books_give_menu(int book_index) {
        show_users();

        string choice;
        cout << "Give to (back - q): ";
        cin >> choice;

        if (choice == "q") return "books_give_menu_back";

        if (con.give_book(book_index, stoi(choice))) {
            cout << "Done!\n";
        } else cout << "Error!\n";
        return "books_give_menu_back";
    }

    string books_select_menu(int book_number) {

        string choice;
        cout << "\nActions: free - 1, give - 2, back - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                if (con.return_book(book_number)) cout << "Done!\n";
                else cout << "Error!\n";
                return "books_menu";
            }
            case '2': {
                return books_give_menu(book_number);
            }
            case 'q': {
                return "books_select_menu_back";
            }
            default: {
                cout << "Error!\n";
                return books_select_menu();
            }
        }
    }

    string books_select_menu() {
        int book_number;
        cout << "Book number: ";
        cin >> book_number;

        string choice;
        cout << "\nActions: free - 1, give - 2, back - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                if (con.return_book(book_number)) cout << "Done!\n";
                else cout << "Error!\n";
                return "books_menu";
            }
            case '2': {
                return books_give_menu(book_number);
            }
            case 'q': {
                return "books_select_menu_back";
            }
            default: {
                cout << "Error!\n";
                return books_select_menu(book_number);
            }
        }
    }

    string books_menu() {
        string choice;
        cout << "\nList books - 1, Select book - 2, back - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                show_books();
                return "books_menu";
            }
            case '2': {
                return books_select_menu();
            }
            case 'q': {
                return "books_menu_back";
            }
            default: {
                cout << "Error!\n";
                return books_menu();
            }
        }
    }

    string users_menu() {
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
            else if (menu == "books_menu" || menu == "books_select_menu_back" || menu == "books_give_menu_back")
                menu = books_menu();
            else if (menu == "quit") {
                con.write_on_disc();
                return;
            }
        }
    }
};


int main() {

    view nv;
    nv.start_routine();

    std::cout << std::endl;
    return 0;
}