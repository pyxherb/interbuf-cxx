#include "serdes.h"

using namespace interbuf;

INTERBUF_API StructMemberSerializeFrameExData::~StructMemberSerializeFrameExData() {
}

INTERBUF_API SerializeContext::~SerializeContext() {
}

INTERBUF_API ExceptionPointer interbuf::_doSerializeStruct(SerializeContext* context) {
	while (context->frames.size() > 1) {
		SerializeFrame &frame = context->frames.back();

		switch (frame.frameType) {
			case SerializeFrameType::StructMember: {
				StructMemberSerializeFrameExData &exData = std::get<StructMemberSerializeFrameExData>(frame.exData);

				for (auto &i : exData.layout->getFields()) {

				}

				break;
			}
		}
	}
}

INTERBUF_API ExceptionPointer interbuf::serializeStruct(const void* ptr, size_t size, ObjectPtr<StructLayoutObject> rootLayout) {

}
