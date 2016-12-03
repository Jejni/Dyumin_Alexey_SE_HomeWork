//
// Created by alexey on 11/20/16.
//

#include "Library.h"
#include "database/DatabaseTextFile.h"
#include "database/DatabaseSerial.h"
#include <string>

Library::Library(int type) : DefaultInfo("default_library") {
    if (type == 1) {
        std::string path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data/data.txt";
        dataBase = new DatabaseTextFile(path);
    } else if (type == 2) {
        std::string path = "/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data/data.bin";
        dataBase = new DatabaseSerial(path);
    }
//    Database *copy = new DatabaseSerial("/home/alexey/Dropbox/SafeBoard/Software_Engineering/Hw1/data/data.bin");
//    *copy = *dataBase;
//    copy->write_on_disc();
}

bool Library::can_give(User &get_user, const Book &get_book) const {
    return check_user_date(get_user) && check_number_of_books(get_user) && check_book_free(get_book);
}

bool Library::check_user_date(User &get_user) const {
    for (auto owned_book_id = get_user.getOwns().begin();
         owned_book_id != get_user.getOwns().end(); owned_book_id++) {
        for (auto book = dataBase->getBooks().begin(); book != dataBase->getBooks().end(); book++) {
            if (book->getObject_id() == *owned_book_id && !check_book_date(*book)) return check_book_date(*book);
        }
    }
    return true;
}

bool Library::check_book_date(const Book &get_book) const {
    //2628002.88 - число секунд в месяце
    return (difftime(time(0), get_book.getOwned_date()) / 2628002.88 < 1);
}

bool Library::check_number_of_books(const User &get_user) const {
    return get_user.get_number_of_books() < 3;
}

bool Library::check_book_free(const Book &get_book) const {
    return get_book.is_free();
}

std::list<Book> &Library::getBooks() {
    return dataBase->getBooks();
}

std::list<User> &Library::getUsers() {
    return dataBase->getUsers();
}

void Library::read_from_disc() {
    dataBase->read_from_disc();
}

void Library::write_on_disc() {
    dataBase->write_on_disc();
}
