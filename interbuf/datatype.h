#ifndef _INTERBUF_DATATYPE_H_
#define _INTERBUF_DATATYPE_H_

#include <cstdint>

namespace interbuf {
	enum class DataType : uint8_t {
		I8 = 0,
		I16,
		I24,
		I32,
		I40,
		I48,
		I56,
		I64,

		U8,
		U16,
		U24,
		U32,
		U40,
		U48,
		U56,
		U64,

		F32,
		F64,
		String,
		Bool,
		Object
	};
}

#endif
