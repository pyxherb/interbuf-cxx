#include "except.h"

using namespace interbuf;

OutOfMemoryError interbuf::g_outOfMemoryError;

INTERBUF_API OutOfMemoryError::OutOfMemoryError() : Exception(ExceptionKind::OutOfMemory) {}
INTERBUF_API OutOfMemoryError::~OutOfMemoryError() {}

INTERBUF_API void OutOfMemoryError::dealloc() {
}

INTERBUF_API OutOfMemoryError *OutOfMemoryError::alloc() {
	return &g_outOfMemoryError;
}

INTERBUF_API PrematuredEndOfBufferError::PrematuredEndOfBufferError(peff::Alloc *allocator)
	: Exception(ExceptionKind::PrematuredEndOfBuffer) {}
INTERBUF_API PrematuredEndOfBufferError::~PrematuredEndOfBufferError() {}

INTERBUF_API void PrematuredEndOfBufferError::dealloc() {
	peff::destroyAndRelease<PrematuredEndOfBufferError>(allocator.get(), this, sizeof(std::max_align_t));
}

INTERBUF_API PrematuredEndOfBufferError *PrematuredEndOfBufferError::alloc(peff::Alloc *allocator) noexcept {
	void *buf = allocator->alloc(sizeof(PrematuredEndOfBufferError), sizeof(std::max_align_t));

	if (!buf)
		return nullptr;

	peff::constructAt<PrematuredEndOfBufferError>((PrematuredEndOfBufferError *)buf, allocator);

	return (PrematuredEndOfBufferError *)buf;
}
