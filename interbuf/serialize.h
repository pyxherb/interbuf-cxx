#ifndef _INTERBUF_SERIALIZE_H_
#define _INTERBUF_SERIALIZE_H_

#include "document.h"
#include <peff/utils/byteord.h>

namespace interbuf {
	class Writer {
	public:
		INTERBUF_API virtual ~Writer();

		virtual ExceptionPointer write(const char *src, size_t size) = 0;
		virtual ExceptionPointer write_i8(int8_t data) = 0;
		virtual ExceptionPointer write_i16(int16_t data) noexcept = 0;
		virtual ExceptionPointer write_i32(int32_t data) noexcept = 0;
		virtual ExceptionPointer write_i64(int64_t data) noexcept = 0;
		virtual ExceptionPointer write_u8(uint8_t data) noexcept = 0;
		virtual ExceptionPointer write_u16(uint16_t data) noexcept = 0;
		virtual ExceptionPointer write_u32(uint32_t data) noexcept = 0;
		virtual ExceptionPointer write_u64(uint64_t data) noexcept = 0;
		virtual ExceptionPointer write_f32(float data) noexcept = 0;
		virtual ExceptionPointer write_f64(double data) noexcept = 0;
		virtual ExceptionPointer write_bool(bool data) noexcept = 0;
	};

	struct StructMemberSerializeFrameExData {
		ObjectPtr<StructLayoutObject> layout;
		size_t idx_member = 0;

		INTERBUF_FORCEINLINE StructMemberSerializeFrameExData(ObjectPtr<StructLayoutObject> layout) : layout(layout) {}
		INTERBUF_API ~StructMemberSerializeFrameExData();
	};

	struct ClassMemberSerializeFrameExData {
		ObjectPtr<ClassLayoutObject> layout;
		size_t idx_member = 0;

		INTERBUF_FORCEINLINE ClassMemberSerializeFrameExData(ObjectPtr<ClassLayoutObject> layout) : layout(layout) {}
		INTERBUF_API ~ClassMemberSerializeFrameExData();
	};

	struct ArrayMemberSerializeFrameExData {
		ObjectPtr<ArrayDataTypeDefObject> data_type;
		size_t length = 0;
		size_t idx_member = 0;

		INTERBUF_FORCEINLINE ArrayMemberSerializeFrameExData(ObjectPtr<ArrayDataTypeDefObject> data_type) : data_type(data_type) {}
		INTERBUF_API ~ArrayMemberSerializeFrameExData();
	};

	enum class SerializeFrameType {
		StructMember = 0,
		ClassMember,
		ArrayMember,
	};

	struct SerializeFrame {
		std::variant<std::monostate, StructMemberSerializeFrameExData, ClassMemberSerializeFrameExData, ArrayMemberSerializeFrameExData> ex_data;
		SerializeFrameType frame_type;
		const char *ptr;
		size_t sz_per_element;
		DataType element_type;
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

	INTERBUF_API ExceptionPointer _do_serialize(SerializeContext *context);
	INTERBUF_API ExceptionPointer serialize_struct(peff::Alloc *allocator, const void *ptr, Writer *writer, ObjectPtr<StructLayoutObject> root_layout);
	INTERBUF_API ExceptionPointer serialize_class(peff::Alloc *allocator, const void *ptr, Writer *writer, ObjectPtr<ClassLayoutObject> root_layout);
}

#endif
