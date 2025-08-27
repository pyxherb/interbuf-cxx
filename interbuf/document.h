#ifndef _INTERBUF_DOCUMENT_H_
#define _INTERBUF_DOCUMENT_H_

#include "datatype.h"
#include "encode.h"
#include <peff/base/deallocable.h>
#include <peff/containers/set.h>
#include <peff/containers/map.h>
#include <peff/containers/hashmap.h>
#include <peff/advutils/shared_ptr.h>
#include <variant>

namespace interbuf {
	class Document;

	class Object;

	typedef void (*ObjectDestructor)(Object *astNode);

	template <typename T>
	using ObjectPtr = peff::SharedPtr<T>;

	class Object {
	private:
		Document *_document;
		Object *_nextDestructible;
		ObjectDestructor _destructor;

		friend INTERBUF_API void addObjectToDestructibleList(Object *astNode, ObjectDestructor destructor);
		friend class Document;

	public:
		peff::RcObjectPtr<peff::Alloc> selfAllocator;

		INTERBUF_API Object(Document *document, peff::Alloc *allocator, ObjectType type);

		virtual void dealloc() noexcept = 0;

	private:
		ObjectType _type;

	public:
		INTERBUF_FORCEINLINE ObjectType getObjectType() const {
			return _type;
		}
	};

	INTERBUF_API void addObjectToDestructibleList(Object *astNode, ObjectDestructor destructor);

	template <typename T>
	struct ObjectControlBlock : public peff::SharedPtr<T>::DefaultSharedPtrControlBlock {
		INTERBUF_FORCEINLINE ObjectControlBlock(peff::Alloc *allocator, T *ptr) noexcept : peff::SharedPtr<T>::DefaultSharedPtrControlBlock(allocator, ptr) {}
		inline virtual ~ObjectControlBlock() {}

		inline virtual void onStrongRefZero() noexcept override {
			addObjectToDestructibleList(this->ptr, [](AstNode *astNode) {
				peff::destroyAndRelease<T>(astNode->selfAllocator.get(), static_cast<T *>(astNode), alignof(T));
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

	class FieldTypeObject : public Object {
	private:
		FieldTypeKind _fieldTypeKind;
		std::variant<std::monostate, ObjectPtr<StructLayoutObject>, ObjectPtr<FieldTypeObject>> _exData;

	public:
	};

	class StructLayoutObject : public Object {
	public:
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
