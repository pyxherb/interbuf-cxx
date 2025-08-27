#include "document.h"

using namespace interbuf;

INTERBUF_API Object::Object(Document *document, peff::Alloc *allocator, ObjectType type) : _document(document), selfAllocator(allocator), _type(type) {
}

INTERBUF_API void Document::_doClearDeferredDestructibleObjects() {
	Object *i, *next;

	while ((i = destructibleObjectList)) {
		destructibleObjectList = nullptr;

		while (i) {
			next = i->_nextDestructible;
			i->_destructor(i);
			i = next;
		};
	}
}

INTERBUF_API Document::Document(peff::Alloc *allocator) : allocator(allocator) {
}

INTERBUF_API Document::~Document() {
	_doClearDeferredDestructibleObjects();
}

INTERBUF_API void interbuf::addObjectToDestructibleList(Object* astNode, ObjectDestructor destructor) {
	astNode->_nextDestructible = astNode->_document->destructibleObjectList;
	astNode->_destructor = destructor;
	astNode->_document->destructibleObjectList = astNode;
}
