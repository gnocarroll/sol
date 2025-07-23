#pragma once

#include <istream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "string_line_pair.h"

/*
 * Note: CharIOStack will "take ownership" of the file pointer handed to it.
 * The only thing that really means/the only consequence of that is that
 * the function to free an instance of that will call fclose on the FILE *.
*/

class CharStream {
public:
    class Checkpoint {
        size_t buffer_idx;
        size_t line_no;

        Checkpoint(size_t buffer_idx, size_t line_no) :
            buffer_idx(buffer_idx), line_no(line_no) {}

        friend class CharStream;
    };

    class ScopedCheckpoint {
        bool is_disabled = false;
        CharStream &cstream;
        size_t buffer_idx;
        size_t line_no;

        ScopedCheckpoint(CharStream& cstream, size_t idx, size_t line_no) :
            cstream(cstream), buffer_idx(idx), line_no(line_no) {}

    public:
        ~ScopedCheckpoint() {
            if (is_disabled) return;

            cstream.buffer_idx = buffer_idx;
            cstream.line_no = line_no;
        }

        void disable() {
            is_disabled = true;
        }

        friend class CharStream;
    };

private:
    bool eof_flag = false;

    size_t buffer_idx = 0;
    std::string buffer;

    size_t line_no = 1;

    std::vector<size_t> line_starts{ 0 };

    std::istream& in_stream;

public:

    CharStream(std::istream& in_stream) : in_stream(in_stream) {}

    std::optional<char> getc();
    std::optional<char> peekc();

    size_t line_start_pos(size_t req_line_no) const;
    FilePos get_file_pos(size_t go_back = 0) const;

    std::optional<StringLinePair> get_line(size_t line_no, size_t first_idx = 1) const;
    std::optional<StringLinePair> last_n_as_str(size_t n_chars) const;

    FilePos get_file_pos_from_buffer_idx(size_t idx) const;

    Checkpoint checkpoint();
    ScopedCheckpoint scoped_checkpoint();
    void goto_checkpoint(Checkpoint);
};