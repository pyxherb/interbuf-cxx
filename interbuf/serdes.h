#ifndef _INTERBUF_SERDES_H_
#define _INTERBUF_SERDES_H_

#include "document.h"
#include <peff/utils/byteord.h>

namespace interbuf {
	class Reader {
	public:
		INTERBUF_API virtual ~Reader();

		[[nodiscard]] virtual bool read(char *buffer, size_t size) noexcept = 0;

		[[nodiscard]] INTERBUF_FORCEINLINE bool readI8(int8_t &data) noexcept {
			return read((char *)&data, sizeof(int8_t));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readI16(int16_t &data) noexcept {
			return read((char *)&data, sizeof(int16_t));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readI32(int32_t &data) noexcept {
			return read((char *)&data, sizeof(int32_t));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readI64(int64_t &data) noexcept {
			return read((char *)&data, sizeof(int64_t));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readU8(uint8_t &data) noexcept {
			return read((char *)&data, sizeof(uint8_t));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readU16(uint16_t &data) noexcept {
			return read((char *)&data, sizeof(uint16_t));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readU32(uint32_t &data) noexcept {
			return read((char *)&data, sizeof(uint32_t));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readU64(uint64_t &data) noexcept {
			return read((char *)&data, sizeof(uint64_t));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readBool(bool &data) noexcept {
			return read((char *)&data, sizeof(bool));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readF32(float &data) noexcept {
			return read((char *)&data, sizeof(float));
		}

		[[nodiscard]] INTERBUF_FORCEINLINE bool readF64(double &data) noexcept {
			return read((char *)&data, sizeof(double));
		}

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

	enum class SerializeFrameType {
		StructMember = 0,
		ArrayElement,
	};

	struct SerializeFrame {
		std::variant<std::monostate, StructMemberSerializeFrameExData> exData;
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
			const char *ptr,
			size_t size,
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
