#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <fstream>
#include <sstream>
#include <list>
#include <cereal/types/list.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/archives/portable_binary.hpp>

using namespace std;

class default_info {
    //for serialisation
protected:
    string name, object_id;
public:
    default_info() {
        name = "default_name";
        object_id = "";
    }

    default_info(const string &get_name) { set_name(get_name); }

    void set_name(const string &get_name) { name = get_name; }

    void set_id(const string &get_id) { object_id = get_id; }

    const string &get_name() const { return name; }

    const string &get_id() const { return object_id; }
};

class book;

class user : public default_info {
    vector<string> owns;
public:
    user() {}

    user(const string &get_name) : default_info(get_name) {}

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

    int get_number_of_books() const { return owns.size(); }

    template<class Archive>
    void serialize(Archive &archive) {
        archive(owns, name, object_id); // serialize things by passing them to the archive
    }
};

class book : public default_info {
    string owner_id;
    time_t owned_date;
public:
    book() {}

    template<class Archive>
    void serialize(Archive &archive) {
        archive(owner_id, owned_date, name, object_id); // serialize things by passing them to the archive
    }

    book(const string get_name) : default_info(get_name) { assign_owner("", 0); }

    string get_owner() const {
        if (is_free()) return "free";
        else return owner_id;
    }

    void set_owned_date(const time_t &get_time) { owned_date = get_time; }

    const time_t &get_owned_date_time_t() const { return owned_date; }

    string get_owned_date() const {
        int max_size = 11;
        char *temp = new char(max_size);
        strftime(temp, max_size, "%e.%m.%Y", localtime(&owned_date));
        string temp_str = temp;
        return temp_str;
    }

    void assign_owner(const string &get_user_id, const time_t &get_time) {
        owner_id = get_user_id;
        owned_date = get_time;
    }

    void assign_owner(const string &get_user_id) { owner_id = get_user_id; }

    void free() {
        owner_id = "";
        owned_date = 0;
    }

    bool is_free() const { return owner_id.length() == 0; }
};


class library : public default_info {
    list <book> books;
    list <user> users;
public:
    library() : default_info("default_library") {}

    bool can_give(user &get_user, const book &get_book) const {
        return check_user_date(get_user) && check_number_of_books(get_user) && check_book_free(get_book);
    }

    bool check_user_date(user &get_user) const {
        for (auto owned_book_id = get_user.get_owns().begin();
             owned_book_id != get_user.get_owns().end(); owned_book_id++) {
            for (auto book = books.begin(); book != books.end(); book++) {
                if (book->get_id() == *owned_book_id && !check_book_date(*book)) return check_book_date(*book);
            }
        }
        return true;
    }

    bool check_book_date(const book &get_book) const {
        //2628002.88 - число секунд в месяце
        return (difftime(time(0), get_book.get_owned_date_time_t()) / 2628002.88 < 1);
    }

    bool check_number_of_books(const user &get_user) const {
        return get_user.get_number_of_books() < 3;
    }

    bool check_book_free(const book &get_book) const {
        return get_book.is_free();
    }

    list <book> &get_books() { return books; }

    list <user> &get_users() { return users; }

    template<class Archive>
    void serialize(Archive &archive) {
        archive(books, users); // serialize things by passing them to the archive
    }
};


class controller {
    library lib;
public:
    controller() { read_from_disc(); }


    void serial(string path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data.bin") {
        ofstream file(path, ios::binary);
        if (!file.is_open()) {
            cout << "Error, can't write file!";
            return;
        }
        cereal::PortableBinaryOutputArchive oarchive(file); // Create an output archive
        oarchive(lib); // Write the data to the archive
        file.close();
    }

    void usserial(string path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data.bin") {
        ifstream file(path, ios::binary);
        if (!file.is_open()) {
            cout << "Error, can't write file!";
            return;
        }
        cereal::PortableBinaryInputArchive iarchive(file); // Create an input archive
        iarchive(lib); // Read the data from the archive
        file.close();
    }

    bool check_book_index(int get_index) { return get_index > lib.get_books().size() - 1; }

    bool check_user_index(int get_index) { return get_index > lib.get_users().size() - 1; }

    bool give_book(int book_number, int user_number) {
        book_number -= 1;
        user_number -= 1;
        if (check_book_index(book_number) || check_user_index(user_number)) return false;
        auto book_it = lib.get_books().begin();
        auto user_it = lib.get_users().begin();
        for (int i = 0; (i < book_number) && (book_it != lib.get_books().end()); i++, book_it++);
        for (int i = 0; (i < user_number) && (user_it != lib.get_users().end()); i++, user_it++);

        if (lib.can_give(*user_it, *book_it)) {
            user_it->add_book(book_it->get_id());
            book_it->assign_owner(user_it->get_id(), time(0));
        } else return false;
        write_on_disc();
        return true;
    }

    bool return_book(int book_number) {
        book_number -= 1;
        if (check_book_index(book_number)) return false;
        auto it = lib.get_books().begin();
        for (int i = 0; (i < book_number) && (it != lib.get_books().end()); i++, it++);
        if (it->is_free()) return false;
        get_user_ref(it->get_owner()).return_book(it->get_id());
        it->free();
        write_on_disc();
        return true;
    }

    bool return_book(string book_id) {
        auto it = lib.get_books().begin();
        for (; it->get_id() != book_id; it++);
        if (it->is_free()) return false;
        get_user_ref(it->get_owner()).return_book(it->get_id());
        it->free();
        write_on_disc();
        return true;
    }

    bool return_book(string u_number, string b_number) {
        int user_index = stoi(u_number) - 1, book_index = stoi(b_number) - 1;
        if (check_user_index(user_index)) return false;
        int check = get_user_ref(user_index).get_owns().size() - 1;
        if (check < book_index) return false;
        b_number = get_user_ref(user_index).get_owns()[book_index];
        return return_book(b_number);
    }


    void read_from_disc(string path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data.txt") {
        usserial();
//        fstream file(path, ios::in);
//        if (!file.is_open()) {
//            cout << "Error, can't open file!";
//            return;
//        }
//        for (string line; getline(file, line);) {
//
//            if (line.length() == 0) continue;
//
//            string type = line.substr(0, line.find("="));
//            string get_data = line.substr(line.find("=") + 1, line.length());
//
//            if (type == "user") {
//                user temp(get_data);
//                lib.get_users().push_back(temp);
//            } else if (type == "book") {
//                book temp(get_data);
//                lib.get_books().push_back(temp);
//                lib.get_books().back().assign_owner(lib.get_users().back().get_id());
//            } else if (type == "user_id") {
//                lib.get_users().back().set_id(get_data);
//            } else if (type == "book_id") {
//                lib.get_books().back().set_id(get_data);
//                lib.get_users().back().add_book(lib.get_books().back().get_id());
//            } else if (type == "book_free_book_id") {
//                lib.get_books().back().set_id(get_data);
//            } else if (type == "time") {
//                stringstream ss(get_data);
//                vector<string> temp;
//                string temp_str;
//                while (getline(ss, temp_str, '.')) temp.push_back(temp_str);
//
//                struct tm *mtm = new tm;
//                mktime(mtm);
//                mtm->tm_mday = stoi(temp[0]);
//                mtm->tm_mon = stoi(temp[1]) - 1;
//                mtm->tm_year = stoi(temp[2]) - 1900;
//                lib.get_books().back().set_owned_date(mktime(mtm));
//            } else if (type == "book_free_book") {
//                book temp(get_data);
//                lib.get_books().push_back(temp);
//            }
//        }
//        file.close();
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
                file << "\nbook_free_book=" << it->get_name() << "\nbook_free_book_id=" << it->get_id();
        file.close();
        serial();
    }

    void show_users() {
        int i = 1;
        for (auto it = lib.get_users().begin(); it != lib.get_users().end(); it++, i++)
            cout << i << ": " << it->get_name() << " (" << it->get_number_of_books() << ")" << endl;
    }

    void show_bad_users() {
        int i = 1;
        for (auto it = lib.get_users().begin(); it != lib.get_users().end(); it++, i++) {
            if (!lib.check_user_date(*it)) {
                cout << i << ": " << it->get_name() << " (" << it->get_number_of_books() << ")" << endl;
                for (auto b_it = it->get_owns().begin(); b_it != it->get_owns().end(); b_it++) {
                    book &book = get_book_ref(*b_it);
                    if (!lib.check_book_date(book))
                        cout << "\t" << book.get_name() << " " << book.get_owned_date() << endl;
                }
            }
        }
    }

    void show_user_books(string u_index) {
        int user_undex = stoi(u_index) - 1;
        if (check_user_index(user_undex)) return;

        auto user_it = lib.get_users().begin();
        for (int i = 0; i < user_undex; i++, user_it++);
        int i = 1;
        for (auto book_it = lib.get_books().begin(); book_it != lib.get_books().end(); book_it++, i++) {
            if (book_it->get_owner() == user_it->get_id())
                cout << i << ": " << book_it->get_name() << endl;
        }
    }


    user &get_user_ref(const string &id) {
        for (auto it = lib.get_users().begin(); it != lib.get_users().end(); it++) if (it->get_id() == id) return *it;
        user *temp = new user("free");
        return *temp;
    }

    user &get_user_ref(int index) {
        if (check_user_index(index)) {
            user *temp = new user("free");
            return *temp;
        }
        auto it = lib.get_users().begin();
        for (int i = 0; i < index; i++, it++);
        return *it;
    }

    book &get_book_ref(const string &id) {
        for (auto it = lib.get_books().begin(); it != lib.get_books().end(); it++) if (it->get_id() == id) return *it;
        book *temp = new book("free");
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

    /*
   Пользователи с просроченными книгами.
   */
    void show_bad_users() { con.show_bad_users(); }

    void show_books() { con.show_books(); }

    /*
   Перегрузка, чтобы не вводить зановво номер пользователя.
   */
    string user_select_menu(string user_number) {
        string choice;
        cout << "\nActions: list user's books - 1, free book - 2, add book - 3, back - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                con.show_user_books(user_number);
                break;
            }
            case '2': {
                string book_number;
                cout << "Book number: ";
                cin >> book_number;

                if (con.return_book(user_number, book_number)) cout << "Done!\n";
                else cout << "Error!\n";
                return user_select_menu(user_number);
            }
            case '3': {
                show_books();
                string book_number;
                cout << "Book number: ";
                cin >> book_number;

                if (con.give_book(stoi(book_number), stoi(user_number))) cout << "Done!\n";
                else cout << "Error!\n";
                break;
            }
            case 'q': {
                return "user_select_menu_back";
            }

            default: {
                cout << "Error!\n";
                return user_select_menu(user_number);
            }
        }
        return user_select_menu(user_number);
    }

    string user_select_menu() {
        string user_number;
        cout << "User number: ";
        cin >> user_number;

        string choice;
        cout << "\nActions: list user's books - 1, free book - 2, add book - 3, back - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                con.show_user_books(user_number);
                break;
            }
            case '2': {
                string book_number;
                cout << "Book number: ";
                cin >> book_number;

                if (con.return_book(user_number, book_number)) cout << "Done!\n";
                else cout << "Error!\n";
                return user_select_menu(user_number);
            }
            case '3': {
                show_books();
                string book_number;
                cout << "Book number: ";
                cin >> book_number;

                if (con.give_book(stoi(book_number), stoi(user_number))) cout << "Done!\n";
                else cout << "Error!\n";
                break;
            }
            case 'q': {
                return "user_select_menu_back";
            }

            default: {
                cout << "Error!\n";
                return user_select_menu(user_number);
            }
        }
        return user_select_menu(user_number);
    }

    string books_give_menu(int book_index) {
        show_users();

        string choice;
        cout << "Give to (back - q): ";
        cin >> choice;

        if (choice == "q") return "books_give_menu_back";

        if (con.give_book(book_index, stoi(choice))) cout << "Done!\n";
        else cout << "Error!\n";
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
        string book_number;
        cout << "Book number: ";
        cin >> book_number;

        string choice;
        cout << "\nActions: free - 1, give - 2, back - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                if (con.return_book(stoi(book_number))) cout << "Done!\n";
                else cout << "Error!\n";
                return "books_menu";
            }
            case '2': {
                return books_give_menu(stoi(book_number));
            }
            case 'q': {
                return "books_select_menu_back";
            }
            default: {
                cout << "Error!\n";
                return books_select_menu(stoi(book_number));
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
        cout << "\nList users - 1, Select user - 2, List bad users - 3, back - q: ";
        cin >> choice;

        switch (*choice.c_str()) {
            case '1': {
                show_users();
                break;
            }
            case '2': {
                return user_select_menu();
            }
            case '3': {
                show_bad_users();
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

    /*
     Бесконечный цикл меню с условием выхода
     Каждое из подменю возвращает строку для мониторинга местонахождения пользователя     
     */
    void start_routine() {

        string menu = "main";
        for (;;) {
            if (menu == "main" || menu == "users_menu_back" || menu == "books_menu_back") menu = main_menu();
            else if (menu == "users_menu" || menu == "user_select_menu_back") menu = users_menu();
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