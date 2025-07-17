#include "char_stream.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

std::optional<char> CharStream::getc() {
    if (buffer_idx >= buffer.size()) {
        char c;

        if (!(in_stream >> c)) {
            return {};
        }

        buffer.push_back(c);
    }

    buffer_idx++;

    return buffer[buffer_idx - 1];
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