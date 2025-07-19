#pragma once

#include <string>

class LangErr {
    std::string err_msg;

public:
    LangErr() {}
    LangErr(std::string&& err_msg) : err_msg(std::move(err_msg)) {}

    const std::string &get_err_msg() const {
        return err_msg;
    }
};