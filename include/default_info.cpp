//
// Created by alexey on 11/20/16.
//

#include "default_info.h"

default_info::default_info() {
    name = "default_name";
    object_id = "";
}

default_info::default_info(const std::string &get_name) {
    setName(get_name);
}

const std::string &default_info::getName() const {
    return name;
}

void default_info::setName(const std::string &name) {
    default_info::name = name;
}

const std::string &default_info::getObject_id() const {
    return object_id;
}

void default_info::setObject_id(const std::string &object_id) {
    default_info::object_id = object_id;
}
