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

	extern OutOfMemoryError g_outOfMemoryError;

	class PrematuredEndOfBufferError : public Exception {
	public:
		peff::RcObjectPtr<peff::Alloc> allocator;

		INTERBUF_API PrematuredEndOfBufferError(peff::Alloc *allocator);
		INTERBUF_API virtual ~PrematuredEndOfBufferError();

		INTERBUF_API virtual void dealloc() override;

		INTERBUF_API static PrematuredEndOfBufferError *alloc(peff::Alloc *allocator) noexcept;
	};

	INTERBUF_FORCEINLINE ExceptionPointer withOutOfMemoryErrorIfAllocFailed(Exception *exceptionPtr) noexcept {
		if (!exceptionPtr) {
			return OutOfMemoryError::alloc();
		}
		return exceptionPtr;
	}
}

#endif
