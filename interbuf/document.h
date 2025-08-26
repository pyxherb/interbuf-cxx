#ifndef _INTERBUF_DOCUMENT_H_
#define _INTERBUF_DOCUMENT_H_

#include "datatype.h"
#include "encode.h"
#include <peff/base/deallocable.h>
#include <peff/containers/set.h>
#include <peff/containers/map.h>
#include <peff/containers/hashmap.h>

namespace interbuf {
	class Document;

	class ObjectIndex {
	public:
		Document *document;
		size_t offset;
		peff::HashMap<std::string_view, size_t> idxMemberOffsets;

		INTERBUF_API ObjectIndex(Document *document, size_t offset);
		INTERBUF_API ~ObjectIndex();

		INTERBUF_API void dealloc() noexcept;
	};

	class Document {
	public:
		peff::RcObjectPtr<peff::Alloc> allocator;
		peff::Map<size_t, DataType> offsetTable;
		peff::DynArray<uint8_t> rawData;
		peff::DynArray<std::unique_ptr<ObjectIndex, peff::DeallocableDeleter<ObjectIndex>>> objectIndices;

		INTERBUF_API Document(peff::Alloc *selfAllocator);
		INTERBUF_API ~Document();

		INTERBUF_API void dealloc() noexcept;
	};

	INTERBUF_API ExceptionPointer parseRawIeonField(Document *document, size_t &offset, std::string_view &nameOut, DataType &dataTypeOut);

	INTERBUF_API ExceptionPointer parseRawIeonDocument(Document *document);
}

#endif
