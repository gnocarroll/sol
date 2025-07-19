#pragma once

#include <cstddef>
#include <string>

#include "char_stream.h"
#include "file_pos.h"

class HasErrFlag {
    bool err = false;
public:

    void set_err() {
        err = true;
    }
    bool has_err() {
        return err;
    }
};

class HasFilePos {
    FilePos file_pos;

public:
    HasFilePos() {}
    HasFilePos(FilePos file_pos) : file_pos(file_pos) {}

    FilePos get_file_pos() {
        return file_pos;
    }
    void set_file_pos(FilePos file_pos) {
        this->file_pos = file_pos;
    }

    size_t get_line_no() const {
        return file_pos.line_no;
    }
    size_t get_line_offset() const {
        return file_pos.offset;
    }
};