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
    FilePos get_file_pos() {
        return file_pos;
    }
    void set_file_pos(FilePos file_pos) {
        this->file_pos = file_pos;
    }
};