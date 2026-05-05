#include "except.h"

using namespace interbuf;

static OutOfMemoryError _g_out_of_memory_error;

INTERBUF_API OutOfMemoryError::OutOfMemoryError() : Exception(ExceptionKind::OutOfMemory) {}
INTERBUF_API OutOfMemoryError::~OutOfMemoryError() {}

INTERBUF_API void OutOfMemoryError::dealloc() {
}

INTERBUF_API OutOfMemoryError *OutOfMemoryError::alloc() {
	return &_g_out_of_memory_error;
}

INTERBUF_API IOError::IOError(peff::Alloc *allocator)
	: Exception(ExceptionKind::IO) {}
INTERBUF_API IOError::~IOError() {}

INTERBUF_API void IOError::dealloc() {
	peff::destroy_and_release<IOError>(allocator.get(), this, sizeof(std::max_align_t));
}

INTERBUF_API IOError *IOError::alloc(peff::Alloc *allocator) noexcept {
	void *buf = allocator->alloc(sizeof(IOError), sizeof(std::max_align_t));

	if (!buf)
		return nullptr;

	peff::construct_at<IOError>((IOError *)buf, allocator);

	return (IOError *)buf;
}

INTERBUF_API FieldNameLengthError::FieldNameLengthError(peff::Alloc *allocator)
	: Exception(ExceptionKind::FieldNameLength) {}
INTERBUF_API FieldNameLengthError::~FieldNameLengthError() {}

INTERBUF_API void FieldNameLengthError::dealloc() {
	peff::destroy_and_release<FieldNameLengthError>(allocator.get(), this, sizeof(std::max_align_t));
}

INTERBUF_API FieldNameLengthError *FieldNameLengthError::alloc(peff::Alloc *allocator) noexcept {
	void *buf = allocator->alloc(sizeof(FieldNameLengthError), sizeof(std::max_align_t));

	if (!buf)
		return nullptr;

	peff::construct_at<FieldNameLengthError>((FieldNameLengthError *)buf, allocator);

	return (FieldNameLengthError *)buf;
}

INTERBUF_API IllegalObjectLayoutError::IllegalObjectLayoutError(peff::Alloc *allocator)
	: Exception(ExceptionKind::IllegalObjectLayout) {}
INTERBUF_API IllegalObjectLayoutError::~IllegalObjectLayoutError() {}

INTERBUF_API void IllegalObjectLayoutError::dealloc() {
	peff::destroy_and_release<IllegalObjectLayoutError>(allocator.get(), this, sizeof(std::max_align_t));
}

INTERBUF_API IllegalObjectLayoutError *IllegalObjectLayoutError::alloc(peff::Alloc *allocator) noexcept {
	void *buf = allocator->alloc(sizeof(IllegalObjectLayoutError), sizeof(std::max_align_t));

	if (!buf)
		return nullptr;

	peff::construct_at<IllegalObjectLayoutError>((IllegalObjectLayoutError *)buf, allocator);

	return (IllegalObjectLayoutError *)buf;
}
