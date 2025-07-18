#include "char_stream.h"

#include <algorithm>

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

size_t CharStream::line_start_pos(size_t req_line_no) const {
    if (req_line_no < 1) req_line_no = 1;

    return line_starts[req_line_no - 1];
}

FilePos CharStream::get_file_pos(size_t go_back) const {
    if (go_back == 0) return FilePos(line_no, line_start_pos(line_no));
    if (go_back > buffer_idx) go_back = buffer_idx;

    return get_file_pos_from_buffer_idx(buffer_idx - go_back).line_no;
}

FilePos CharStream::get_file_pos_from_buffer_idx(size_t idx) const {
    auto iter = std::upper_bound(
        line_starts.begin(),
        line_starts.end(),
        idx
    );

    iter--;

    size_t line_no = iter - line_starts.begin() + 1;
    size_t offset = idx - *iter;

    return FilePos(line_no, offset);
}


std::optional<StringLinePair> CharStream::get_line(size_t req_line_no, size_t first_idx) const {
    if (req_line_no < first_idx) return {};

    req_line_no -= first_idx;

    if (req_line_no >= line_starts.size()) return {};

    size_t line_start_idx = line_starts[req_line_no];
    size_t line_end_idx =
        req_line_no + 1 < line_starts.size() ?
            line_starts[req_line_no + 1] : line_starts.size();
    
    return StringLinePair(
        req_line_no,
        0,
        std::string(&buffer[line_start_idx], line_end_idx - line_start_idx)
    );
}

std::optional<StringLinePair> CharStream::last_n_as_str(size_t n_chars) const {
    if (buffer_idx < n_chars) return {};

    FilePos pos = get_file_pos_from_buffer_idx(buffer_idx - n_chars);

    return StringLinePair(
        pos.line_no,
        pos.offset,
        std::string(&buffer[buffer_idx - n_chars], n_chars)
    );
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