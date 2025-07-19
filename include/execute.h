#pragma once

#include <string>

class RetCode {
public:
    enum RetCodeEnum {
        RET_OK,
        RET_ERR,
    };

private:
    RetCodeEnum status = RET_OK;
    std::optional<std::string> err_msg;

public:
    bool is_err() {
        return status == RET_ERR;
    }
    bool is_ok() {
        return status == RET_OK;
    }

    RetCode() {}
    RetCode(std::string err_msg) :
        status(RET_ERR), err_msg(err_msg) {}

    static RetCode ok() {
        return RetCode();
    }

    std::optional<std::string> get_err_msg() {
        return err_msg;
    }
};