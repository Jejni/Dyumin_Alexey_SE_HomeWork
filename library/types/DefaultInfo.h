//
// Created by alexey on 11/20/16.
//

#ifndef HW1_DEFAULT_INFO_H
#define HW1_DEFAULT_INFO_H

#include <string>

class DefaultInfo {
    //for serialisation
protected:
    std::string name, object_id;
public:
    DefaultInfo();

    DefaultInfo(const std::string &get_name);

    const std::string &getName() const;

    void setName(const std::string &name);

    const std::string &getObject_id() const;

    void setObject_id(const std::string &object_id);
};


#endif //HW1_DEFAULT_INFO_H