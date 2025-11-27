#ifndef _INTERBUF_DESERIALIZE_H_
#define _INTERBUF_DESERIALIZE_H_

#include "document.h"
#include <peff/utils/byteord.h>

namespace interbuf {
	class Reader {
	public:
		INTERBUF_API virtual ~Reader();

		[[nodiscard]] virtual bool read(char *buffer, size_t size) noexcept = 0;

		[[nodiscard]] virtual bool readI8(int8_t &data) noexcept = 0;
		[[nodiscard]] virtual bool readI16(int16_t &data) noexcept = 0;
		[[nodiscard]] virtual bool readI32(int32_t &data) noexcept = 0;
		[[nodiscard]] virtual bool readI64(int64_t &data) noexcept = 0;
		[[nodiscard]] virtual bool readU8(uint8_t &data) noexcept = 0;
		[[nodiscard]] virtual bool readU16(uint16_t &data) noexcept = 0;
		[[nodiscard]] virtual bool readU32(uint32_t &data) noexcept = 0;
		[[nodiscard]] virtual bool readU64(uint64_t &data) noexcept = 0;
		[[nodiscard]] virtual bool readBool(bool &data) noexcept = 0;
		[[nodiscard]] virtual bool readF32(float &data) noexcept = 0;
		[[nodiscard]] virtual bool readF64(double &data) noexcept = 0;
	};

	struct StructMemberDeserializeFrameExData {
		ObjectPtr<StructLayoutObject> layout;
		size_t idxMember = 0;

		INTERBUF_FORCEINLINE StructMemberDeserializeFrameExData(ObjectPtr<StructLayoutObject> layout) : layout(layout) {}
		INTERBUF_API ~StructMemberDeserializeFrameExData();
	};

	struct ClassMemberDeserializeFrameExData {
		ObjectPtr<ClassLayoutObject> layout;
		size_t nMembers = 0;
		size_t idxMember = 0;

		INTERBUF_FORCEINLINE ClassMemberDeserializeFrameExData(ObjectPtr<ClassLayoutObject> layout, size_t nMembers) : layout(layout), nMembers(nMembers) {}
		INTERBUF_API ~ClassMemberDeserializeFrameExData();
	};

	struct ArrayMemberDeserializeFrameExData {
		ObjectPtr<ArrayDataTypeObject> dataType;
		size_t length = 0;
		size_t idxMember = 0;

		INTERBUF_FORCEINLINE ArrayMemberDeserializeFrameExData(ObjectPtr<ArrayDataTypeObject> dataType) : dataType(dataType) {}
		INTERBUF_API ~ArrayMemberDeserializeFrameExData();
	};

	enum class DeserializeFrameType {
		StructMember = 0,
		ClassMember,
		ArrayMember,
	};

	struct DeserializeFrame {
		std::variant<std::monostate, StructMemberDeserializeFrameExData, ClassMemberDeserializeFrameExData, ArrayMemberDeserializeFrameExData> exData;
		DeserializeFrameType frameType;
		char *ptr;
		size_t szPerElement;
		ObjectPtr<DataTypeObject> elementType;
	};

	struct DeserializeContext {
		peff::RcObjectPtr<peff::Alloc> allocator;
		peff::List<DeserializeFrame> frames;
		Reader *reader;

		INTERBUF_FORCEINLINE DeserializeContext(
			peff::Alloc *allocator,
			Reader *reader)
			: allocator(allocator),
			  frames(allocator),
			  reader(reader) {}
		INTERBUF_API ~DeserializeContext();
	};

	INTERBUF_API ExceptionPointer _doDeserialize(DeserializeContext *context);
	INTERBUF_API ExceptionPointer deserializeStruct(peff::Alloc *allocator, void *ptr, size_t size, Reader *reader, ObjectPtr<StructLayoutObject> rootLayout);
	INTERBUF_API ExceptionPointer deserializeClass(peff::Alloc *allocator, void *ptr, size_t size, Reader *reader, ObjectPtr<ClassLayoutObject> rootLayout);
}

#define INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(allocator, e)         \
	if (!(e))                                                        \
		return withOOMErrorIfAllocFailed(IOError::alloc(allocator)); \
	else                                                             \
		;

#endif
