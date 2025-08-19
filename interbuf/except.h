#ifndef _INTERBUF_EXCEPT_H_
#define _INTERBUF_EXCEPT_H_

#include "except_base.h"
#include <new>

namespace interbuf {
	constexpr static peff::UUID
		EXCEPT_OOM = PEFF_UUID(6e1a12d1, 2a61, 47dd, ac92, afc369290d1b),
		EXCEPT_IO = PEFF_UUID(eed80e28, b8fb, 40de, 9b97, 81a8a6d688f3);

	/// @brief The out of memory error, indicates that a memory allocation has failed.
	class OutOfMemoryError : public Exception {
	public:
		INTERBUF_API OutOfMemoryError();
		INTERBUF_API virtual ~OutOfMemoryError();

		INTERBUF_API virtual void dealloc() override;

		INTERBUF_API static OutOfMemoryError *alloc();
	};

	extern OutOfMemoryError g_outOfMemoryError;

	class IOError : public Exception {
	public:
		peff::RcObjectPtr<peff::Alloc> allocator;

		INTERBUF_API IOError(peff::Alloc *allocator);
		INTERBUF_API virtual ~IOError();

		INTERBUF_API virtual void dealloc() override;

		INTERBUF_API static IOError *alloc(peff::Alloc *allocator) noexcept;
	};

	INTERBUF_FORCEINLINE ExceptionPointer withOutOfMemoryErrorIfAllocFailed(Exception *exceptionPtr) noexcept {
		if (!exceptionPtr) {
			return OutOfMemoryError::alloc();
		}
		return exceptionPtr;
	}
}

#endif
