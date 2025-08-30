#ifndef _INTERBUF_SERDES_H_
#define _INTERBUF_SERDES_H_

#include "document.h"

namespace interbuf {
	struct StructMemberSerializeFrameExData {
		ObjectPtr<StructLayoutObject> layout;
		size_t idxMember = 0;

		INTERBUF_FORCEINLINE StructMemberSerializeFrameExData(ObjectPtr<StructLayoutObject> layout) : layout(layout) {}
		INTERBUF_API ~StructMemberSerializeFrameExData();
	};

	enum class SerializeFrameType {
		StructMember = 0,
	};

	struct SerializeFrame {
		std::variant<std::monostate, StructMemberSerializeFrameExData> exData;
		SerializeFrameType frameType;
	};

	struct SerializeContext {
		peff::List<SerializeFrame> frames;
		const char *ptr;
		size_t size;
		size_t offset = 0;

		INTERBUF_FORCEINLINE SerializeContext(
			peff::Alloc *allocator,
			const char *ptr,
			size_t size)
			: frames(allocator),
			  ptr(ptr),
			  size(size) {}
		INTERBUF_API ~SerializeContext();
	};

	INTERBUF_API ExceptionPointer _doSerializeStruct(SerializeContext *context);
	INTERBUF_API ExceptionPointer serializeStruct(const void *ptr, size_t size, ObjectPtr<StructLayoutObject> rootLayout);
}

#endif
