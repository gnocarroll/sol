#pragma once

#include "mixins.h"

namespace ast {

class ASTObject : public HasFilePos, public HasErrFlag {
	ASTObject(ASTObject& other) = delete;
};

}