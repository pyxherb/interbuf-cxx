#include "except.h"

using namespace interbuf;

OutOfMemoryError interbuf::g_outOfMemoryError;

INTERBUF_API OutOfMemoryError::OutOfMemoryError() : Exception(EXCEPT_OOM) {}
INTERBUF_API OutOfMemoryError::~OutOfMemoryError() {}

INTERBUF_API void OutOfMemoryError::dealloc() {
}

INTERBUF_API OutOfMemoryError *OutOfMemoryError::alloc() {
	return &g_outOfMemoryError;
}

INTERBUF_API IOError::IOError(peff::Alloc *allocator)
	: Exception(EXCEPT_IO) {}
INTERBUF_API IOError::~IOError() {}

INTERBUF_API void IOError::dealloc() {
	peff::destroyAndRelease<IOError>(allocator.get(), this, sizeof(std::max_align_t));
}

INTERBUF_API IOError *IOError::alloc(peff::Alloc *allocator) noexcept {
	void *buf = allocator->alloc(sizeof(IOError), sizeof(std::max_align_t));

	if (!buf)
		return nullptr;

	peff::constructAt<IOError>((IOError *)buf, allocator);

	return (IOError *)buf;
}
