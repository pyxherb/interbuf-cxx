#include "document.h"

using namespace interbuf;

INTERBUF_API Object::Object(Document *document, peff::Alloc *allocator, ObjectType type) : _document(document), selfAllocator(allocator), _type(type) {
	document->clearDeferredDestructibleObjects();
}

INTERBUF_API Object::~Object() {
}

INTERBUF_API DataTypeObject::DataTypeObject(Document *document, peff::Alloc *allocator, FieldTypeKind fieldTypeKind) : Object(document, allocator, ObjectType::DataType), _fieldTypeKind(fieldTypeKind) {
}

INTERBUF_API DataTypeObject::~DataTypeObject() {
}

INTERBUF_API StructLayoutObject::StructLayoutObject(Document* document, peff::Alloc* allocator): Object(document, allocator, ObjectType::StructLayout), structFields(allocator), fieldNameIndices(allocator) {

}

INTERBUF_API StructLayoutObject::~StructLayoutObject() {

}

INTERBUF_API bool StructLayoutObject::updateFieldNameIndices() {
	fieldNameIndices.clear();

	for (size_t i = 0; i < structFields.size(); ++i) {
		if (!(fieldNameIndices.insert(structFields.at(i).name, +i))) {
			return false;
		}
	}

	return true;
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

INTERBUF_API void interbuf::addObjectToDestructibleList(Object *astNode, ObjectDestructor destructor) {
	astNode->_nextDestructible = astNode->_document->destructibleObjectList;
	astNode->_destructor = destructor;
	astNode->_document->destructibleObjectList = astNode;
}
