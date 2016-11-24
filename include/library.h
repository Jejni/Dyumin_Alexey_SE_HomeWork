//
// Created by alexey on 11/20/16.
//

#ifndef HW1_LIBRARY_H
#define HW1_LIBRARY_H


#include "default_info.h"
#include "book.h"
#include "user.h"
#include <list>

class library : public default_info {
    std::list<book> books;
    std::list<user> users;
public:
    library();

    bool can_give(user &get_user, const book &get_book) const;

    bool check_user_date(user &get_user) const;

    bool check_book_date(const book &get_book) const;

    bool check_number_of_books(const user &get_user) const;

    bool check_book_free(const book &get_book) const;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(books, users); // serialize things by passing them to the archive
    }

    std::list<book> &getBooks();

    std::list<user> &getUsers();
};


#endif //HW1_LIBRARY_H
