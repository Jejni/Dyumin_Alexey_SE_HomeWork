//
// Created by alexey on 11/20/16.
//

#ifndef HW1_USER_H
#define HW1_USER_H

#include "DefaultInfo.h"
#include <vector>

class User : public DefaultInfo {
    std::vector<std::string> owns;
public:
    User();

    User(const std::string &get_name);

    void add_book(const std::string &get_book_id);

    void return_book(const std::string &get_book_id);

    int get_number_of_books() const;

    template<class Archive>
    void serialize(Archive &archive) {
        archive(owns, name, object_id); // serialize things by passing them to the archive
    }

    const std::vector<std::string> &getOwns() const;

    void setOwns(const std::vector<std::string> &owns);
};


#endif //HW1_USER_H
