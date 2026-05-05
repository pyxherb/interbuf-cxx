#include "serialize.h"

using namespace interbuf;

INTERBUF_API Writer::~Writer() {
}

INTERBUF_API StructMemberSerializeFrameExData::~StructMemberSerializeFrameExData() {
}

INTERBUF_API ClassMemberSerializeFrameExData::~ClassMemberSerializeFrameExData() {
}

INTERBUF_API ArrayMemberSerializeFrameExData::~ArrayMemberSerializeFrameExData() {
}

INTERBUF_API SerializeContext::~SerializeContext() {
}

INTERBUF_FORCEINLINE ExceptionPointer _do_serialize_by_frame_type(SerializeContext *context, DataType type, const char *cur_ptr) {
	switch (type.kind) {
		case DataTypeKind::I8: {
			int8_t data = *(int8_t *)cur_ptr;
			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_i8(data));
			break;
		}
		case DataTypeKind::I16: {
			int16_t data;

			memcpy(&data, cur_ptr, sizeof(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_i16(data));
			break;
		}
		case DataTypeKind::I32: {
			int32_t data;

			memcpy(&data, cur_ptr, sizeof(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_i32(data));
			break;
		}
		case DataTypeKind::I64: {
			int64_t data;

			memcpy(&data, cur_ptr, sizeof(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_i64(data));
			break;
		}
		case DataTypeKind::U8: {
			uint8_t data;

			memcpy(&data, cur_ptr, sizeof(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_u8(data));
			break;
		}
		case DataTypeKind::U16: {
			uint16_t data;

			memcpy(&data, cur_ptr, sizeof(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_u16(data));
			break;
		}
		case DataTypeKind::U32: {
			uint32_t data;

			memcpy(&data, cur_ptr, sizeof(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_u32(data));
			break;
		}
		case DataTypeKind::U64: {
			uint64_t data;

			memcpy(&data, cur_ptr, sizeof(data));

			if (peff::get_byte_order())
				data = peff::swap_byte_order(data);

			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_u64(data));
			break;
		}
		case DataTypeKind::F32: {
			float data;

			memcpy(&data, cur_ptr, sizeof(data));

			if (peff::get_byte_order()) {
				uint32_t d;
				d = peff::swap_byte_order(*(uint32_t *)&data);
				data = *(float *)&data;
			}

			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_f32(data));
			break;
		}
		case DataTypeKind::F64: {
			double data;

			memcpy(&data, cur_ptr, sizeof(data));

			if (peff::get_byte_order()) {
				uint64_t d;
				d = peff::swap_byte_order(*(uint64_t *)&data);
				data = *(double *)&data;
			}

			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_f64(data));
			break;
		}
		case DataTypeKind::Bool: {
			bool data = *(bool *)cur_ptr;
			INTERBUF_RETURN_IF_EXCEPT(context->writer->write_bool(data));
			break;
		}
		case DataTypeKind::Struct: {
			const char *data = cur_ptr;

			StructBase *p = ((const ObjectPtr<StructBase> *)data)->get();

			if (p) {
				INTERBUF_RETURN_IF_EXCEPT(context->writer->write_bool(false));
			} else {
				INTERBUF_RETURN_IF_EXCEPT(context->writer->write_bool(true));

				SerializeFrame new_frame;

				new_frame.frame_type = SerializeFrameType::StructMember;
				new_frame.ex_data = StructMemberSerializeFrameExData(type.type_def_object.cast_to<StructLayoutObject>());
				new_frame.ptr = (const char *)p;

				if (!context->frames.push_back(std::move(new_frame)))
					return OutOfMemoryError::alloc();
			}

			break;
		}
		case DataTypeKind::Class: {
			const char *data = cur_ptr;

			ClassBase *p = ((const ObjectPtr<ClassBase> *)data)->get();

			if (p) {
				INTERBUF_RETURN_IF_EXCEPT(context->writer->write_bool(false));
			} else {
				INTERBUF_RETURN_IF_EXCEPT(context->writer->write_bool(true));
				SerializeFrame new_frame;

				new_frame.frame_type = SerializeFrameType::ClassMember;
				new_frame.ex_data = ClassMemberSerializeFrameExData(type.type_def_object.cast_to<ClassLayoutObject>());
				new_frame.ptr = (const char *)p;

				if (!context->frames.push_back(std::move(new_frame)))
					return OutOfMemoryError::alloc();
			}

			break;
		}
		case DataTypeKind::Array: {
			const char *data = cur_ptr;

			SerializeFrame new_frame;

			auto td = type.type_def_object.cast_to<ArrayDataTypeDefObject>();

			ArrayMemberSerializeFrameExData ex_data(td);

			new_frame.frame_type = SerializeFrameType::ArrayMember;

			size_t element_size;

			td->serializer(cur_ptr, new_frame.ptr, element_size, ex_data.length);

			new_frame.sz_per_element = element_size;
			new_frame.ex_data = std::move(ex_data);
			new_frame.element_type = td->element_type;

			if (!context->frames.push_back(std::move(new_frame)))
				return OutOfMemoryError::alloc();

			break;
		}
		default:
			std::terminate();
	}

	return {};
}

INTERBUF_API ExceptionPointer interbuf::_do_serialize(SerializeContext *context) {
	while (context->frames.size()) {
		SerializeFrame &frame = context->frames.back();

		switch (frame.frame_type) {
			case SerializeFrameType::StructMember: {
				StructMemberSerializeFrameExData &ex_data = std::get<StructMemberSerializeFrameExData>(frame.ex_data);

				if (ex_data.idx_member >= ex_data.layout->get_fields().size()) {
					context->frames.pop_back();
					break;
				}

				auto &i = ex_data.layout->get_fields().at(ex_data.idx_member);

				const char *cur_ptr = frame.ptr + i.offset;

				INTERBUF_RETURN_IF_EXCEPT(_do_serialize_by_frame_type(context, i.type, cur_ptr));

				++ex_data.idx_member;

				break;
			}
			case SerializeFrameType::ClassMember: {
				ClassMemberSerializeFrameExData &ex_data = std::get<ClassMemberSerializeFrameExData>(frame.ex_data);

				if (ex_data.idx_member >= ex_data.layout->get_fields().size()) {
					context->frames.pop_back();
					break;
				}

				auto &i = ex_data.layout->get_fields().at(ex_data.idx_member);

				const char *cur_ptr = frame.ptr + i.offset;

				{
					uint64_t data;

					if (i.name.size() > UINT64_MAX)
						return FieldNameLengthError::alloc(context->allocator.get());

					data = (uint64_t)i.name.size();

					if (peff::get_byte_order())
						data = peff::swap_byte_order(data);

					INTERBUF_RETURN_IF_EXCEPT(context->writer->write_u64(data));
				}
				INTERBUF_RETURN_IF_EXCEPT(context->writer->write(i.name.data(), i.name.size()));

				INTERBUF_RETURN_IF_EXCEPT(_do_serialize_by_frame_type(context, i.type, cur_ptr));

				++ex_data.idx_member;

				break;
			}
			case SerializeFrameType::ArrayMember: {
				ArrayMemberSerializeFrameExData &ex_data = std::get<ArrayMemberSerializeFrameExData>(frame.ex_data);

				if (!ex_data.idx_member) {
					uint64_t len = ex_data.length;
					if (peff::get_byte_order())
						len = peff::swap_byte_order(len);
					INTERBUF_RETURN_IF_EXCEPT(context->writer->write_u64(len));
				}
				if (ex_data.idx_member >= ex_data.length) {
					context->frames.pop_back();
					break;
				}

				const char *cur_ptr = frame.ptr + ex_data.idx_member * frame.sz_per_element;

				INTERBUF_RETURN_IF_EXCEPT(_do_serialize_by_frame_type(context, frame.element_type, cur_ptr));

				++ex_data.idx_member;

				break;
			}
		}
	}

	return {};
}

INTERBUF_API ExceptionPointer interbuf::serialize_struct(peff::Alloc *allocator, const void *ptr, Writer *writer, ObjectPtr<StructLayoutObject> root_layout) {
	SerializeContext context(allocator, writer);

	SerializeFrame new_frame;

	new_frame.frame_type = SerializeFrameType::StructMember;
	new_frame.ex_data = StructMemberSerializeFrameExData(root_layout);
	new_frame.ptr = (const char *)ptr;

	if (!context.frames.push_back(std::move(new_frame)))
		return OutOfMemoryError::alloc();

	return _do_serialize(&context);
}

INTERBUF_API ExceptionPointer interbuf::serialize_class(peff::Alloc *allocator, const void *ptr, Writer *writer, ObjectPtr<ClassLayoutObject> root_layout) {
	SerializeContext context(allocator, writer);

	if (root_layout->get_fields().size() >= SIZE_MAX)
		std::terminate();

	INTERBUF_RETURN_IF_EXCEPT(context.writer->write_u64(
		peff::get_byte_order()
			? peff::swap_byte_order((uint64_t)root_layout->get_fields().size())
			: (uint64_t)root_layout->get_fields().size()));

	SerializeFrame new_frame;

	new_frame.frame_type = SerializeFrameType::ClassMember;
	new_frame.ex_data = ClassMemberSerializeFrameExData(root_layout);
	new_frame.ptr = (const char *)ptr;

	if (!context.frames.push_back(std::move(new_frame)))
		return OutOfMemoryError::alloc();

	return _do_serialize(&context);
}
