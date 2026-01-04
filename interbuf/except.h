#ifndef _INTERBUF_EXCEPT_H_
#define _INTERBUF_EXCEPT_H_

#include "except_base.h"
#include <new>

namespace interbuf {
	/// @brief The out of memory error, indicates that a memory allocation has failed.
	class OutOfMemoryError : public Exception {
	public:
		INTERBUF_API OutOfMemoryError();
		INTERBUF_API virtual ~OutOfMemoryError();

		INTERBUF_API virtual void dealloc() override;

		INTERBUF_API static OutOfMemoryError *alloc();
	};

	class IOError : public Exception {
	public:
		peff::RcObjectPtr<peff::Alloc> allocator;

		INTERBUF_API IOError(peff::Alloc *allocator);
		INTERBUF_API virtual ~IOError();

		INTERBUF_API virtual void dealloc() override;

		INTERBUF_API static IOError *alloc(peff::Alloc *allocator) noexcept;
	};

	class FieldNameLengthError : public Exception {
	public:
		peff::RcObjectPtr<peff::Alloc> allocator;

		INTERBUF_API FieldNameLengthError(peff::Alloc *allocator);
		INTERBUF_API virtual ~FieldNameLengthError();

		INTERBUF_API virtual void dealloc() override;

		INTERBUF_API static FieldNameLengthError *alloc(peff::Alloc *allocator) noexcept;
	};

	class IllegalObjectLayoutError : public Exception {
	public:
		peff::RcObjectPtr<peff::Alloc> allocator;

		INTERBUF_API IllegalObjectLayoutError(peff::Alloc *allocator);
		INTERBUF_API virtual ~IllegalObjectLayoutError();

		INTERBUF_API virtual void dealloc() override;

		INTERBUF_API static IllegalObjectLayoutError *alloc(peff::Alloc *allocator) noexcept;
	};

	INTERBUF_FORCEINLINE ExceptionPointer withOOMErrorIfAllocFailed(Exception *exceptionPtr) noexcept {
		if (!exceptionPtr) {
			return OutOfMemoryError::alloc();
		}
		return exceptionPtr;
	}
}

#endif
