#ifndef _INTERBUF_EXCEPT_BASE_H_
#define _INTERBUF_EXCEPT_BASE_H_

#include "basedefs.h"
#include <peff/base/uuid.h>
#include <peff/base/alloc.h>

namespace interbuf {
	enum class ExceptionKind {
		OutOfMemory = 0,
		IO,

		InvalidDataType,
	};

	class Exception {
	public:
		ExceptionKind kind;

		INTERBUF_API Exception(ExceptionKind kind);
		INTERBUF_API virtual ~Exception();

		virtual void dealloc() = 0;
	};

	class ExceptionPointer {
	private:
		Exception *_ptr = nullptr;

	public:
		INTERBUF_FORCEINLINE ExceptionPointer() noexcept = default;
		INTERBUF_FORCEINLINE ExceptionPointer(Exception *exception) noexcept : _ptr(exception) {
		}

		INTERBUF_FORCEINLINE ~ExceptionPointer() noexcept {
			unwrap();
			reset();
		}

		ExceptionPointer(const ExceptionPointer &) = delete;
		ExceptionPointer &operator=(const ExceptionPointer &) = delete;
		INTERBUF_FORCEINLINE ExceptionPointer(ExceptionPointer &&other) noexcept {
			_ptr = other._ptr;
			other._ptr = nullptr;
		}
		INTERBUF_FORCEINLINE ExceptionPointer &operator=(ExceptionPointer &&other) noexcept {
			_ptr = other._ptr;
			other._ptr = nullptr;
			return *this;
		}

		INTERBUF_FORCEINLINE Exception *get() noexcept {
			return _ptr;
		}
		INTERBUF_FORCEINLINE const Exception *get() const noexcept {
			return _ptr;
		}

		INTERBUF_FORCEINLINE void reset() noexcept {
			if (_ptr) {
				_ptr->dealloc();
			}
			_ptr = nullptr;
		}

		INTERBUF_FORCEINLINE void unwrap() noexcept {
			if (_ptr) {
				assert(("Unhandled NetKnot exception: ", false));
			}
		}

		INTERBUF_FORCEINLINE explicit operator bool() noexcept {
			return (bool)_ptr;
		}

		INTERBUF_FORCEINLINE Exception *operator->() noexcept {
			return _ptr;
		}

		INTERBUF_FORCEINLINE const Exception *operator->() const noexcept {
			return _ptr;
		}
	};
}

#define INTERBUF_UNWRAP_EXCEPT(expr) (expr).unwrap()
#define INTERBUF_RETURN_IF_EXCEPT(expr)                         \
	if (interbuf::ExceptionPointer e = (expr); (bool)e) \
	return e
#define INTERBUF_RETURN_IF_EXCEPT_WITH_LVAR(name, expr) \
	if ((bool)(name = (expr)))                         \
		return name;

#endif
