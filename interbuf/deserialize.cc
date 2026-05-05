#include "deserialize.h"

using namespace interbuf;

INTERBUF_API Reader::~Reader() {
}

INTERBUF_API StructMemberDeserializeFrameExData::~StructMemberDeserializeFrameExData() {
}

INTERBUF_API ClassMemberDeserializeFrameExData::~ClassMemberDeserializeFrameExData() {
}

INTERBUF_API ArrayMemberDeserializeFrameExData::~ArrayMemberDeserializeFrameExData() {
}

INTERBUF_API DeserializeContext::~DeserializeContext() {
}

INTERBUF_FORCEINLINE ExceptionPointer _do_deserialize_by_frame_type(DeserializeContext *context, DataType i, char *cur_ptr) {
	auto &frame = context->frames.back();

	switch (i.kind) {
		case DataTypeKind::I8: {
			int8_t data;
			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_i8(data));
			*((int8_t *)cur_ptr) = data;
			break;
		}
		case DataTypeKind::I16: {
			int16_t data;

			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_i16(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			memcpy(cur_ptr, &data, sizeof(data));
			break;
		}
		case DataTypeKind::I32: {
			int32_t data;

			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_i32(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			memcpy(cur_ptr, &data, sizeof(data));
			break;
		}
		case DataTypeKind::I64: {
			int64_t data;

			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_i64(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			memcpy(cur_ptr, &data, sizeof(data));
			break;
		}
		case DataTypeKind::U8: {
			uint8_t data;

			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_u8(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			memcpy(cur_ptr, &data, sizeof(data));
			break;
		}
		case DataTypeKind::U16: {
			uint16_t data;

			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_u16(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			memcpy(cur_ptr, &data, sizeof(data));
			break;
		}
		case DataTypeKind::U32: {
			uint32_t data;

			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_u32(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			memcpy(cur_ptr, &data, sizeof(data));
			break;
		}
		case DataTypeKind::U64: {
			uint64_t data;

			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_u64(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			memcpy(cur_ptr, &data, sizeof(data));
			break;
		}
		case DataTypeKind::F32: {
			float data;

			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_f32(data));

			if (peff::get_byte_order()) {
				uint32_t d;
				d = peff::swap_byte_order(*(uint32_t *)&data);
				data = *(float *)&data;
			}

			memcpy(cur_ptr, &data, sizeof(data));
			break;
		}
		case DataTypeKind::F64: {
			double data;

			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_f64(data));

			if (peff::get_byte_order()) {
				uint64_t d;
				d = peff::swap_byte_order(*(uint64_t *)&data);
				data = *(float *)&data;
			}

			memcpy(cur_ptr, &data, sizeof(data));
			break;
		}
		case DataTypeKind::Bool: {
			bool data;
			INTERBUF_RETURN_IF_EXCEPT(context->reader->read_bool(data));
			*((bool *)cur_ptr) = data;
			break;
		}
		case DataTypeKind::Struct: {
			{
				bool is_not_null;
				INTERBUF_RETURN_IF_EXCEPT(context->reader->read_bool(is_not_null));
				if (is_not_null) {
					(*(ObjectPtr<StructBase> *)cur_ptr) = {};
					break;
				}
			}
			auto sl = i.type_def_object.cast_to<StructLayoutObject>();
			ObjectPtr<StructBase> sb = sl->constructor(context->allocator.get());

			if (!sb)
				return OutOfMemoryError::alloc();

			(*(ObjectPtr<StructBase> *)cur_ptr) = sb;

			char *data = (char *)sb.get();

			DeserializeFrame new_frame;

			new_frame.frame_type = DeserializeFrameType::StructMember;
			new_frame.ex_data = StructMemberDeserializeFrameExData(sl);
			new_frame.ptr = data;

			if (!context->frames.push_back(std::move(new_frame)))
				return OutOfMemoryError::alloc();

			break;
		}
		case DataTypeKind::Array: {
			const char *data = cur_ptr;

			DeserializeFrame new_frame;

			ArrayMemberDeserializeFrameExData ex_data(i);

			new_frame.frame_type = DeserializeFrameType::ArrayMember;

			size_t element_size;

			uint64_t len;
			{
				INTERBUF_RETURN_IF_EXCEPT(context->reader->read_u64(len));
				if (peff::get_byte_order())
					len = peff::swap_byte_order(len);
			}

			INTERBUF_RETURN_IF_EXCEPT(i.type_def_object.cast_to<ArrayDataTypeDefObject>()->deserializer(len, cur_ptr, new_frame.ptr, element_size));

			new_frame.ex_data = std::move(ex_data);

			if (!context->frames.push_back(std::move(new_frame)))
				return OutOfMemoryError::alloc();

			break;
		}
		default:
			std::terminate();
	}

	return {};
}

INTERBUF_API ExceptionPointer interbuf::_do_deserialize(DeserializeContext *context) {
	while (context->frames.size()) {
		DeserializeFrame &frame = context->frames.back();

		switch (frame.frame_type) {
			case DeserializeFrameType::StructMember: {
				StructMemberDeserializeFrameExData &ex_data = std::get<StructMemberDeserializeFrameExData>(frame.ex_data);

				if (ex_data.idx_member >= ex_data.layout->get_fields().size()) {
					context->frames.pop_back();
					return {};
				}

				auto &i = ex_data.layout->get_fields().at(ex_data.idx_member);
				char *ptr = frame.ptr + ex_data.layout->get_fields().at(ex_data.idx_member).offset;
				INTERBUF_RETURN_IF_EXCEPT(_do_deserialize_by_frame_type(
					context,
					i.type,
					ptr));

				++ex_data.idx_member;

				break;
			}
			case DeserializeFrameType::ClassMember: {
				ClassMemberDeserializeFrameExData &ex_data = std::get<ClassMemberDeserializeFrameExData>(frame.ex_data);

				if (ex_data.idx_member >= ex_data.n_members) {
					context->frames.pop_back();
					break;
				}

				uint64_t length;
				{
					INTERBUF_RETURN_IF_EXCEPT(context->reader->read_u64(length));

					if (peff::get_byte_order())
						length = peff::swap_byte_order(length);
				}

				// TODO: Add detailed information.
				if (length > SIZE_MAX)
					return FieldNameLengthError::alloc(context->allocator.get());

				peff::String name(context->allocator.get());
				if (!name.resize((size_t)length))
					return OutOfMemoryError::alloc();

				INTERBUF_RETURN_IF_EXCEPT(context->reader->read(name.data(), (size_t)length));

				auto &i = ex_data.layout->get_named_field(name);
				name.clear();

				char *cur_ptr = frame.ptr + i.offset;

				INTERBUF_RETURN_IF_EXCEPT(_do_deserialize_by_frame_type(
					context,
					i.type,
					cur_ptr));

				++ex_data.idx_member;

				break;
			}
			case DeserializeFrameType::ArrayMember: {
				ArrayMemberDeserializeFrameExData &ex_data = std::get<ArrayMemberDeserializeFrameExData>(frame.ex_data);

				if (ex_data.idx_member >= ex_data.length) {
					context->frames.pop_back();
					break;
				}

				char *cur_ptr = frame.ptr + ex_data.idx_member * frame.sz_per_element;

				INTERBUF_RETURN_IF_EXCEPT(_do_deserialize_by_frame_type(
					context,
					frame.element_type,
					cur_ptr));

				++ex_data.idx_member;

				break;
			}
		}
	}

	return {};
}

INTERBUF_API ExceptionPointer interbuf::deserialize_struct(peff::HashSet<peff::String> &string_pool_out, peff::Alloc *allocator, void *ptr, size_t size, Reader *reader, ObjectPtr<StructLayoutObject> root_layout) {
	DeserializeContext context(string_pool_out, allocator, reader);

	DeserializeFrame new_frame;

	new_frame.frame_type = DeserializeFrameType::StructMember;
	new_frame.ex_data = StructMemberDeserializeFrameExData(root_layout);
	new_frame.ptr = (char *)ptr;

	if (!context.frames.push_back(std::move(new_frame)))
		return OutOfMemoryError::alloc();

	return _do_deserialize(&context);
}

INTERBUF_API ExceptionPointer interbuf::deserialize_class(peff::HashSet<peff::String> &string_pool_out, peff::Alloc *allocator, void *ptr, size_t size, Reader *reader, ObjectPtr<ClassLayoutObject> root_layout) {
	DeserializeContext context(string_pool_out, allocator, reader);

	uint64_t n_members;
	{
		INTERBUF_RETURN_IF_EXCEPT(context.reader->read_u64(n_members));

		if (peff::get_byte_order())
			n_members = peff::swap_byte_order(n_members);
	}

	// TODO: Add detailed information.
	if (n_members > SIZE_MAX)
		return FieldNameLengthError::alloc(context.allocator.get());

	DeserializeFrame new_frame;

	new_frame.frame_type = DeserializeFrameType::ClassMember;
	new_frame.ex_data = ClassMemberDeserializeFrameExData(root_layout, n_members);
	new_frame.ptr = (char *)ptr;

	if (!context.frames.push_back(std::move(new_frame)))
		return OutOfMemoryError::alloc();

	return _do_deserialize(&context);
}
