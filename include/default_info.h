//
// Created by alexey on 11/20/16.
//

#ifndef HW1_DEFAULT_INFO_H
#define HW1_DEFAULT_INFO_H

#include <string>

class default_info {
    //for serialisation
protected:
    std::string name, object_id;
public:
    default_info();

    default_info(const std::string &get_name);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getObject_id() const;

    void setObject_id(const std::string &object_id);
};


#endif //HW1_DEFAULT_INFO_H