#include "document.h"

using namespace interbuf;

INTERBUF_API Object::Object(Document *document, peff::Alloc *allocator, ObjectType type) : _document(document), self_allocator(allocator), _type(type) {
	document->clear_deferred_destructible_objects();
}

INTERBUF_API Object::~Object() {
}

INTERBUF_API ArrayDataTypeDefObject::ArrayDataTypeDefObject(Document *document, peff::Alloc *allocator) : Object(document, allocator, ObjectType::ArrayTypeDef) {
}

INTERBUF_API ArrayDataTypeDefObject::~ArrayDataTypeDefObject() {
}

INTERBUF_API void ArrayDataTypeDefObject::dealloc() noexcept {
	peff::destroy_and_release<ArrayDataTypeDefObject>(self_allocator.get(), this, alignof(ArrayDataTypeDefObject));
}

INTERBUF_API StructLayoutObject::StructLayoutObject(Document *document, peff::Alloc *allocator) : Object(document, allocator, ObjectType::StructLayout), _fields(allocator) {
}

INTERBUF_API StructLayoutObject::~StructLayoutObject() {
}

INTERBUF_API void StructLayoutObject::dealloc() noexcept {
	peff::destroy_and_release<StructLayoutObject>(self_allocator.get(), this, alignof(StructLayoutObject));
}

INTERBUF_API bool StructLayoutObject::add_field(StructField &&field) {
	size_t index = _fields.size();

	if (!_fields.push_back(std::move(field))) {
		return false;
	}

	return true;
}

INTERBUF_API bool StructLayoutObject::insert_field(size_t index, StructField &&field) {
	size_t size = _fields.size();

	if (!_fields.insert(index, std::move(field))) {
		return false;
	}

	return true;
}

INTERBUF_API ClassLayoutObject::ClassLayoutObject(Document *document, peff::Alloc *allocator) : Object(document, allocator, ObjectType::ClassLayout), _fields(allocator), _field_name_indices(allocator) {
}

INTERBUF_API ClassLayoutObject::~ClassLayoutObject() {
}

INTERBUF_API void ClassLayoutObject::dealloc() noexcept {
	peff::destroy_and_release<ClassLayoutObject>(self_allocator.get(), this, alignof(ClassLayoutObject));
}

INTERBUF_API bool ClassLayoutObject::update_field_name_indices() noexcept {
	invalidate_field_name_indices();

	for (size_t i = 0; i < _fields.size(); ++i) {
		if (!(_field_name_indices.insert(std::string_view(_fields.at(i).name), +i))) {
			return false;
		}
	}

	_is_field_name_indices_valid = true;

	return true;
}

INTERBUF_API bool ClassLayoutObject::add_field(ClassField &&field) {
	size_t index = _fields.size();

	if (!_fields.push_back(std::move(field))) {
		return false;
	}

	if (_is_field_name_indices_valid) {
		if (!(_field_name_indices.insert(std::string_view(_fields.at(index).name), +index))) {
			return false;
		}
	}

	return true;
}

INTERBUF_API bool ClassLayoutObject::insert_field(size_t index, ClassField &&field) {
	size_t size = _fields.size();

	if (!_fields.insert(index, std::move(field))) {
		return false;
	}

	if (_is_field_name_indices_valid) {
		return update_field_name_indices();
	}

	return true;
}

INTERBUF_API void Document::_do_clear_deferred_destructible_objects() {
	Object *i, *next;

	while ((i = destructible_object_list)) {
		destructible_object_list = nullptr;

		while (i) {
			next = i->_next_destructible;
			i->_destructor(i);
			i = next;
		};
	}
}

INTERBUF_API Document::Document(peff::Alloc *allocator) : allocator(allocator) {
}

INTERBUF_API Document::~Document() {
	_do_clear_deferred_destructible_objects();
}

INTERBUF_API void interbuf::add_object_to_destructible_list(Object *ast_node, ObjectDestructor destructor) {
	ast_node->_next_destructible = ast_node->_document->destructible_object_list;
	ast_node->_destructor = destructor;
	ast_node->_document->destructible_object_list = ast_node;
}
