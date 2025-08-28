#include "except_base.h"

using namespace interbuf;

INTERBUF_API Exception::Exception(ExceptionKind kind) : kind(kind) {
}

INTERBUF_API Exception::~Exception() {
}
