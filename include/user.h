//
// Created by alexey on 11/20/16.
//

#ifndef HW1_USER_H
#define HW1_USER_H

#include "default_info.h"
#include <vector>

class user : public default_info {
    std::vector<std::string> owns;
public:
    user();

    user(const std::string &get_name);

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
