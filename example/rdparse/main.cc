#include <interbuf/serdes.h>
#include <peff/base/deallocable.h>
#include <peff/advutils/unique_ptr.h>
#include <cstdio>

class MyWriter final : public interbuf::Writer {
private:
	FILE *_fp;

public:
	MyWriter(FILE *fp) : _fp(fp) {
	}
	virtual ~MyWriter() {
		if (_fp)
			fclose(_fp);
	}

	virtual bool write(const char *src, size_t size) override {
		if (!(fwrite(src, size, 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeI8(int8_t data) override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeI16(int16_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeI32(int32_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeI64(int64_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeU8(uint8_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeU16(uint16_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeU32(uint32_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeU64(uint64_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeF32(float data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeF64(double data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
	virtual bool writeBool(bool data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return false;
		return true;
	}
};

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

		{
			name = { peff::getDefaultAlloc() };

			if (!name.build("u8")) {
				std::terminate();
			}

			if (!(dataType = interbuf::makeObject<interbuf::I8DataTypeObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()).castTo<interbuf::DataTypeObject>())) {
				std::terminate();
			}

			field = { std::move(name), std::move(dataType), offsetof(Test, u8) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			name = { peff::getDefaultAlloc() };

			if (!name.build("u16")) {
				std::terminate();
			}

			if (!(dataType = interbuf::makeObject<interbuf::I16DataTypeObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()).castTo<interbuf::DataTypeObject>())) {
				std::terminate();
			}

			field = { std::move(name), std::move(dataType), offsetof(Test, u16) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			name = { peff::getDefaultAlloc() };

			if (!name.build("u32")) {
				std::terminate();
			}

			if (!(dataType = interbuf::makeObject<interbuf::I32DataTypeObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()).castTo<interbuf::DataTypeObject>())) {
				std::terminate();
			}

			field = { std::move(name), std::move(dataType), offsetof(Test, u32) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			name = { peff::getDefaultAlloc() };

			if (!name.build("u64")) {
				std::terminate();
			}

			if (!(dataType = interbuf::makeObject<interbuf::I64DataTypeObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()).castTo<interbuf::DataTypeObject>())) {
				std::terminate();
			}

			field = { std::move(name), std::move(dataType), offsetof(Test, u64) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		FILE *fp;

		if (!(fp = fopen("test.bin", "wb"))) {
			puts("Error opening test.bin");
			return -1;
		}

		MyWriter writer(fp);

		Test test;

		test.i8 = -12;
		test.i16 = -34;
		test.i32 = -56;
		test.i64 = -78;
		test.u8 = 0x12;
		test.u16 = 0x34;
		test.u32 = 0x56;
		test.u64 = 0x78;

		interbuf::serializeStruct(peff::getDefaultAlloc(), &test, sizeof(test), &writer, structLayout);
	}

	return 0;
}
