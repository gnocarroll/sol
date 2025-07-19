#pragma once

#include <cstddef>

struct FilePos {
    size_t line_no = 0;
    size_t offset = 0;

    FilePos() {}
    FilePos(size_t line_no, size_t offset) : line_no(line_no), offset(offset) {}
};