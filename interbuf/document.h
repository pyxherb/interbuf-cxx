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

#define interbuf_offsetof(p, m) ((size_t)(((char*)&((p)->m)) - ((char *)p)))

namespace interbuf {
	class Document;

	class Object;

	struct StructBase {
	};

	class ClassBase {
	};

	typedef void (*ObjectDestructor)(Object *ast_node);

	template <typename T>
	using ObjectPtr = peff::SharedPtr<T>;

	INTERBUF_API void add_object_to_destructible_list(Object *ast_node, ObjectDestructor destructor);

	class Object {
	private:
		Document *_document;
		Object *_next_destructible;
		ObjectDestructor _destructor;

		friend class Document;
		friend INTERBUF_API void interbuf::add_object_to_destructible_list(Object *ast_node, ObjectDestructor destructor);

	public:
		peff::RcObjectPtr<peff::Alloc> self_allocator;

		INTERBUF_API Object(Document *document, peff::Alloc *allocator, ObjectType type);
		INTERBUF_API virtual ~Object();

		virtual void dealloc() noexcept = 0;

	private:
		ObjectType _type;

	public:
		INTERBUF_FORCEINLINE ObjectType get_object_type() const {
			return _type;
		}
	};

	template <typename T>
	struct ObjectControlBlock : public peff::SharedPtr<T>::DefaultSharedPtrControlBlock {
		INTERBUF_FORCEINLINE ObjectControlBlock(peff::Alloc *allocator, T *ptr) noexcept : peff::SharedPtr<T>::DefaultSharedPtrControlBlock(allocator, ptr) {}
		inline virtual ~ObjectControlBlock() {}

		inline virtual void on_strong_ref_zero() noexcept override {
			add_object_to_destructible_list(this->ptr, [](Object *object) {
				peff::destroy_and_release<T>(object->self_allocator.get(), static_cast<T *>(object), alignof(T));
			});
		}

		inline virtual void on_ref_zero() noexcept override {
			peff::destroy_and_release<ObjectControlBlock<T>>(this->allocator.get(), this, alignof(ObjectControlBlock<T>));
		}
	};

	template <typename T, typename... Args>
	INTERBUF_FORCEINLINE ObjectPtr<T> make_object(peff::Alloc *allocator, Args &&...args) {
		return peff::make_shared_with_control_block<T, ObjectControlBlock<T>>(allocator, std::forward<Args>(args)...);
	}

	class StructLayoutObject;

	struct DataType {
		DataTypeKind kind;
		ObjectPtr<Object> type_def_object;
	};

	struct StructField {
		DataType type;
		size_t offset;

		INTERBUF_FORCEINLINE StructField() : type({}), offset(0) {}
		INTERBUF_FORCEINLINE StructField(StructField &&rhs) : type(std::move(rhs.type)), offset(std::move(rhs.offset)) {}
		INTERBUF_FORCEINLINE StructField(DataType type, size_t offset) : type(type), offset(offset) {}
		~StructField() = default;

		INTERBUF_FORCEINLINE StructField &operator=(StructField &&rhs) noexcept {
			type = std::move(rhs.type);
			offset = rhs.offset;

			return *this;
		}
	};

	/// @brief The blank structure instance constructor type.
	/// @param allocator Allocator for object creation.
	/// @return The created object, nullptr if failed (OOM occurred.)
	typedef ObjectPtr<StructBase> (*BlankStructConstructor)(peff::Alloc *allocator);

	/// @brief The blank class instance constructor type.
	/// @param allocator Allocator for object creation.
	/// @return The created object, nullptr if failed (OOM occurred.)
	typedef ObjectPtr<ClassBase> (*BlankClassConstructor)(peff::Alloc *allocator);

	class StructLayoutObject final : public Object {
	private:
		peff::DynArray<StructField> _fields;

	public:
		BlankStructConstructor constructor = nullptr;

		INTERBUF_API StructLayoutObject(Document *document, peff::Alloc *allocator);
		INTERBUF_API virtual ~StructLayoutObject();

		INTERBUF_API virtual void dealloc() noexcept override;

		[[nodiscard]] INTERBUF_API bool add_field(StructField &&field);
		[[nodiscard]] INTERBUF_API bool insert_field(size_t index, StructField &&field);

		INTERBUF_FORCEINLINE const decltype(_fields) &get_fields() const {
			return _fields;
		}
	};

	struct ClassField {
		std::string_view name;
		DataType type;
		size_t offset;

		INTERBUF_FORCEINLINE ClassField() : name(), type({}), offset(0) {}
		INTERBUF_FORCEINLINE ClassField(ClassField &&rhs) : name(std::move(rhs.name)), type(std::move(rhs.type)), offset(std::move(rhs.offset)) {}
		INTERBUF_FORCEINLINE ClassField(const std::string_view &name, DataType type, size_t offset) : name(name), type(type), offset(offset) {}
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
		peff::HashMap<std::string_view, size_t> _field_name_indices;
		bool _is_field_name_indices_valid = true;

	public:
		BlankClassConstructor constructor = nullptr;

		INTERBUF_API ClassLayoutObject(Document *document, peff::Alloc *allocator);
		INTERBUF_API virtual ~ClassLayoutObject();

		INTERBUF_API virtual void dealloc() noexcept override;

		INTERBUF_FORCEINLINE bool is_field_name_indices_built() const noexcept {
			return _is_field_name_indices_valid;
		}

		INTERBUF_FORCEINLINE void invalidate_field_name_indices() noexcept {
			_field_name_indices.clear();
			_is_field_name_indices_valid = false;
		}
		[[nodiscard]] INTERBUF_API bool update_field_name_indices() noexcept;

		[[nodiscard]] INTERBUF_API bool add_field(ClassField &&field);
		[[nodiscard]] INTERBUF_API bool insert_field(size_t index, ClassField &&field);

		INTERBUF_FORCEINLINE const decltype(_fields) &get_fields() const {
			return _fields;
		}

		INTERBUF_FORCEINLINE ClassField &get_named_field(const std::string_view &name) {
			assert(_is_field_name_indices_valid);
			return _fields.at(_field_name_indices.at(name));
		}

		INTERBUF_FORCEINLINE const ClassField &get_named_field(const std::string_view &name) const {
			assert(_is_field_name_indices_valid);
			return _fields.at(_field_name_indices.at(name));
		}
	};

	typedef void (*ArraySerializer)(
		const void *ptr,	   // Pointer to the array structure
		const char *&ptr_out,   // Pointer out to the array data
		size_t &sz_element_out,  // Element size out
		size_t &length_out	   // Length out
	);
	typedef ExceptionPointer (*ArrayDeserializer)(
		size_t n_elements,	  // Element number
		void *ptr,			  // Pointer to the array structure
		char *&ptr_out,		  // Pointer out to the array data
		size_t &sz_element_out  // Element size out
	);

	class ArrayDataTypeDefObject final : public Object {
	public:
		DataType element_type;
		ArraySerializer serializer = nullptr;
		ArrayDeserializer deserializer = nullptr;

		INTERBUF_API ArrayDataTypeDefObject(Document *document, peff::Alloc *allocator);
		INTERBUF_API ~ArrayDataTypeDefObject();

		INTERBUF_API void dealloc() noexcept override;
	};

	class Document {
	private:
		INTERBUF_API void _do_clear_deferred_destructible_objects();

	public:
		peff::RcObjectPtr<peff::Alloc> allocator;
		Object *destructible_object_list = nullptr;

		INTERBUF_API Document(peff::Alloc *allocator);
		INTERBUF_API virtual ~Document();

		INTERBUF_FORCEINLINE void clear_deferred_destructible_objects() {
			if (destructible_object_list) {
				_do_clear_deferred_destructible_objects();
			}
		}
	};
}

#endif
