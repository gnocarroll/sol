#pragma once

#include <cstddef>

struct FilePos {
    size_t line_no = 0;
    size_t offset = 0;

    FilePos() {}
    FilePos(size_t line_no, size_t offset) : line_no(line_no), offset(offset) {}
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

template <typename T, typename... Args>
std::unique_ptr<T> make_unique_w_pos(FilePos file_pos, Args&&... args) {
    auto ret = std::make_unique<T>(std::forward<Args>(args)...);

    ret->set_file_pos(file_pos);

    return ret;
}