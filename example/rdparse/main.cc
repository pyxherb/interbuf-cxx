#include <interbuf/serdes.h>
#include <peff/base/deallocable.h>
#include <peff/advutils/unique_ptr.h>

struct Test {
	int8_t i8;
	int16_t i16;
	int32_t i32;
	int64_t i64;
	uint8_t u8;
	uint16_t u16;
	uint32_t u32;
	uint64_t u64;
};

int main() {
#ifdef _MSC_VER
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	interbuf::Document document(peff::getDefaultAlloc());
	{
		interbuf::ObjectPtr<interbuf::StructLayoutObject> structLayout;

		if (!(structLayout = interbuf::makeObject<interbuf::StructLayoutObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()))) {
			std::terminate();
		}

		peff::String name(peff::getDefaultAlloc());
		interbuf::ObjectPtr<interbuf::DataTypeObject> dataType;
		interbuf::StructField field;
		{
			name = { peff::getDefaultAlloc() };

			if (!name.build("i8")) {
				std::terminate();
			}

			if (!(dataType = interbuf::makeObject<interbuf::I8DataTypeObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()).castTo<interbuf::DataTypeObject>())) {
				std::terminate();
			}

			field = { std::move(name), std::move(dataType), offsetof(Test, i8) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			name = { peff::getDefaultAlloc() };

			if (!name.build("i16")) {
				std::terminate();
			}

			if (!(dataType = interbuf::makeObject<interbuf::I16DataTypeObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()).castTo<interbuf::DataTypeObject>())) {
				std::terminate();
			}

			field = { std::move(name), std::move(dataType), offsetof(Test, i16) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			name = { peff::getDefaultAlloc() };

			if (!name.build("i32")) {
				std::terminate();
			}

			if (!(dataType = interbuf::makeObject<interbuf::I32DataTypeObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()).castTo<interbuf::DataTypeObject>())) {
				std::terminate();
			}

			field = { std::move(name), std::move(dataType), offsetof(Test, i32) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			name = { peff::getDefaultAlloc() };

			if (!name.build("i64")) {
				std::terminate();
			}

			if (!(dataType = interbuf::makeObject<interbuf::I64DataTypeObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()).castTo<interbuf::DataTypeObject>())) {
				std::terminate();
			}

			field = { std::move(name), std::move(dataType), offsetof(Test, i64) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}
	}
}
