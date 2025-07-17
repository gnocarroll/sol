#pragma once

#include <istream>
#include <optional>
#include <sstream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

/*
 * Note: CharIOStack will "take ownership" of the file pointer handed to it.
 * The only thing that really means/the only consequence of that is that
 * the function to free an instance of that will call fclose on the FILE *.
*/

class CharStream {
public:
    class Checkpoint {
        size_t idx;

        Checkpoint(size_t idx) : idx(idx) {}

        friend class CharStream;
    };

    class ScopedCheckpoint {
        bool is_disabled = false;
        CharStream &cstream;
        size_t idx;

        ScopedCheckpoint(CharStream& cstream, size_t idx) :
            cstream(cstream), idx(idx) {}

    public:
        ~ScopedCheckpoint() {
            if (is_disabled) return;

            cstream.buffer_idx = idx;
        }

        void disable() {
            is_disabled = true;
        }

        friend class CharStream;
    };

private:
    size_t buffer_idx = 0;
    std::string buffer;

    std::optional<std::unique_ptr<std::istream>> owned_in_stream;
    std::istream& in_stream;

public:
    CharStream(std::string_view s) :
        owned_in_stream(
            std::make_unique<std::istringstream>(std::move(
                std::istringstream(s.data())
            ))
        ),
        in_stream(**owned_in_stream) {}
    CharStream(std::istream&& in_stream) :
        owned_in_stream(std::make_unique<std::istream>(std::move(in_stream))),
        in_stream(**owned_in_stream) {}

    // this last one is most useful for std::cin
    // otherwise maybe more desirable to give this ownership of istream
    CharStream(std::istream& in_stream) : in_stream(in_stream) {}

    std::optional<char> getc();

    Checkpoint checkpoint();
    ScopedCheckpoint scoped_checkpoint();
    void goto_checkpoint(Checkpoint);
};