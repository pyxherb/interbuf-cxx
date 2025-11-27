#ifndef _INTERBUF_DOCUMENT_H_
#define _INTERBUF_DOCUMENT_H_

#include "datatype.h"
#include "varint_encode.h"
#include <peff/base/deallocable.h>
#include <peff/containers/set.h>
#include <peff/containers/map.h>
#include <peff/containers/hashmap.h>
#include <peff/containers/string.h>
#include <peff/advutils/shared_ptr.h>
#include <variant>

namespace interbuf {
	class Document;

	class Object;

	typedef void (*ObjectDestructor)(Object *astNode);

	template <typename T>
	using ObjectPtr = peff::SharedPtr<T>;

	INTERBUF_API void addObjectToDestructibleList(Object *astNode, ObjectDestructor destructor);

	class Object {
	private:
		Document *_document;
		Object *_nextDestructible;
		ObjectDestructor _destructor;

		friend class Document;
		friend INTERBUF_API void interbuf::addObjectToDestructibleList(Object *astNode, ObjectDestructor destructor);

	public:
		peff::RcObjectPtr<peff::Alloc> selfAllocator;

		INTERBUF_API Object(Document *document, peff::Alloc *allocator, ObjectType type);
		INTERBUF_API virtual ~Object();

		virtual void dealloc() noexcept = 0;

	private:
		ObjectType _type;

	public:
		INTERBUF_FORCEINLINE ObjectType getObjectType() const {
			return _type;
		}
	};

	template <typename T>
	struct ObjectControlBlock : public peff::SharedPtr<T>::DefaultSharedPtrControlBlock {
		INTERBUF_FORCEINLINE ObjectControlBlock(peff::Alloc *allocator, T *ptr) noexcept : peff::SharedPtr<T>::DefaultSharedPtrControlBlock(allocator, ptr) {}
		inline virtual ~ObjectControlBlock() {}

		inline virtual void onStrongRefZero() noexcept override {
			addObjectToDestructibleList(this->ptr, [](Object *object) {
				peff::destroyAndRelease<T>(object->selfAllocator.get(), static_cast<T *>(object), alignof(T));
			});
		}

		inline virtual void onRefZero() noexcept override {
			peff::destroyAndRelease<ObjectControlBlock<T>>(this->allocator.get(), this, alignof(ObjectControlBlock<T>));
		}
	};

	template <typename T, typename... Args>
	INTERBUF_FORCEINLINE ObjectPtr<T> makeObject(peff::Alloc *allocator, Args &&...args) {
		return peff::makeSharedWithControlBlock<T, ObjectControlBlock<T>>(allocator, std::forward<Args>(args)...);
	}

	class StructLayoutObject;

	class DataTypeObject : public Object {
	private:
		FieldTypeKind _fieldTypeKind;

	public:
		INTERBUF_API DataTypeObject(Document *document, peff::Alloc *allocator, FieldTypeKind fieldTypeKind);
		INTERBUF_API virtual ~DataTypeObject();

		INTERBUF_FORCEINLINE FieldTypeKind getFieldTypeKind() {
			return _fieldTypeKind;
		}
	};

	template <FieldTypeKind ftk>
	class SimpleDataTypeObject final : public DataTypeObject {
	public:
		INTERBUF_FORCEINLINE SimpleDataTypeObject(Document *document, peff::Alloc *allocator) : DataTypeObject(document, allocator, ftk) {}
		inline virtual ~SimpleDataTypeObject() {}

		inline virtual void dealloc() noexcept override {
			peff::destroyAndRelease<SimpleDataTypeObject<ftk>>(selfAllocator.get(), this, alignof(SimpleDataTypeObject<ftk>));
		}
	};

	using I8DataTypeObject = SimpleDataTypeObject<FieldTypeKind::I8>;
	using I16DataTypeObject = SimpleDataTypeObject<FieldTypeKind::I16>;
	using I32DataTypeObject = SimpleDataTypeObject<FieldTypeKind::I32>;
	using I64DataTypeObject = SimpleDataTypeObject<FieldTypeKind::I64>;
	using U8DataTypeObject = SimpleDataTypeObject<FieldTypeKind::U8>;
	using U16DataTypeObject = SimpleDataTypeObject<FieldTypeKind::U16>;
	using U32DataTypeObject = SimpleDataTypeObject<FieldTypeKind::U32>;
	using U64DataTypeObject = SimpleDataTypeObject<FieldTypeKind::U64>;
	using F32DataTypeObject = SimpleDataTypeObject<FieldTypeKind::F32>;
	using F64DataTypeObject = SimpleDataTypeObject<FieldTypeKind::F64>;
	using StringDataTypeObject = SimpleDataTypeObject<FieldTypeKind::String>;
	using BoolDataTypeObject = SimpleDataTypeObject<FieldTypeKind::Bool>;

	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::I8);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::I16);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::I32);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::I64);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::U8);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::U16);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::U32);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::U64);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::F32);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::F64);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::String);
	INTERBUF_DECL_EXPLICIT_INSTANTIATED_CLASS(INTERBUF_API, SimpleDataTypeObject, FieldTypeKind::Bool);

	struct StructField {
		ObjectPtr<DataTypeObject> type;
		size_t offset;

		INTERBUF_FORCEINLINE StructField() : type({}), offset(0) {}
		INTERBUF_FORCEINLINE StructField(StructField &&rhs) : type(std::move(rhs.type)), offset(std::move(rhs.offset)) {}
		INTERBUF_FORCEINLINE StructField(ObjectPtr<DataTypeObject> type, size_t offset) : type(type), offset(offset) {}
		~StructField() = default;

		INTERBUF_FORCEINLINE StructField &operator=(StructField &&rhs) noexcept {
			type = std::move(rhs.type);
			offset = rhs.offset;

			return *this;
		}
	};

	class StructLayoutObject final : public Object {
	private:
		peff::DynArray<StructField> _fields;

	public:
		INTERBUF_API StructLayoutObject(Document *document, peff::Alloc *allocator);
		INTERBUF_API virtual ~StructLayoutObject();

		INTERBUF_API virtual void dealloc() noexcept override;

		[[nodiscard]] INTERBUF_API bool addField(StructField &&field);
		[[nodiscard]] INTERBUF_API bool insertField(size_t index, StructField &&field);

		INTERBUF_FORCEINLINE const decltype(_fields) &getFields() const {
			return _fields;
		}
	};

	class StructDataTypeObject final : public DataTypeObject {
	public:
		ObjectPtr<StructLayoutObject> structLayout;

		INTERBUF_API StructDataTypeObject(Document *document, peff::Alloc *allocator);
		INTERBUF_API ~StructDataTypeObject();

		INTERBUF_API void dealloc() noexcept override;
	};

	struct ClassField {
		peff::String name;
		ObjectPtr<DataTypeObject> type;
		size_t offset;

		INTERBUF_FORCEINLINE ClassField() : name(nullptr), type({}), offset(0) {}
		INTERBUF_FORCEINLINE ClassField(ClassField &&rhs) : name(std::move(rhs.name)), type(std::move(rhs.type)), offset(std::move(rhs.offset)) {}
		INTERBUF_FORCEINLINE ClassField(peff::String &&name, ObjectPtr<DataTypeObject> type, size_t offset) : name(std::move(name)), type(type), offset(offset) {}
		~ClassField() = default;

		INTERBUF_FORCEINLINE ClassField &operator=(ClassField &&rhs) noexcept {
			name = std::move(rhs.name);
			type = std::move(rhs.type);
			offset = rhs.offset;

			return *this;
		}
	};

	class ClassLayoutObject final : public Object {
	private:
		peff::DynArray<ClassField> _fields;
		peff::HashMap<std::string_view, size_t> _fieldNameIndices;
		bool _isFieldNameIndicesValid = true;

	public:
		INTERBUF_API ClassLayoutObject(Document *document, peff::Alloc *allocator);
		INTERBUF_API virtual ~ClassLayoutObject();

		INTERBUF_API virtual void dealloc() noexcept override;

		INTERBUF_FORCEINLINE bool isFieldNameIndicesBuilt() const noexcept {
			return _isFieldNameIndicesValid;
		}

		INTERBUF_FORCEINLINE void invalidateFieldNameIndices() noexcept {
			_fieldNameIndices.clear();
			_isFieldNameIndicesValid = false;
		}
		[[nodiscard]] INTERBUF_API bool updateFieldNameIndices() noexcept;

		[[nodiscard]] INTERBUF_API bool addField(ClassField &&field);
		[[nodiscard]] INTERBUF_API bool insertField(size_t index, ClassField &&field);

		INTERBUF_FORCEINLINE const decltype(_fields) &getFields() const {
			return _fields;
		}

		INTERBUF_FORCEINLINE ClassField &getNamedField(const std::string_view &name) {
			assert(_isFieldNameIndicesValid);
			return _fields.at(_fieldNameIndices.at(name));
		}

		INTERBUF_FORCEINLINE const ClassField &getNamedField(const std::string_view &name) const {
			assert(_isFieldNameIndicesValid);
			return _fields.at(_fieldNameIndices.at(name));
		}
	};

	class ClassDataTypeObject final : public DataTypeObject {
	public:
		ObjectPtr<ClassLayoutObject> classLayout;

		INTERBUF_API ClassDataTypeObject(Document *document, peff::Alloc *allocator);
		INTERBUF_API ~ClassDataTypeObject();

		INTERBUF_API void dealloc() noexcept override;
	};

	typedef void (*ArraySerializer)(
		const void *ptr,	   // Pointer to the array structure
		const char *&ptrOut,   // Pointer out to the array data
		size_t &szElementOut,  // Element size out
		size_t &lengthOut	   // Length out
	);
	typedef ExceptionPointer (*ArrayDeserializer)(
		size_t nElements,	 // Element number
		void *ptr,			 // Pointer to the array structure
		char *&ptrOut,		 // Pointer out to the array data
		size_t &szElementOut	 // Element size out
	);

	class ArrayDataTypeObject final : public DataTypeObject {
	public:
		ObjectPtr<DataTypeObject> elementType;
		ArraySerializer serializer = nullptr;
		ArrayDeserializer deserializer = nullptr;

		INTERBUF_API ArrayDataTypeObject(Document *document, peff::Alloc *allocator);
		INTERBUF_API ~ArrayDataTypeObject();

		INTERBUF_API void dealloc() noexcept override;
	};

	class Document {
	private:
		INTERBUF_API void _doClearDeferredDestructibleObjects();

	public:
		peff::RcObjectPtr<peff::Alloc> allocator;
		Object *destructibleObjectList = nullptr;

		INTERBUF_API Document(peff::Alloc *allocator);
		INTERBUF_API virtual ~Document();

		INTERBUF_FORCEINLINE void clearDeferredDestructibleObjects() {
			if (destructibleObjectList) {
				_doClearDeferredDestructibleObjects();
			}
		}
	};
}

#endif
