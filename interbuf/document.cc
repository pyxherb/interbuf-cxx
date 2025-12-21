#include "document.h"

using namespace interbuf;

INTERBUF_API Object::Object(Document *document, peff::Alloc *allocator, ObjectType type) : _document(document), selfAllocator(allocator), _type(type) {
	document->clearDeferredDestructibleObjects();
}

INTERBUF_API Object::~Object() {
}

INTERBUF_API ArrayDataTypeDefObject::ArrayDataTypeDefObject(Document *document, peff::Alloc *allocator) : Object(document, allocator, ObjectType::ArrayTypeDef) {
}

INTERBUF_API ArrayDataTypeDefObject::~ArrayDataTypeDefObject() {
}

INTERBUF_API void ArrayDataTypeDefObject::dealloc() noexcept {
	peff::destroyAndRelease<ArrayDataTypeDefObject>(selfAllocator.get(), this, alignof(ArrayDataTypeDefObject));
}

INTERBUF_API StructLayoutObject::StructLayoutObject(Document *document, peff::Alloc *allocator) : Object(document, allocator, ObjectType::StructLayout), _fields(allocator) {
}

INTERBUF_API StructLayoutObject::~StructLayoutObject() {
}

INTERBUF_API void StructLayoutObject::dealloc() noexcept {
	peff::destroyAndRelease<StructLayoutObject>(selfAllocator.get(), this, alignof(StructLayoutObject));
}

INTERBUF_API bool StructLayoutObject::addField(StructField &&field) {
	size_t index = _fields.size();

	if (!_fields.pushBack(std::move(field))) {
		return false;
	}

	return true;
}

INTERBUF_API bool StructLayoutObject::insertField(size_t index, StructField &&field) {
	size_t size = _fields.size();

	if (!_fields.insert(index, std::move(field))) {
		return false;
	}

	return true;
}

INTERBUF_API ClassLayoutObject::ClassLayoutObject(Document *document, peff::Alloc *allocator) : Object(document, allocator, ObjectType::ClassLayout), _fields(allocator), _fieldNameIndices(allocator) {
}

INTERBUF_API ClassLayoutObject::~ClassLayoutObject() {
}

INTERBUF_API void ClassLayoutObject::dealloc() noexcept {
	peff::destroyAndRelease<ClassLayoutObject>(selfAllocator.get(), this, alignof(ClassLayoutObject));
}

INTERBUF_API bool ClassLayoutObject::updateFieldNameIndices() noexcept {
	invalidateFieldNameIndices();

	for (size_t i = 0; i < _fields.size(); ++i) {
		if (!(_fieldNameIndices.insert(std::string_view(_fields.at(i).name), +i))) {
			return false;
		}
	}

	_isFieldNameIndicesValid = true;

	return true;
}

INTERBUF_API bool ClassLayoutObject::addField(ClassField &&field) {
	size_t index = _fields.size();

	if (!_fields.pushBack(std::move(field))) {
		return false;
	}

	if (_isFieldNameIndicesValid) {
		if (!(_fieldNameIndices.insert(std::string_view(_fields.at(index).name), +index))) {
			return false;
		}
	}

	return true;
}

INTERBUF_API bool ClassLayoutObject::insertField(size_t index, ClassField &&field) {
	size_t size = _fields.size();

	if (!_fields.insert(index, std::move(field))) {
		return false;
	}

	if (_isFieldNameIndicesValid) {
		return updateFieldNameIndices();
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
