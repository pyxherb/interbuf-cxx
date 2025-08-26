#ifndef _INTERBUF_DATATYPE_H_
#define _INTERBUF_DATATYPE_H_

#include "basedefs.h"
#include <cstdint>

namespace interbuf {
	enum class DataTypeKind : uint8_t {
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
		Binary,
		BoolTrue,
		BoolFalse,
		Object,
		Array
	};

	struct DataType {
		union {
			struct {
				struct {
					uint64_t len;
					uint8_t offset;
				} asBinary;
				struct {
					uint64_t nMembers;
					uint8_t offset;
				} asObject;
				struct {
					uint64_t len;
					uint8_t offset;
				} asArray;
			};
		} exData;
		DataTypeKind kind;

		DataType() = default;
		INTERBUF_FORCEINLINE DataType(DataTypeKind kind) : kind(kind) {}
		~DataType() = default;
	};
}

#endif
