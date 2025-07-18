#pragma once

#include <string>

class StringLinePair {
    size_t line_no;
    std::string s;
public:

    StringLinePair(size_t line_no, std::string&& str) :
        line_no(line_no), s(std::move(str)) {}

    size_t get_line_no() const {
        return line_no;
    }

    const std::string& get_str() const {
        return s;
    }
};