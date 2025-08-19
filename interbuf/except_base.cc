#include "except_base.h"

using namespace interbuf;

INTERBUF_API Exception::Exception(const peff::UUID &kind) : kind(kind) {
}

INTERBUF_API Exception::~Exception() {
}
