//
// Created by alexey on 11/20/16.
//

#ifndef HW1_BOOK_H
#define HW1_BOOK_H

#include <time.h>
#include <string>
#include "DefaultInfo.h"

class Book : public DefaultInfo {
    std::string owner_id, free_str = "free";
    time_t owned_date;
public:
    Book();

    template<class Archive>
    void serialize(Archive &archive) {
        archive(owner_id, owned_date, name, object_id); // serialize things by passing them to the archive
    }

    Book(const std::string get_name);

    void free();

    bool is_free() const;

    const std::string &getOwner_id() const;

    void setOwner_id(const std::string &owner_id);

    void setOwner_id(const std::string &owner_id, const time_t &owned_date);

    time_t getOwned_date() const;

    std::string getOwned_date_Str() const;

    void setOwned_date(time_t owned_date);
};

#endif //HW1_BOOK_H
