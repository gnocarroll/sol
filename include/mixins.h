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

class HasErrMsg {
    static constexpr size_t PRINT_LINE_COUNT = 3;

    std::string msg;

public:
    HasErrMsg(std::string&& msg_details, CharStream& cstream, size_t back = 0) {
        FilePos file_pos = cstream.get_file_pos(back);
        
        msg.reserve(32);
        
        for (
            size_t print_line_no = file_pos.line_no - PRINT_LINE_COUNT + 1;
            print_line_no <= file_pos.line_no;
            print_line_no++
        ) {
            auto maybe_line = cstream.get_line(print_line_no);

            if (maybe_line) msg += maybe_line->get_str();
        }

        for (size_t i = 0; i < file_pos.offset - 1; i++) msg += " ";
        msg += "^\n";

        msg += msg_details;

        msg += "\n";

        std::cerr << msg;
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