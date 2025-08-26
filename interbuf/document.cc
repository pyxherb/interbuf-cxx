#include "document.h"

using namespace interbuf;

INTERBUF_API ObjectIndex::ObjectIndex(Document *document, size_t offset) : document(document), idxMemberOffsets(document->allocator.get()) {
}

INTERBUF_API ObjectIndex::~ObjectIndex() {
}

INTERBUF_API void ObjectIndex::dealloc() {
}

INTERBUF_API Document::Document(peff::Alloc *selfAllocator) : allocator(selfAllocator), offsetTable(selfAllocator), rootMemberIndices(selfAllocator), rawData(selfAllocator), objectIndices(selfAllocator) {
}

INTERBUF_API Document::~Document() {
}

INTERBUF_API void Document::dealloc() noexcept {
}

INTERBUF_API ExceptionPointer interbuf::parseRawIeonField(Document* document, size_t &offset, std::string_view& nameOut, DataType& dataTypeOut) {
	const size_t initialOffset = offset;

	const uint8_t *data = document->rawData.data();
	const size_t size = document->rawData.size();
	uint64_t lenName;

	{
		size_t szReadBuffer;
		if (!decodeVarInt64(data + offset, size - offset, szReadBuffer, lenName)) {
			std::terminate();
		}

		offset += szReadBuffer;
	}

	if (lenName > SIZE_MAX) {
		std::terminate();
	}

	if (((size_t)lenName) + offset >= size) {
		std::terminate();
	}

	offset += (size_t)lenName;

	nameOut = std::string_view((char*)data + offset, (size_t)lenName);

	DataTypeKind rawDataType;

	if (offset + 1 >= size) {
		std::terminate();
	}

	rawDataType = (DataTypeKind)data[offset];

	++offset;

	switch (rawDataType) {
		case DataTypeKind::I8:
			offset += sizeof(int8_t);
			dataTypeOut = DataTypeKind::I8;
			break;
		case DataTypeKind::I16:
			offset += sizeof(int16_t);
			dataTypeOut = DataTypeKind::I16;
			break;
		case DataTypeKind::I32:
			offset += sizeof(int32_t);
			dataTypeOut = DataTypeKind::I32;
			break;
		case DataTypeKind::I64:
			offset += sizeof(int64_t);
			dataTypeOut = DataTypeKind::I64;
			break;
		case DataTypeKind::U8:
			offset += sizeof(uint8_t);
			dataTypeOut = DataTypeKind::U8;
			break;
		case DataTypeKind::U16:
			offset += sizeof(uint16_t);
			dataTypeOut = DataTypeKind::U16;
			break;
		case DataTypeKind::U32:
			offset += sizeof(uint32_t);
			dataTypeOut = DataTypeKind::U32;
			break;
		case DataTypeKind::U64:
			offset += sizeof(uint64_t);
			dataTypeOut = DataTypeKind::U64;
			break;
		case DataTypeKind::F32:
			offset += sizeof(uint32_t);
			dataTypeOut = DataTypeKind::F32;
			break;
		case DataTypeKind::F64:
			offset += sizeof(uint64_t);
			dataTypeOut = DataTypeKind::F64;
			break;
		case DataTypeKind::Binary: {
			uint64_t lenData;

			size_t szReadBuffer;
			{
				if (!decodeVarInt64(data + offset, size - offset, szReadBuffer, lenName)) {
					std::terminate();
				}

				offset += szReadBuffer;
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
				if (!decodeVarInt64(data + offset, size - offset, szReadBuffer, lenName)) {
					std::terminate();
				}

				offset += szReadBuffer;
			}

			dataTypeOut.exData.asObject.nMembers = lenData;
			dataTypeOut.exData.asObject.offset = (uint8_t)szReadBuffer;

			break;
		}
		case DataTypeKind::Array: {
			dataTypeOut = DataTypeKind::Array;

			uint64_t lenData;

			size_t szReadBuffer;
			{
				if (!decodeVarInt64(data + offset, size - offset, szReadBuffer, lenName)) {
					std::terminate();
				}

				offset += szReadBuffer;
			}

			dataTypeOut.exData.asArray.len = lenData;
			dataTypeOut.exData.asArray.offset = (uint8_t)szReadBuffer;

			break;
		}
		default:
			std::terminate();
	}

	return {};
}

INTERBUF_API ExceptionPointer interbuf::parseRawIeonDocument(Document *document) {
	ExceptionPointer e;

	size_t offset = 0;

	std::string_view fieldName;
	DataType dataType;
	if ((e = parseRawIeonField(document, offset, fieldName, dataType))) {
		std::terminate();
	}
}
