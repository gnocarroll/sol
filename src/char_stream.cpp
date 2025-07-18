#include "char_stream.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

std::optional<char> CharStream::getc() {
    auto ret = peekc();

    if (ret) buffer_idx++;

    return ret;
}

std::optional<char> CharStream::peekc() {
    if (buffer_idx >= buffer.size()) {
        char c;

        if (!(in_stream >> c)) {
            return {};
        }

        buffer.push_back(c);
    }

    return buffer[buffer_idx];
}

std::optional<std::string> CharStream::last_n_as_str(size_t n_chars) {
    if (buffer_idx < n_chars) return {};

    return std::string(&buffer[buffer_idx - n_chars], n_chars);
}


CharStream::Checkpoint CharStream::checkpoint() {
    return Checkpoint(buffer_idx);
}

void CharStream::goto_checkpoint(Checkpoint checkpoint) {
    if (checkpoint.idx > buffer.size()) {
        checkpoint.idx = buffer.size();
    }

    buffer_idx = checkpoint.idx;
}

CharStream::ScopedCheckpoint CharStream::scoped_checkpoint() {
    return ScopedCheckpoint(*this, buffer_idx);
}