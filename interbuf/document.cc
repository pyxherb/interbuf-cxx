#include "document.h"

using namespace interbuf;

INTERBUF_API ObjectMetadata::ObjectMetadata(Document *document, size_t offset) : document(document), idxMemberOffsets(document->allocator.get()) {
}

INTERBUF_API ObjectMetadata::~ObjectMetadata() {
}

INTERBUF_API void ObjectMetadata::dealloc() {
}

INTERBUF_API ArrayMetadata::ArrayMetadata(Document *document, size_t offset) : document(document), idxElementOffsets(document->allocator.get()) {
}

INTERBUF_API ArrayMetadata::~ArrayMetadata() {
}

INTERBUF_API void ArrayMetadata::dealloc() {
}

INTERBUF_API Document::Document(peff::Alloc *selfAllocator) : allocator(selfAllocator), offsetTable(selfAllocator), rawData(selfAllocator), objectMetadata(selfAllocator), arrayMetadata(selfAllocator) {
}

INTERBUF_API Document::~Document() {
}

INTERBUF_API void Document::dealloc() noexcept {
}

INTERBUF_API ExceptionPointer interbuf::parseRawIeonDataType(DocumentParseContext &context, DataType &dataTypeOut) {
	const size_t initialOffset = context.offset;

	const uint8_t *data = context.document->rawData.data();
	const size_t size = context.document->rawData.size();

	DataTypeKind rawDataType;

	if (context.offset + 1 >= size) {
		std::terminate();
	}

	rawDataType = (DataTypeKind)data[context.offset];

	++context.offset;

	switch (rawDataType) {
		case DataTypeKind::I8:
			context.offset += sizeof(int8_t);
			dataTypeOut = DataTypeKind::I8;
			break;
		case DataTypeKind::I16:
			context.offset += sizeof(int16_t);
			dataTypeOut = DataTypeKind::I16;
			break;
		case DataTypeKind::I32:
			context.offset += sizeof(int32_t);
			dataTypeOut = DataTypeKind::I32;
			break;
		case DataTypeKind::I64:
			context.offset += sizeof(int64_t);
			dataTypeOut = DataTypeKind::I64;
			break;
		case DataTypeKind::U8:
			context.offset += sizeof(uint8_t);
			dataTypeOut = DataTypeKind::U8;
			break;
		case DataTypeKind::U16:
			context.offset += sizeof(uint16_t);
			dataTypeOut = DataTypeKind::U16;
			break;
		case DataTypeKind::U32:
			context.offset += sizeof(uint32_t);
			dataTypeOut = DataTypeKind::U32;
			break;
		case DataTypeKind::U64:
			context.offset += sizeof(uint64_t);
			dataTypeOut = DataTypeKind::U64;
			break;
		case DataTypeKind::F32:
			context.offset += sizeof(uint32_t);
			dataTypeOut = DataTypeKind::F32;
			break;
		case DataTypeKind::F64:
			context.offset += sizeof(uint64_t);
			dataTypeOut = DataTypeKind::F64;
			break;
		case DataTypeKind::Binary: {
			uint64_t lenData;

			size_t szReadBuffer;
			{
				if (!decodeVarInt64(data + context.offset, size - context.offset, szReadBuffer, lenData)) {
					std::terminate();
				}

				context.offset += szReadBuffer;
			}

			dataTypeOut = DataTypeKind::Binary;
			dataTypeOut.exData.asBinary.len = lenData;
			dataTypeOut.exData.asBinary.offset = (uint8_t)szReadBuffer;

			break;
		}
		case DataTypeKind::BoolTrue:
			dataTypeOut = DataTypeKind::BoolTrue;
			break;
		case DataTypeKind::BoolFalse:
			dataTypeOut = DataTypeKind::BoolFalse;
			break;
		case DataTypeKind::Object: {
			dataTypeOut = DataTypeKind::Object;

			uint64_t lenData;

			size_t szReadBuffer;
			{
				if (!decodeVarInt64(data + context.offset, size - context.offset, szReadBuffer, lenData)) {
					std::terminate();
				}

				context.offset += szReadBuffer;
			}

			DocumentParseFrame frame;

			frame.kind = DocumentParseFrameKind::ObjectMember;
			frame.asObjectMember.nMembers = lenData;
			frame.asObjectMember.curIndex = 0;

			dataTypeOut.exData.asObject.nMembers = lenData;
			dataTypeOut.exData.asObject.offset = (uint8_t)szReadBuffer;

			std::unique_ptr<ObjectMetadata, peff::DeallocableDeleter<ObjectMetadata>> metadata;

			if ((metadata = std::unique_ptr<ObjectMetadata, peff::DeallocableDeleter<ObjectMetadata>>(
				peff::allocAndConstruct<ObjectMetadata>(
					context.document->allocator.get(),
					alignof(ObjectMetadata),
					context.document,
					context.offset)))) {
				std::terminate();
			}

			frame.asObjectMember.objectMetadata = metadata.get();

			if (!context.document->objectMetadata.insert(std::move(metadata))) {
				std::terminate();
			}

			if (!context.frames.pushBack(std::move(frame))) {
				std::terminate();
			}

			break;
		}
		case DataTypeKind::Array: {
			dataTypeOut = DataTypeKind::Array;

			uint64_t lenData;

			size_t szReadBuffer;
			{
				if (!decodeVarInt64(data + context.offset, size - context.offset, szReadBuffer, lenData)) {
					std::terminate();
				}

				context.offset += szReadBuffer;
			}

			DocumentParseFrame frame;

			frame.kind = DocumentParseFrameKind::ArrayElement;
			frame.asArrayElement.nElements = lenData;
			frame.asArrayElement.curIndex = 0;

			dataTypeOut.exData.asArray.len = lenData;
			dataTypeOut.exData.asArray.offset = (uint8_t)szReadBuffer;

			std::unique_ptr<ArrayMetadata, peff::DeallocableDeleter<ArrayMetadata>> metadata;

			if ((metadata = std::unique_ptr<ArrayMetadata, peff::DeallocableDeleter<ArrayMetadata>>(
					 peff::allocAndConstruct<ArrayMetadata>(
						 context.document->allocator.get(),
						 alignof(ArrayMetadata),
						 context.document,
						 context.offset)))) {
				std::terminate();
			}

			frame.asArrayElement.arrayMetadata = metadata.get();

			if (!context.document->arrayMetadata.insert(std::move(metadata))) {
				std::terminate();
			}

			if (!context.frames.pushBack(std::move(frame))) {
				std::terminate();
			}

			break;
		}
		default:
			std::terminate();
	}

	return parseRawIeonDataType(context, dataTypeOut);
}

INTERBUF_API ExceptionPointer interbuf::parseRawIeonField(DocumentParseContext &context, std::string_view &nameOut, DataType &dataTypeOut) {
	const size_t initialOffset = context.offset;

	const uint8_t *data = context.document->rawData.data();
	const size_t size = context.document->rawData.size();
	uint64_t lenName;

	{
		size_t szReadBuffer;
		if (!decodeVarInt64(data + context.offset, size - context.offset, szReadBuffer, lenName)) {
			std::terminate();
		}

		context.offset += szReadBuffer;
	}

	if (lenName > SIZE_MAX) {
		std::terminate();
	}

	if (((size_t)lenName) + context.offset >= size) {
		std::terminate();
	}

	context.offset += (size_t)lenName;

	nameOut = std::string_view((char *)data + context.offset, (size_t)lenName);

	

	return {};
}

INTERBUF_API ExceptionPointer interbuf::parseRawIeonDocument(peff::Alloc *allocator, Document *document) {
	ExceptionPointer e;

	DocumentParseContext context(allocator, document);

	DocumentParseFrame frame;

	frame.kind = DocumentParseFrameKind::Normal;

	if (!context.frames.pushBack(std::move(frame))) {
		std::terminate();
	}

	size_t offset = 0;

	while (context.frames.size()) {
		DocumentParseFrame &curFrame = context.frames.back();

		switch (curFrame.kind) {
			case DocumentParseFrameKind::Normal: {
				std::string_view fieldName;
				DataType dataType;
				if ((e = parseRawIeonField(context, fieldName, dataType))) {
					std::terminate();
				}

				if (!document->offsetTable.insert(document->offsetTable.size(), std::move(dataType))) {
					std::terminate();
				}

				context.frames.popBack();

				break;
			}
			case DocumentParseFrameKind::ObjectMember: {
				std::string_view fieldName;
				DataType dataType;
				if ((e = parseRawIeonField(context, fieldName, dataType))) {
					std::terminate();
				}

				if (!curFrame.asObjectMember.objectMetadata->idxMemberOffsets.insert(std::move(fieldName), document->offsetTable.size())) {
					std::terminate();
				}

				if (!document->offsetTable.insert(document->offsetTable.size(), std::move(dataType))) {
					std::terminate();
				}

				if (curFrame.asObjectMember.curIndex++ >= curFrame.asObjectMember.nMembers)
					context.frames.popBack();

				break;
			}
			case DocumentParseFrameKind::ArrayElement: {
				DataType dataType;
				if ((e = parseRawIeonDataType(context, dataType))) {
					std::terminate();
				}

				if (!curFrame.asArrayElement.arrayMetadata->idxElementOffsets.pushBack(+document->offsetTable.size())) {
					std::terminate();
				}

				if (!document->offsetTable.insert(document->offsetTable.size(), std::move(dataType))) {
					std::terminate();
				}

				if (curFrame.asArrayElement.curIndex++ >= curFrame.asArrayElement.nElements)
					context.frames.popBack();

				break;
			}
			default:
				std::terminate();
		}
	}

	return {};
}
