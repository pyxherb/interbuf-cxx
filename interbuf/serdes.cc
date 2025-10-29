#include "serdes.h"

using namespace interbuf;

INTERBUF_API Reader::~Reader() {
}

INTERBUF_API Writer::~Writer() {
}

INTERBUF_API StructMemberSerializeFrameExData::~StructMemberSerializeFrameExData() {
}

INTERBUF_API ArrayMemberSerializeFrameExData::~ArrayMemberSerializeFrameExData() {
}

INTERBUF_API SerializeContext::~SerializeContext() {
}

INTERBUF_API ExceptionPointer interbuf::_doSerializeStruct(SerializeContext *context) {
	while (context->frames.size()) {
		SerializeFrame &frame = context->frames.back();

		switch (frame.frameType) {
			case SerializeFrameType::StructMember: {
				StructMemberSerializeFrameExData &exData = std::get<StructMemberSerializeFrameExData>(frame.exData);

				if (exData.idxMember >= exData.layout->getFields().size()) {
					context->frames.popBack();
					break;
				}

				auto &i = exData.layout->getFields().at(exData.idxMember);

				const char *curPtr = frame.ptr + i.offset;

				switch (i.type->getFieldTypeKind()) {
					case FieldTypeKind::I8: {
						int8_t data = *(int8_t *)curPtr;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI8(data));
						break;
					}
					case FieldTypeKind::I16: {
						int16_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI16(data));
						break;
					}
					case FieldTypeKind::I32: {
						int32_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI32(data));
						break;
					}
					case FieldTypeKind::I64: {
						int64_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI64(data));
						break;
					}
					case FieldTypeKind::U8: {
						uint8_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU8(data));
						break;
					}
					case FieldTypeKind::U16: {
						uint16_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU16(data));
						break;
					}
					case FieldTypeKind::U32: {
						uint32_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU32(data));
						break;
					}
					case FieldTypeKind::U64: {
						uint64_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU64(data));
						break;
					}
					case FieldTypeKind::F32: {
						float data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder()) {
							uint32_t d;
							d = peff::swapByteOrder(*(uint32_t *)&data);
							data = *(float *)&data;
						}

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeF32(data));
						break;
					}
					case FieldTypeKind::F64: {
						double data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder()) {
							uint64_t d;
							d = peff::swapByteOrder(*(uint64_t *)&data);
							data = *(double *)&data;
						}

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeF64(data));
						break;
					}
					case FieldTypeKind::Bool: {
						bool data = *(bool *)curPtr;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeBool(data));
						break;
					}
					case FieldTypeKind::Struct: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						newFrame.frameType = SerializeFrameType::StructMember;
						newFrame.exData = StructMemberSerializeFrameExData(i.type.castTo<StructDataTypeObject>()->structLayout);
						newFrame.ptr = data;
						newFrame.size = frame.size - i.offset;

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case FieldTypeKind::Array: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						auto type = i.type.castTo<ArrayDataTypeObject>();

						ArrayMemberSerializeFrameExData exData(type);

						newFrame.frameType = SerializeFrameType::ArrayElement;

						size_t elementSize;

						type->serializer(curPtr, newFrame.ptr, elementSize, exData.length);

						newFrame.size = elementSize * exData.length;
						newFrame.exData = std::move(exData);

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					default:
						std::terminate();
				}

				++exData.idxMember;

				break;
			}
		}
	}

	return {};
}

INTERBUF_API ExceptionPointer interbuf::serializeStruct(peff::Alloc *allocator, const void *ptr, size_t size, Writer *writer, ObjectPtr<StructLayoutObject> rootLayout) {
	SerializeContext context(allocator, writer);

	SerializeFrame newFrame;

	newFrame.frameType = SerializeFrameType::StructMember;
	newFrame.exData = StructMemberSerializeFrameExData(rootLayout);
	newFrame.ptr = (const char*)ptr;
	newFrame.size = size;

	if (!context.frames.pushBack(std::move(newFrame)))
		return OutOfMemoryError::alloc();

	return _doSerializeStruct(&context);
}
