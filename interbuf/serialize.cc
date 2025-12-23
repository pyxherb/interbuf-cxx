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

INTERBUF_API ExceptionPointer interbuf::_doSerialize(SerializeContext *context) {
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

				switch (i.type.kind) {
					case DataTypeKind::I8: {
						int8_t data = *(int8_t *)curPtr;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI8(data));
						break;
					}
					case DataTypeKind::I16: {
						int16_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI16(data));
						break;
					}
					case DataTypeKind::I32: {
						int32_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI32(data));
						break;
					}
					case DataTypeKind::I64: {
						int64_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI64(data));
						break;
					}
					case DataTypeKind::U8: {
						uint8_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU8(data));
						break;
					}
					case DataTypeKind::U16: {
						uint16_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU16(data));
						break;
					}
					case DataTypeKind::U32: {
						uint32_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU32(data));
						break;
					}
					case DataTypeKind::U64: {
						uint64_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU64(data));
						break;
					}
					case DataTypeKind::F32: {
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
					case DataTypeKind::F64: {
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
					case DataTypeKind::Bool: {
						bool data = *(bool *)curPtr;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeBool(data));
						break;
					}
					case DataTypeKind::Struct: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						newFrame.frameType = SerializeFrameType::StructMember;
						newFrame.exData = StructMemberSerializeFrameExData(i.type.typeDefObject.castTo<StructLayoutObject>());
						newFrame.ptr = (const char*)((const ObjectPtr<StructBase>*)data)->get();

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case DataTypeKind::Class: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						newFrame.frameType = SerializeFrameType::ClassMember;
						newFrame.exData = ClassMemberSerializeFrameExData(i.type.typeDefObject.castTo<ClassLayoutObject>());
						newFrame.ptr = (const char *)((const ObjectPtr<ClassBase> *)data)->get();

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case DataTypeKind::Array: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						auto type = i.type.typeDefObject.castTo<ArrayDataTypeDefObject>();

						ArrayMemberSerializeFrameExData exData(type);

						newFrame.frameType = SerializeFrameType::ArrayMember;

						size_t elementSize;

						type->serializer(curPtr, newFrame.ptr, elementSize, exData.length);

						newFrame.szPerElement = elementSize;
						newFrame.exData = std::move(exData);
						newFrame.elementType = type->elementType;

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
			case SerializeFrameType::ClassMember: {
				ClassMemberSerializeFrameExData &exData = std::get<ClassMemberSerializeFrameExData>(frame.exData);

				if (exData.idxMember >= exData.layout->getFields().size()) {
					context->frames.popBack();
					break;
				}

				auto &i = exData.layout->getFields().at(exData.idxMember);

				const char *curPtr = frame.ptr + i.offset;

				{
					uint64_t data;

					if (i.name.size() > UINT64_MAX)
						return FieldNameLengthError::alloc(context->allocator.get());

					data = (uint64_t)i.name.size();

					if (peff::getByteOrder())
						data = peff::swapByteOrder(data);

					INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU64(data));
				}
				INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->write(i.name.data(), i.name.size()));

				switch (i.type.kind) {
					case DataTypeKind::I8: {
						int8_t data = *(int8_t *)curPtr;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI8(data));
						break;
					}
					case DataTypeKind::I16: {
						int16_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI16(data));
						break;
					}
					case DataTypeKind::I32: {
						int32_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI32(data));
						break;
					}
					case DataTypeKind::I64: {
						int64_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI64(data));
						break;
					}
					case DataTypeKind::U8: {
						uint8_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU8(data));
						break;
					}
					case DataTypeKind::U16: {
						uint16_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU16(data));
						break;
					}
					case DataTypeKind::U32: {
						uint32_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU32(data));
						break;
					}
					case DataTypeKind::U64: {
						uint64_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU64(data));
						break;
					}
					case DataTypeKind::F32: {
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
					case DataTypeKind::F64: {
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
					case DataTypeKind::Bool: {
						bool data = *(bool *)curPtr;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeBool(data));
						break;
					}
					case DataTypeKind::Struct: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						newFrame.frameType = SerializeFrameType::StructMember;
						newFrame.exData = StructMemberSerializeFrameExData(i.type.typeDefObject.castTo<StructLayoutObject>());
						newFrame.ptr = (const char *)((const ObjectPtr<StructBase> *)data)->get();

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case DataTypeKind::Class: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						newFrame.frameType = SerializeFrameType::ClassMember;
						newFrame.exData = ClassMemberSerializeFrameExData(i.type.typeDefObject.castTo<ClassLayoutObject>());
						newFrame.ptr = (const char *)((const ObjectPtr<ClassBase> *)data)->get();

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case DataTypeKind::Array: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						auto type = i.type.typeDefObject.castTo<ArrayDataTypeDefObject>();

						ArrayMemberSerializeFrameExData exData(type);

						newFrame.frameType = SerializeFrameType::ArrayMember;

						size_t elementSize;

						type->serializer(curPtr, newFrame.ptr, elementSize, exData.length);

						newFrame.szPerElement = elementSize;
						newFrame.exData = std::move(exData);
						newFrame.elementType = type->elementType;

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
			case SerializeFrameType::ArrayMember: {
				ArrayMemberSerializeFrameExData &exData = std::get<ArrayMemberSerializeFrameExData>(frame.exData);

				if (!exData.idxMember) {
					uint64_t len = exData.length;
					if (peff::getByteOrder())
						len = peff::swapByteOrder(len);
					INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU64(len));
				}
				if (exData.idxMember >= exData.length) {
					context->frames.popBack();
					break;
				}

				const char *curPtr = frame.ptr + exData.idxMember * frame.szPerElement;

				switch (frame.elementType.kind) {
					case DataTypeKind::I8: {
						int8_t data = *(int8_t *)curPtr;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI8(data));
						break;
					}
					case DataTypeKind::I16: {
						int16_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI16(data));
						break;
					}
					case DataTypeKind::I32: {
						int32_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI32(data));
						break;
					}
					case DataTypeKind::I64: {
						int64_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeI64(data));
						break;
					}
					case DataTypeKind::U8: {
						uint8_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU8(data));
						break;
					}
					case DataTypeKind::U16: {
						uint16_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU16(data));
						break;
					}
					case DataTypeKind::U32: {
						uint32_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU32(data));
						break;
					}
					case DataTypeKind::U64: {
						uint64_t data;

						memcpy(&data, curPtr, sizeof(data));

						if (peff::getByteOrder())
							data = peff::swapByteOrder(data);

						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeU64(data));
						break;
					}
					case DataTypeKind::F32: {
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
					case DataTypeKind::F64: {
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
					case DataTypeKind::Bool: {
						bool data = *(bool *)curPtr;
						INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(context->allocator.get(), context->writer->writeBool(data));
						break;
					}
					case DataTypeKind::Struct: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						newFrame.frameType = SerializeFrameType::StructMember;
						newFrame.exData = StructMemberSerializeFrameExData(frame.elementType.typeDefObject.castTo<StructLayoutObject>());
						newFrame.ptr = (const char *)((const ObjectPtr<StructBase> *)data)->get();

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case DataTypeKind::Class: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						newFrame.frameType = SerializeFrameType::ClassMember;
						newFrame.exData = ClassMemberSerializeFrameExData(frame.elementType.typeDefObject.castTo<ClassLayoutObject>());
						newFrame.ptr = (const char *)((const ObjectPtr<ClassBase> *)data)->get();

						if (!context->frames.pushBack(std::move(newFrame)))
							return OutOfMemoryError::alloc();

						break;
					}
					case DataTypeKind::Array: {
						const char *data = curPtr;

						SerializeFrame newFrame;

						auto type = frame.elementType.typeDefObject.castTo<ArrayDataTypeDefObject>();

						ArrayMemberSerializeFrameExData exData(type);

						newFrame.frameType = SerializeFrameType::ArrayMember;

						size_t elementSize;

						type->serializer(curPtr, newFrame.ptr, elementSize, exData.length);
						;
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

INTERBUF_API ExceptionPointer interbuf::serializeStruct(peff::Alloc *allocator, const void *ptr, Writer *writer, ObjectPtr<StructLayoutObject> rootLayout) {
	SerializeContext context(allocator, writer);

	SerializeFrame newFrame;

	newFrame.frameType = SerializeFrameType::StructMember;
	newFrame.exData = StructMemberSerializeFrameExData(rootLayout);
	newFrame.ptr = (const char *)ptr;

	if (!context.frames.pushBack(std::move(newFrame)))
		return OutOfMemoryError::alloc();

	return _doSerialize(&context);
}

INTERBUF_API ExceptionPointer interbuf::serializeClass(peff::Alloc *allocator, const void *ptr, Writer *writer, ObjectPtr<ClassLayoutObject> rootLayout) {
	SerializeContext context(allocator, writer);

	if (rootLayout->getFields().size() >= SIZE_MAX)
		std::terminate();

	INTERBUF_RETURN_EXCEPT_IF_WRITE_FAILED(
		context.allocator.get(), context.writer->writeU64(
									 peff::getByteOrder()
										 ? peff::swapByteOrder((uint64_t)rootLayout->getFields().size())
										 : (uint64_t)rootLayout->getFields().size()));

	SerializeFrame newFrame;

	newFrame.frameType = SerializeFrameType::ClassMember;
	newFrame.exData = ClassMemberSerializeFrameExData(rootLayout);
	newFrame.ptr = (const char *)ptr;

	if (!context.frames.pushBack(std::move(newFrame)))
		return OutOfMemoryError::alloc();

	return _doSerialize(&context);
}
