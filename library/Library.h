//
// Created by alexey on 11/20/16.
//

#ifndef HW1_LIBRARY_H
#define HW1_LIBRARY_H

#include "types/DefaultInfo.h"
#include "types/Book.h"
#include "types/User.h"
#include "database/Database.h"

class Library : public DefaultInfo {
protected:
    Database *dataBase;
public:
    Library(int);

    bool can_give(User &get_user, const Book &get_book) const;

    bool check_user_date(User &get_user) const;

    bool check_book_date(const Book &get_book) const;

    bool check_number_of_books(const User &get_user) const;

    bool check_book_free(const Book &get_book) const;

    std::list<Book> &getBooks();

    std::list<User> &getUsers();

    void read_from_disc();

    void write_on_disc();

    bool give_book(User &user, Book &book);

    bool check_book_index(int get_index);

    bool check_user_index(int get_index);

    bool return_book(Book &book);

    Book &get_book_ref(const std::string &id);

    User &get_user_ref(int index);

    User &get_user_ref(const std::string &id);
};

#endif //HW1_LIBRARY_H
