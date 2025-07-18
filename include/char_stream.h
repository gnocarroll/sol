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

    std::istream& in_stream;

public:

    // this last one is most useful for std::cin
    // otherwise maybe more desirable to give this ownership of istream
    CharStream(std::istream& in_stream) : in_stream(in_stream) {
        in_stream >> std::noskipws;
    }
    ~CharStream() {
        in_stream >> std::skipws;
    }

    std::optional<char> getc();
    std::optional<char> peekc();

    std::optional<std::string> last_n_as_str(size_t n_chars);

    Checkpoint checkpoint();
    ScopedCheckpoint scoped_checkpoint();
    void goto_checkpoint(Checkpoint);
};