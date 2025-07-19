#pragma once

#include <string>
#include <vector>

#include "char_stream.h"
#include "file_pos.h"
#include "lang_err.h"
#include "mixins.h"

namespace ast {

struct ASTErr : public HasFilePos, public LangErr {
    ASTErr() {}
    ASTErr(FilePos file_pos, std::string &&err_msg) :
        HasFilePos(file_pos), LangErr(std::move(err_msg)) {}
};

class ASTBuilder : public ErrorRegistry<ASTErr> {
private:
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
        ErrorRegistry<ASTErr>::register_error(
            ASTErr(cstream.get_file_pos(cstream_lookback), std::move(err_msg))
        );
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