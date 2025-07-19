#pragma once

#include <string>

#include "file_pos.h"

/// @brief when you get a string out of CharStream you will get position in "file" as well
class StringLinePair {
    FilePos pos;
    std::string s;
public:

    StringLinePair(size_t line_no, size_t offset, std::string&& str) :
        pos(line_no, offset), s(std::move(str)) {}

    size_t get_line_no() const {
        return pos.line_no;
    }
    size_t get_line_offset() const {
        return pos.offset;
    }
    FilePos get_file_pos() const {
        return pos;
    }

    const std::string& get_str() const {
        return s;
    }
};