#ifndef _INTERBUF_DATATYPE_H_
#define _INTERBUF_DATATYPE_H_

#include "basedefs.h"
#include <cstdint>

namespace interbuf {
	enum class RawDataTypeKind : uint8_t {
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

		BoolTrue,
		BoolFalse,

		Struct,
		Array
	};

	enum class FieldTypeKind : uint8_t {
		I8 = 0,
		I16,
		I32,
		I64,
		U8,
		U16,
		U32,
		U64,
		F32,
		F64,
		String,
		Bool,
		Struct,
		Array
	};

	enum class ObjectType : uint8_t {
		DataType = 0,
		StructLayout,
		StructInstance,
	};
}

#endif
