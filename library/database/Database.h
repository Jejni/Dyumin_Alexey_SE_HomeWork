//
// Created by alexey on 12/3/16.
//

#ifndef HW1_DATA_BASE_H
#define HW1_DATA_BASE_H

#include "types/Book.h"
#include "types/User.h"
#include <list>
#include <string>

class Database {
protected:
    std::string path;
    std::list<Book> books;
    std::list<User> users;
public:
    Database(const std::string &_path) {
        path = _path;
    }

    std::list<Book> &getBooks() {
        return books;
    }

    std::list<User> &getUsers() {
        return users;
    }

    void operator=(const Database &D) {
        users = D.users;
        books = D.books;
    }

    bool check_book_index(int get_index) {
        return get_index > books.size() - 1;
    }

    bool check_user_index(int get_index) {
        return get_index > users.size() - 1;
    }

    virtual void read_from_disc() = 0;

    virtual void write_on_disc() = 0;
};

#endif //HW1_DATA_BASE_H
