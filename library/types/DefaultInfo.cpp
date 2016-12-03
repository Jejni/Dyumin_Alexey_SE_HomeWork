//
// Created by alexey on 11/20/16.
//

#include "DefaultInfo.h"

DefaultInfo::DefaultInfo() {
    name = "default_name";
    object_id = "";
}

DefaultInfo::DefaultInfo(const std::string &get_name) {
    DefaultInfo::name = get_name;
}

const std::string &DefaultInfo::getName() const {
    return name;
}

void DefaultInfo::setName(const std::string &name) {
    DefaultInfo::name = name;
}

const std::string &DefaultInfo::getObject_id() const {
    return object_id;
}

void DefaultInfo::setObject_id(const std::string &object_id) {
    DefaultInfo::object_id = object_id;
}
