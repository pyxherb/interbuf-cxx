#include <interbuf/serialize.h>
#include <interbuf/deserialize.h>
#include <peff/base/deallocable.h>
#include <peff/advutils/unique_ptr.h>
#include <cstdio>

class MyWriter final : public interbuf::Writer {
private:
	FILE *_fp;

public:
	peff::RcObjectPtr<peff::Alloc> allocator;
	MyWriter(peff::Alloc *allocator, FILE *fp) : allocator(allocator), _fp(fp) {
	}
	virtual ~MyWriter() {
		if (_fp)
			fclose(_fp);
	}

	virtual interbuf::ExceptionPointer write(const char *src, size_t size) override {
		if (!(fwrite(src, size, 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeI8(int8_t data) override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeI16(int16_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeI32(int32_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeI64(int64_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeU8(uint8_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeU16(uint16_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeU32(uint32_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeU64(uint64_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeF32(float data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeF64(double data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer writeBool(bool data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
};

class MyReader final : public interbuf::Reader {
private:
	FILE *_fp;

public:
	peff::RcObjectPtr<peff::Alloc> allocator;
	MyReader(peff::Alloc *allocator, FILE *fp) : allocator(allocator), _fp(fp) {
	}
	virtual ~MyReader() {
		if (_fp)
			fclose(_fp);
	}

	virtual interbuf::ExceptionPointer read(char *buffer, size_t size) noexcept override {
		if (!(fread(buffer, size, 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readI8(int8_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readI16(int16_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readI32(int32_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readI64(int64_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readU8(uint8_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readU16(uint16_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readU32(uint32_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readU64(uint64_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readF32(float &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readF64(double &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer readBool(bool &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::withOOMErrorIfAllocFailed(interbuf::IOError::alloc(allocator.get()));
		return {};
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
	uint8_t s[16];
};

struct ClassTest {
	uint32_t u32;
	float f32;
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

		interbuf::StructField field;
		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::I8 }, interbuf_offsetof(Test, i8) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::I16 }, interbuf_offsetof(Test, i16) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::I32 }, interbuf_offsetof(Test, i32) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::I64 }, interbuf_offsetof(Test, i64) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::U8 }, interbuf_offsetof(Test, u8) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::U16 }, interbuf_offsetof(Test, u16) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::U32 }, interbuf_offsetof(Test, u32) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::U64 }, interbuf_offsetof(Test, u64) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		{
			interbuf::ObjectPtr<interbuf::ArrayDataTypeDefObject> arrayDataType;

			if (!(arrayDataType = interbuf::makeObject<interbuf::ArrayDataTypeDefObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()))) {
				std::terminate();
			}

			arrayDataType->elementType = interbuf::DataType{ interbuf::DataTypeKind::U8 };

			arrayDataType->serializer = [](const void *ptr,
											const char *&ptrOut,
											size_t &szElementOut,
											size_t &lengthOut) {
				ptrOut = (const char *)ptr;
				szElementOut = sizeof(uint8_t);
				lengthOut = std::size(std::declval<Test>().s);
			};

			arrayDataType->deserializer = [](size_t nElements,
											  void *ptr,
											  char *&ptrOut,
											  size_t &szElementOut) -> interbuf::ExceptionPointer {
				szElementOut = sizeof(uint8_t);
				ptrOut = (char *)ptr;
				return {};
			};

			field = { interbuf::DataType{ interbuf::DataTypeKind::Array, arrayDataType.castTo<interbuf::Object>() }, interbuf_offsetof(Test, s) };

			if (!structLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		FILE *fp;

		if (!(fp = fopen("test.bin", "wb"))) {
			puts("Error opening test.bin");
			return -1;
		}

		Test test;

		test.i8 = -12;
		test.i16 = -34;
		test.i32 = -56;
		test.i64 = -78;
		test.u8 = 0x12;
		test.u16 = 0x34;
		test.u32 = 0x56;
		test.u64 = 0x78;
		memset(test.s, 0xa1, sizeof(test.s));

		{
			MyWriter writer(peff::getDefaultAlloc(), fp);

			interbuf::serializeStruct(peff::getDefaultAlloc(), &test, &writer, structLayout);
		}

		FILE *fp2;

		if (!(fp2 = fopen("test.bin", "rb"))) {
			puts("Error opening test.bin");
			return -1;
		}

		Test test2;

		{
			MyReader reader(peff::getDefaultAlloc(), fp2);

			interbuf::deserializeStruct(peff::getDefaultAlloc(), &test2, sizeof(test2), &reader, structLayout);
		}

		assert(test2.i8 == -12);
		assert(test2.i16 == -34);
		assert(test2.i32 == -56);
		assert(test2.i64 == -78);
		assert(test2.u8 == 0x12);
		assert(test2.u16 == 0x34);
		assert(test2.u32 == 0x56);
		assert(test2.u64 == 0x78);

		for (size_t i = 0; i < std::size(test.s); ++i)
			assert(test.s[i] == 0xa1);
	}

	{
		interbuf::ObjectPtr<interbuf::ClassLayoutObject> classLayout;

		if (!(classLayout = interbuf::makeObject<interbuf::ClassLayoutObject>(peff::getDefaultAlloc(), &document, peff::getDefaultAlloc()))) {
			std::terminate();
		}

		interbuf::ClassField field;
		{
			field = { "u32", interbuf::DataType{ interbuf::DataTypeKind::U32 }, interbuf_offsetof(ClassTest, u32) };

			if (!classLayout->addField(std::move(field))) {
				std::terminate();
			}
		}
		{
			field = { "f32", interbuf::DataType{ interbuf::DataTypeKind::F32 }, interbuf_offsetof(ClassTest, f32) };

			if (!classLayout->addField(std::move(field))) {
				std::terminate();
			}
		}

		FILE *fp;

		if (!(fp = fopen("test.bin", "wb"))) {
			puts("Error opening test.bin");
			return -1;
		}

		ClassTest test;

		test.u32 = 0x12;
		test.f32 = 0.34f;

		{
			MyWriter writer(peff::getDefaultAlloc(), fp);

			interbuf::serializeClass(peff::getDefaultAlloc(), &test, &writer, classLayout);
		}

		FILE *fp2;

		if (!(fp2 = fopen("test.bin", "rb"))) {
			puts("Error opening test.bin");
			return -1;
		}

		ClassTest test2;

		{
			MyReader reader(peff::getDefaultAlloc(), fp2);

			interbuf::deserializeClass(peff::getDefaultAlloc(), &test2, sizeof(test2), &reader, classLayout);
		}

		assert(test2.u32 == 0x12);
		assert(test2.f32 == 0.34f);
	}

	return 0;
}
