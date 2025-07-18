#include "char_stream.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

std::optional<char> CharStream::getc() {
    auto ret = peekc();

    if (ret) {
        buffer_idx++;

        if (*ret == '\n') line_no++;
    }

    return ret;
}

std::optional<char> CharStream::peekc() {
    if (buffer_idx >= buffer.size()) {
        char c;

        if (!(in_stream >> c)) {
            return {};
        }

        buffer.push_back(c);

        if (c == '\n') line_starts.push_back(buffer.size());
    }

    return buffer[buffer_idx];
}

std::optional<StringLinePair> CharStream::get_line(size_t req_line_no, size_t first_idx) {
    if (req_line_no < first_idx) return {};

    req_line_no -= first_idx;

    if (req_line_no >= line_starts.size()) return {};

    size_t line_start_idx = line_starts[req_line_no];
    size_t line_end_idx =
        req_line_no + 1 < line_starts.size() ?
            line_starts[req_line_no + 1] : line_starts.size();
    
    return StringLinePair(
        req_line_no,
        std::string(&buffer[line_start_idx], line_end_idx - line_start_idx)
    );
}

std::optional<StringLinePair> CharStream::last_n_as_str(size_t n_chars) {
    if (buffer_idx < n_chars) return {};

    size_t ret_line_no = line_no;

    for (size_t pos = buffer_idx - n_chars; pos < buffer_idx; pos++) {
        if (buffer[pos] == '\n') ret_line_no--;
    }

    return StringLinePair(ret_line_no, std::string(&buffer[buffer_idx - n_chars], n_chars));
}


CharStream::Checkpoint CharStream::checkpoint() {
    return Checkpoint(buffer_idx, line_no);
}

void CharStream::goto_checkpoint(Checkpoint checkpoint) {
    if (checkpoint.buffer_idx > buffer.size()) {
        checkpoint.buffer_idx = buffer.size();
    }

    buffer_idx = checkpoint.buffer_idx;
    line_no = checkpoint.line_no;
}

CharStream::ScopedCheckpoint CharStream::scoped_checkpoint() {
    return ScopedCheckpoint(*this, buffer_idx, line_no);
}