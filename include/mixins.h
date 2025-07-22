#pragma once

#include <cstddef>
#include <string>

#include "char_stream.h"
#include "file_pos.h"
#include "lang_err.h"

template <typename Err>
class ErrorRegistry {
    std::vector<Err> errors;

public:
    ErrorRegistry() {
        static_assert(
            std::is_base_of_v<LangErr,Err>,
            "provided class must be derived from LangErr"
        );
    }

    template <typename... Args>
    void register_error(Args&&... err) {
        errors.emplace_back(std::forward<Args>(err)...);
    }

    size_t n_errs() const {
        return errors.size();
    }
    const auto& get_errs() const {
        return errors;
    }
};

class HasErrFlag {
    bool err = false;
public:

    void set_err() {
        err = true;
    }
    bool has_err() const {
        return err;
    }
};

class HasFilePos {
    FilePos file_pos;

public:
    HasFilePos() {}
    HasFilePos(FilePos file_pos) : file_pos(file_pos) {}

    FilePos get_file_pos() const {
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