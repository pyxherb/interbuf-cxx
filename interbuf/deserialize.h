#ifndef _INTERBUF_DESERIALIZE_H_
#define _INTERBUF_DESERIALIZE_H_

#include "document.h"
#include <peff/utils/byteord.h>

namespace interbuf {
	class Reader {
	public:
		INTERBUF_API virtual ~Reader();

		[[nodiscard]] virtual ExceptionPointer read(char *buffer, size_t size) noexcept = 0;

		[[nodiscard]] virtual ExceptionPointer read_i8(int8_t &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_i16(int16_t &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_i32(int32_t &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_i64(int64_t &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_u8(uint8_t &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_u16(uint16_t &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_u32(uint32_t &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_u64(uint64_t &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_bool(bool &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_f32(float &data) noexcept = 0;
		[[nodiscard]] virtual ExceptionPointer read_f64(double &data) noexcept = 0;
	};

	struct StructMemberDeserializeFrameExData {
		ObjectPtr<StructLayoutObject> layout;
		size_t idx_member = 0;

		INTERBUF_FORCEINLINE StructMemberDeserializeFrameExData(ObjectPtr<StructLayoutObject> layout) : layout(layout) {}
		INTERBUF_API ~StructMemberDeserializeFrameExData();
	};

	struct ClassMemberDeserializeFrameExData {
		ObjectPtr<ClassLayoutObject> layout;
		size_t n_members = 0;
		size_t idx_member = 0;

		INTERBUF_FORCEINLINE ClassMemberDeserializeFrameExData(ObjectPtr<ClassLayoutObject> layout, size_t n_members) : layout(layout), n_members(n_members) {}
		INTERBUF_API ~ClassMemberDeserializeFrameExData();
	};

	struct ArrayMemberDeserializeFrameExData {
		DataType data_type;
		size_t length = 0;
		size_t idx_member = 0;

		INTERBUF_FORCEINLINE ArrayMemberDeserializeFrameExData(DataType data_type) : data_type(data_type) {}
		INTERBUF_API ~ArrayMemberDeserializeFrameExData();
	};

	enum class DeserializeFrameType {
		StructMember = 0,
		ClassMember,
		ArrayMember,
	};

	struct DeserializeFrame {
		std::variant<std::monostate, StructMemberDeserializeFrameExData, ClassMemberDeserializeFrameExData, ArrayMemberDeserializeFrameExData> ex_data;
		DeserializeFrameType frame_type;
		char *ptr;
		size_t sz_per_element;
		DataType element_type;
	};

	struct DeserializeContext {
		peff::HashSet<peff::String> &string_pool_out;
		peff::RcObjectPtr<peff::Alloc> allocator;
		peff::List<DeserializeFrame> frames;
		Reader *reader;

		INTERBUF_FORCEINLINE DeserializeContext(
			peff::HashSet<peff::String> &string_pool_out,
			peff::Alloc *allocator,
			Reader *reader)
			: string_pool_out(string_pool_out),
			  allocator(allocator),
			  frames(allocator),
			  reader(reader) {}
		INTERBUF_API ~DeserializeContext();
	};

	INTERBUF_API ExceptionPointer _do_deserialize(DeserializeContext *context);
	INTERBUF_API ExceptionPointer deserialize_struct(peff::HashSet<peff::String> &string_pool_out, peff::Alloc *allocator, void *ptr, size_t size, Reader *reader, ObjectPtr<StructLayoutObject> root_layout);
	INTERBUF_API ExceptionPointer deserialize_class(peff::HashSet<peff::String> &string_pool_out, peff::Alloc *allocator, void *ptr, size_t size, Reader *reader, ObjectPtr<ClassLayoutObject> root_layout);
}

#endif
