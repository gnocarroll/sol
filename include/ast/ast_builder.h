#pragma once

#include <string>
#include <vector>

#include "char_stream.h"
#include "file_pos.h"

namespace ast {

class ASTBuilder {
public:
    struct ASTErr {
        FilePos file_pos;
        std::string err_msg;

        ASTErr() {}
        ASTErr(FilePos file_pos, std::string &&err_msg) :
            file_pos(file_pos), err_msg(std::move(err_msg)) {}

        const std::string& get_err_msg() const {
            return err_msg;
        }
    };

private:
    std::vector<ASTErr> errors;
    size_t cstream_lookback = 0;

public:
    class CStreamLookback {
        ASTBuilder &ast_builder;
        size_t old;

        CStreamLookback(ASTBuilder& ast_builder, size_t new_lookback) :
            ast_builder(ast_builder),
            old(ast_builder.cstream_lookback) {
            ast_builder.cstream_lookback = new_lookback;
        }
    public:
        ~CStreamLookback() {
            ast_builder.cstream_lookback = old;
        }

        friend class ASTBuilder;
    };

    CharStream cstream;

    ASTBuilder(CharStream &&cstream) :
        cstream(std::move(cstream)) {}

    void register_error(std::string&& err_msg) {
        errors.emplace_back(
            cstream.get_file_pos(cstream_lookback),
            std::move(err_msg)
        );
    }

    size_t n_errs() const {
        return errors.size();
    }
    const auto& get_errs() const {
        return errors;
    }

    auto create_char_lookback(size_t lookback) {
        return CStreamLookback(*this, lookback);
    }

    template <typename T, typename... Args>
    std::unique_ptr<T> make_w_pos(Args&&... args) {
        auto ret = std::make_unique<T>(std::forward<Args>(args)...);

        ret->set_file_pos(
            cstream.get_file_pos(cstream_lookback)
        );

        return ret;
    }
};

}