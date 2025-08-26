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

	class ObjectMetadata {
	public:
		Document *document;
		size_t offset;
		peff::HashMap<std::string_view, size_t> idxMemberOffsets;

		INTERBUF_API ObjectMetadata(Document *document, size_t offset);
		INTERBUF_API ~ObjectMetadata();

		INTERBUF_API void dealloc() noexcept;
	};

	class ArrayMetadata {
	public:
		Document *document;
		size_t offset;
		peff::DynArray<size_t> idxElementOffsets;

		INTERBUF_API ArrayMetadata(Document *document, size_t offset);
		INTERBUF_API ~ArrayMetadata();

		INTERBUF_API void dealloc() noexcept;
	};

	class Document {
	public:
		peff::RcObjectPtr<peff::Alloc> allocator;
		peff::Map<size_t, DataType> offsetTable;
		peff::DynArray<uint8_t> rawData;
		peff::Set<std::unique_ptr<ObjectMetadata, peff::DeallocableDeleter<ObjectMetadata>>> objectMetadata;
		peff::Set<std::unique_ptr<ArrayMetadata, peff::DeallocableDeleter<ArrayMetadata>>> arrayMetadata;

		INTERBUF_API Document(peff::Alloc *selfAllocator);
		INTERBUF_API ~Document();

		INTERBUF_API void dealloc() noexcept;
	};

	enum class DocumentParseFrameKind {
		Normal = 0,
		ObjectMember,
		ObjectMemberEnd,
		ArrayElement,
		ArrayElementEnd,
	};

	struct DocumentParseFrame {
		union {
			struct {
				ObjectMetadata *objectMetadata;
				size_t nMembers;
				size_t curIndex;
			} asObjectMember;
			struct {
				ArrayMetadata *arrayMetadata;
				size_t nElements;
				size_t curIndex;
			} asArrayElement;
		};

		DocumentParseFrameKind kind;
	};

	struct DocumentParseContext {
		Document *document;
		size_t offset = 0;
		peff::List<DocumentParseFrame> frames;

		INTERBUF_FORCEINLINE DocumentParseContext(peff::Alloc *allocator, Document *document) : document(document), frames(allocator) {}
	};

	INTERBUF_API ExceptionPointer parseRawIeonDataType(DocumentParseContext &context, DataType &dataTypeOut);
	INTERBUF_API ExceptionPointer parseRawIeonField(DocumentParseContext &context, std::string_view &nameOut, DataType &dataTypeOut);

	INTERBUF_API ExceptionPointer parseRawIeonDocument(peff::Alloc *allocator, Document *document);
}

#endif
