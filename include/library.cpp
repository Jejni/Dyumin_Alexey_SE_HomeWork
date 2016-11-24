//
// Created by alexey on 11/20/16.
//

#include "library.h"

library::library() : default_info("default_library") {}

bool library::can_give(user &get_user, const book &get_book) const {
    return check_user_date(get_user) && check_number_of_books(get_user) && check_book_free(get_book);
}

bool library::check_user_date(user &get_user) const {
    for (auto owned_book_id = get_user.getOwns().begin();
         owned_book_id != get_user.getOwns().end(); owned_book_id++) {
        for (auto book = books.begin(); book != books.end(); book++) {
            if (book->getObject_id() == *owned_book_id && !check_book_date(*book)) return check_book_date(*book);
        }
    }
    return true;
}

bool library::check_book_date(const book &get_book) const {
    //2628002.88 - число секунд в месяце
    return (difftime(time(0), get_book.getOwned_date()) / 2628002.88 < 1);
}

bool library::check_number_of_books(const user &get_user) const {
    return get_user.get_number_of_books() < 3;
}

bool library::check_book_free(const book &get_book) const {
    return get_book.is_free();
}

std::list<book> &library::getBooks() {
    return books;
}

std::list<user> &library::getUsers() {
    return users;
}
