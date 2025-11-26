#ifndef _INTERBUF_DATATYPE_H_
#define _INTERBUF_DATATYPE_H_

#include "basedefs.h"
#include <cstdint>

namespace interbuf {
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
		Class,
		Array
	};

	enum class ObjectType : uint8_t {
		DataType = 0,
		StructLayout,
		StructInstance,
		ClassLayout,
		ClassInstance
	};
}

#endif
