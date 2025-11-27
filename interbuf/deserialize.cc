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

INTERBUF_API ExceptionPointer interbuf::_doDeserialize(DeserializeContext *context) {
	while (context->frames.size()) {
		DeserializeFrame &frame = context->frames.back();

		switch (frame.frameType) {
			case DeserializeFrameType::StructMember: {
				StructMemberDeserializeFrameExData &exData = std::get<StructMemberDeserializeFrameExData>(frame.exData);

				if (exData.idxMember >= exData.layout->getFields().size()) {
					context->frames.popBack();
					break;
				}

				auto &i = exData.layout->getFields().at(exData.idxMember);

				char *curPtr = frame.ptr + i.offset;

				switch (i.type->getFieldTypeKind()) {
					case FieldTypeKind::I8: {
						int8_t data;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI8(data));
						*((int8_t *)curPtr) = data;
						break;
					}
					case FieldTypeKind::I16: {
						int16_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI16(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::I32: {
						int32_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI32(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::I64: {
						int64_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI64(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U8: {
						uint8_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU8(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U16: {
						uint16_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU16(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U32: {
						uint32_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU32(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U64: {
						uint64_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU64(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::F32: {
						float data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readF32(data));

						if (peff::getByteOrder()) {
							uint32_t d;
							d = peff::swapByteOrder(*(uint32_t *)&data);
							data = *(float *)&data;
						}

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::F64: {
						double data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readF64(data));

						if (peff::getByteOrder()) {
							uint64_t d;
							d = peff::swapByteOrder(*(uint64_t *)&data);
							data = *(float *)&data;
						}

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::Bool: {
						bool data;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readBool(data));
						*((bool *)curPtr) = data;
						break;
					}
					case FieldTypeKind::Struct: {
						char *data = curPtr;

						DeserializeFrame newFrame;

						newFrame.frameType = DeserializeFrameType::StructMember;
						newFrame.exData = StructMemberDeserializeFrameExData(i.type.castTo<StructDataTypeObject>()->structLayout);
						newFrame.ptr = data;

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case FieldTypeKind::Array: {
						const char *data = curPtr;

						DeserializeFrame newFrame;

						auto type = i.type.castTo<ArrayDataTypeObject>();

						ArrayMemberDeserializeFrameExData exData(type);

						newFrame.frameType = DeserializeFrameType::ArrayMember;

						size_t elementSize;

						uint64_t len;
						{
							INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU64(len));
							if (peff::getByteOrder())
								len = peff::swapByteOrder(len);
						}

						INTERBUF_RETURN_IF_EXCEPT(type->deserializer(len, curPtr, newFrame.ptr, elementSize));

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
			case DeserializeFrameType::ClassMember: {
				ClassMemberDeserializeFrameExData &exData = std::get<ClassMemberDeserializeFrameExData>(frame.exData);

				if (exData.idxMember >= exData.nMembers) {
					context->frames.popBack();
					break;
				}

				uint64_t length;
				{
					INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU64(length));

					if (peff::getByteOrder())
						length = peff::swapByteOrder(length);
				}

				// TODO: Add detailed information.
				if (length > SIZE_MAX)
					return FieldNameLengthError::alloc(context->allocator.get());

				peff::String name(context->allocator.get());
				if (!name.resize((size_t)length))
					return OutOfMemoryError::alloc();

				INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->read(name.data(), (size_t)length));

				auto &i = exData.layout->getNamedField(name);
				name.clear();

				char *curPtr = frame.ptr + i.offset;

				switch (i.type->getFieldTypeKind()) {
					case FieldTypeKind::I8: {
						int8_t data;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI8(data));
						*((int8_t *)curPtr) = data;
						break;
					}
					case FieldTypeKind::I16: {
						int16_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI16(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::I32: {
						int32_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI32(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::I64: {
						int64_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI64(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U8: {
						uint8_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU8(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U16: {
						uint16_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU16(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U32: {
						uint32_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU32(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U64: {
						uint64_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU64(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::F32: {
						float data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readF32(data));

						if (peff::getByteOrder()) {
							uint32_t d;
							d = peff::swapByteOrder(*(uint32_t *)&data);
							data = *(float *)&data;
						}

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::F64: {
						double data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readF64(data));

						if (peff::getByteOrder()) {
							uint64_t d;
							d = peff::swapByteOrder(*(uint64_t *)&data);
							data = *(float *)&data;
						}

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::Bool: {
						bool data;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readBool(data));
						*((bool *)curPtr) = data;
						break;
					}
					case FieldTypeKind::Struct: {
						char *data = curPtr;

						DeserializeFrame newFrame;

						newFrame.frameType = DeserializeFrameType::StructMember;
						newFrame.exData = StructMemberDeserializeFrameExData(i.type.castTo<StructDataTypeObject>()->structLayout);
						newFrame.ptr = data;

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case FieldTypeKind::Array: {
						const char *data = curPtr;

						DeserializeFrame newFrame;

						auto type = i.type.castTo<ArrayDataTypeObject>();

						ArrayMemberDeserializeFrameExData exData(type);

						newFrame.frameType = DeserializeFrameType::ArrayMember;

						size_t elementSize;

						uint64_t len;
						{
							INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU64(len));
							if (peff::getByteOrder())
								len = peff::swapByteOrder(len);
						}

						INTERBUF_RETURN_IF_EXCEPT(type->deserializer(len, curPtr, newFrame.ptr, elementSize));

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
			case DeserializeFrameType::ArrayMember: {
				ArrayMemberDeserializeFrameExData &exData = std::get<ArrayMemberDeserializeFrameExData>(frame.exData);

				if (exData.idxMember >= exData.length) {
					context->frames.popBack();
					break;
				}

				char *curPtr = frame.ptr + exData.idxMember * frame.szPerElement;

				switch (frame.elementType->getFieldTypeKind()) {
					case FieldTypeKind::I8: {
						int8_t data;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI8(data));
						*((int8_t *)curPtr) = data;
						break;
					}
					case FieldTypeKind::I16: {
						int16_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI16(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::I32: {
						int32_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI32(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::I64: {
						int64_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readI64(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U8: {
						uint8_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU8(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U16: {
						uint16_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU16(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U32: {
						uint32_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU32(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::U64: {
						uint64_t data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU64(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::F32: {
						float data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readF32(data));

						if (peff::getByteOrder()) {
							uint32_t d;
							d = peff::swapByteOrder(*(uint32_t *)&data);
							data = *(float *)&data;
						}

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::F64: {
						double data;

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readF64(data));

						if (peff::getByteOrder()) {
							uint64_t d;
							d = peff::swapByteOrder(*(uint64_t *)&data);
							data = *(float *)&data;
						}

						memcpy(curPtr, &data, sizeof(data));
						break;
					}
					case FieldTypeKind::Bool: {
						bool data;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readBool(data));
						*((bool *)curPtr) = data;
						break;
					}
					case FieldTypeKind::Struct: {
						char *data = curPtr;

						DeserializeFrame newFrame;

						newFrame.frameType = DeserializeFrameType::StructMember;
						newFrame.exData = StructMemberDeserializeFrameExData(frame.elementType.castTo<StructDataTypeObject>()->structLayout);
						newFrame.ptr = data;

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case FieldTypeKind::Array: {
						const char *data = curPtr;

						DeserializeFrame newFrame;

						auto type = frame.elementType.castTo<ArrayDataTypeObject>();

						ArrayMemberDeserializeFrameExData exData(type);

						newFrame.frameType = DeserializeFrameType::ArrayMember;

						size_t elementSize;

						uint64_t len;
						{
							if (peff::getByteOrder())
								len = peff::swapByteOrder(len);
							INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->reader->readU64(len));
						}

						INTERBUF_RETURN_IF_EXCEPT(type->deserializer(len, curPtr, newFrame.ptr, elementSize));

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

INTERBUF_API ExceptionPointer interbuf::deserializeStruct(peff::Alloc *allocator, void *ptr, size_t size, Reader *reader, ObjectPtr<StructLayoutObject> rootLayout) {
	DeserializeContext context(allocator, reader);

	DeserializeFrame newFrame;

	newFrame.frameType = DeserializeFrameType::StructMember;
	newFrame.exData = StructMemberDeserializeFrameExData(rootLayout);
	newFrame.ptr = (char *)ptr;

	if (!context.frames.pushBack(std::move(newFrame)))
		return OutOfMemoryError::alloc();

	return _doDeserialize(&context);
}

INTERBUF_API ExceptionPointer interbuf::deserializeClass(peff::Alloc *allocator, void *ptr, size_t size, Reader *reader, ObjectPtr<ClassLayoutObject> rootLayout) {
	DeserializeContext context(allocator, reader);

	uint64_t nMembers;
	{
		INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context.allocator.get(), context.reader->readU64(nMembers));

		if (peff::getByteOrder())
			nMembers = peff::swapByteOrder(nMembers);
	}

	// TODO: Add detailed information.
	if (nMembers > SIZE_MAX)
		return FieldNameLengthError::alloc(context.allocator.get());

	DeserializeFrame newFrame;

	newFrame.frameType = DeserializeFrameType::ClassMember;
	newFrame.exData = ClassMemberDeserializeFrameExData(rootLayout, nMembers);
	newFrame.ptr = (char *)ptr;

	if (!context.frames.pushBack(std::move(newFrame)))
		return OutOfMemoryError::alloc();

	return _doDeserialize(&context);
}
