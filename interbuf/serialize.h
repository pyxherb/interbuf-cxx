#ifndef _INTERBUF_SERIALIZE_H_
#define _INTERBUF_SERIALIZE_H_

#include "document.h"
#include <peff/utils/byteord.h>

namespace interbuf {
	class Writer {
	public:
		INTERBUF_API virtual ~Writer();

		virtual ExceptionPointer write(const char *src, size_t size) = 0;
		virtual ExceptionPointer writeI8(int8_t data) = 0;
		virtual ExceptionPointer writeI16(int16_t data) noexcept = 0;
		virtual ExceptionPointer writeI32(int32_t data) noexcept = 0;
		virtual ExceptionPointer writeI64(int64_t data) noexcept = 0;
		virtual ExceptionPointer writeU8(uint8_t data) noexcept = 0;
		virtual ExceptionPointer writeU16(uint16_t data) noexcept = 0;
		virtual ExceptionPointer writeU32(uint32_t data) noexcept = 0;
		virtual ExceptionPointer writeU64(uint64_t data) noexcept = 0;
		virtual ExceptionPointer writeF32(float data) noexcept = 0;
		virtual ExceptionPointer writeF64(double data) noexcept = 0;
		virtual ExceptionPointer writeBool(bool data) noexcept = 0;
	};

	struct StructMemberSerializeFrameExData {
		ObjectPtr<StructLayoutObject> layout;
		size_t idxMember = 0;

		INTERBUF_FORCEINLINE StructMemberSerializeFrameExData(ObjectPtr<StructLayoutObject> layout) : layout(layout) {}
		INTERBUF_API ~StructMemberSerializeFrameExData();
	};

	struct ClassMemberSerializeFrameExData {
		ObjectPtr<ClassLayoutObject> layout;
		size_t idxMember = 0;

		INTERBUF_FORCEINLINE ClassMemberSerializeFrameExData(ObjectPtr<ClassLayoutObject> layout) : layout(layout) {}
		INTERBUF_API ~ClassMemberSerializeFrameExData();
	};

	struct ArrayMemberSerializeFrameExData {
		ObjectPtr<ArrayDataTypeDefObject> dataType;
		size_t length = 0;
		size_t idxMember = 0;

		INTERBUF_FORCEINLINE ArrayMemberSerializeFrameExData(ObjectPtr<ArrayDataTypeDefObject> dataType) : dataType(dataType) {}
		INTERBUF_API ~ArrayMemberSerializeFrameExData();
	};

	enum class SerializeFrameType {
		StructMember = 0,
		ClassMember,
		ArrayMember,
	};

	struct SerializeFrame {
		std::variant<std::monostate, StructMemberSerializeFrameExData, ClassMemberSerializeFrameExData, ArrayMemberSerializeFrameExData> exData;
		SerializeFrameType frameType;
		const char *ptr;
		size_t szPerElement;
		DataType elementType;
	};

	struct SerializeContext {
		peff::RcObjectPtr<peff::Alloc> allocator;
		peff::List<SerializeFrame> frames;
		Writer *writer;

		INTERBUF_FORCEINLINE SerializeContext(
			peff::Alloc *allocator,
			Writer *writer)
			: allocator(allocator),
			  frames(allocator),
			  writer(writer) {}
		INTERBUF_API ~SerializeContext();
	};

	INTERBUF_API ExceptionPointer _doSerialize(SerializeContext *context);
	INTERBUF_API ExceptionPointer serializeStruct(peff::Alloc *allocator, const void *ptr, Writer *writer, ObjectPtr<StructLayoutObject> rootLayout);
	INTERBUF_API ExceptionPointer serializeClass(peff::Alloc *allocator, const void *ptr, Writer *writer, ObjectPtr<ClassLayoutObject> rootLayout);
}

#endif
