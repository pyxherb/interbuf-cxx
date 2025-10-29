#ifndef _INTERBUF_SERDES_H_
#define _INTERBUF_SERDES_H_

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

		virtual void dealloc() noexcept = 0;
	};

	class Writer {
	public:
		INTERBUF_API virtual ~Writer();

		virtual bool write(const char *src, size_t size) = 0;
		virtual bool writeI8(int8_t data) = 0;
		virtual bool writeI16(int16_t data) noexcept = 0;
		virtual bool writeI32(int32_t data) noexcept = 0;
		virtual bool writeI64(int64_t data) noexcept = 0;
		virtual bool writeU8(uint8_t data) noexcept = 0;
		virtual bool writeU16(uint16_t data) noexcept = 0;
		virtual bool writeU32(uint32_t data) noexcept = 0;
		virtual bool writeU64(uint64_t data) noexcept = 0;
		virtual bool writeF32(float data) noexcept = 0;
		virtual bool writeF64(double data) noexcept = 0;
		virtual bool writeBool(bool data) noexcept = 0;
	};

	struct StructMemberSerializeFrameExData {
		ObjectPtr<StructLayoutObject> layout;
		size_t idxMember = 0;

		INTERBUF_FORCEINLINE StructMemberSerializeFrameExData(ObjectPtr<StructLayoutObject> layout) : layout(layout) {}
		INTERBUF_API ~StructMemberSerializeFrameExData();
	};

	struct ArrayMemberSerializeFrameExData {
		ObjectPtr<ArrayDataTypeObject> dataType;
		size_t length = 0;
		size_t idxMember = 0;

		INTERBUF_FORCEINLINE ArrayMemberSerializeFrameExData(ObjectPtr<ArrayDataTypeObject> dataType) : dataType(dataType) {}
		INTERBUF_API ~ArrayMemberSerializeFrameExData();
	};

	enum class SerializeFrameType {
		StructMember = 0,
		ArrayElement,
	};

	struct SerializeFrame {
		std::variant<std::monostate, StructMemberSerializeFrameExData, ArrayMemberSerializeFrameExData> exData;
		SerializeFrameType frameType;
		const char *ptr;
		size_t size;
		size_t offset = 0;
	};

	struct SerializeContext {
		peff::RcObjectPtr<peff::Alloc> allocator;
		peff::List<SerializeFrame> frames;
		Writer *writer;

		INTERBUF_FORCEINLINE SerializeContext(
			peff::Alloc *allocator,
			Writer *writer)
			: frames(allocator),
			  writer(writer) {}
		INTERBUF_API ~SerializeContext();
	};

	INTERBUF_API ExceptionPointer _doSerializeStruct(SerializeContext *context);
	INTERBUF_API ExceptionPointer serializeStruct(peff::Alloc *allocator, const void *ptr, size_t size, Writer *writer, ObjectPtr<StructLayoutObject> rootLayout);
}

#define INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(allocator, e)         \
	if (!(e))                                                        \
		return withOOMErrorIfAllocFailed(IOError::alloc(allocator)); \
	else                                                             \
		;

#endif
