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
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_i8(int8_t data) override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_i16(int16_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_i32(int32_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_i64(int64_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_u8(uint8_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_u16(uint16_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_u32(uint32_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_u64(uint64_t data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_f32(float data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_f64(double data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer write_bool(bool data) noexcept override {
		if (!(fwrite(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
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
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_i8(int8_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_i16(int16_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_i32(int32_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_i64(int64_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_u8(uint8_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_u16(uint16_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_u32(uint32_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_u64(uint64_t &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_f32(float &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_f64(double &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
		return {};
	}
	virtual interbuf::ExceptionPointer read_bool(bool &data) noexcept override {
		if (!(fread(&data, sizeof(data), 1, _fp)))
			return interbuf::with_oom_error_if_alloc_failed(interbuf::IOError::alloc(allocator.get()));
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

	interbuf::Document document(peff::default_allocator());
	{
		interbuf::ObjectPtr<interbuf::StructLayoutObject> struct_layout;

		if (!(struct_layout = interbuf::make_object<interbuf::StructLayoutObject>(peff::default_allocator(), &document, peff::default_allocator()))) {
			std::terminate();
		}

		interbuf::StructField field;
		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::I8 }, interbuf_offsetof(Test, i8) };

			if (!struct_layout->add_field(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::I16 }, interbuf_offsetof(Test, i16) };

			if (!struct_layout->add_field(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::I32 }, interbuf_offsetof(Test, i32) };

			if (!struct_layout->add_field(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::I64 }, interbuf_offsetof(Test, i64) };

			if (!struct_layout->add_field(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::U8 }, interbuf_offsetof(Test, u8) };

			if (!struct_layout->add_field(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::U16 }, interbuf_offsetof(Test, u16) };

			if (!struct_layout->add_field(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::U32 }, interbuf_offsetof(Test, u32) };

			if (!struct_layout->add_field(std::move(field))) {
				std::terminate();
			}
		}

		{
			field = { interbuf::DataType{ interbuf::DataTypeKind::U64 }, interbuf_offsetof(Test, u64) };

			if (!struct_layout->add_field(std::move(field))) {
				std::terminate();
			}
		}

		{
			interbuf::ObjectPtr<interbuf::ArrayDataTypeDefObject> array_data_type;

			if (!(array_data_type = interbuf::make_object<interbuf::ArrayDataTypeDefObject>(peff::default_allocator(), &document, peff::default_allocator()))) {
				std::terminate();
			}

			array_data_type->element_type = interbuf::DataType{ interbuf::DataTypeKind::U8 };

			array_data_type->serializer = [](const void *ptr,
											const char *&ptr_out,
											size_t &sz_element_out,
											size_t &length_out) {
				ptr_out = (const char *)ptr;
				sz_element_out = sizeof(uint8_t);
				length_out = std::size(std::declval<Test>().s);
			};

			array_data_type->deserializer = [](size_t n_elements,
											  void *ptr,
											  char *&ptr_out,
											  size_t &sz_element_out) -> interbuf::ExceptionPointer {
				sz_element_out = sizeof(uint8_t);
				ptr_out = (char *)ptr;
				return {};
			};

			field = { interbuf::DataType{ interbuf::DataTypeKind::Array, array_data_type.cast_to<interbuf::Object>() }, interbuf_offsetof(Test, s) };

			if (!struct_layout->add_field(std::move(field))) {
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
			MyWriter writer(peff::default_allocator(), fp);

			interbuf::serialize_struct(peff::default_allocator(), &test, &writer, struct_layout);
		}

		FILE *fp2;

		if (!(fp2 = fopen("test.bin", "rb"))) {
			puts("Error opening test.bin");
			return -1;
		}

		Test test2;

		peff::HashSet<peff::String> string_pool(peff::default_allocator());
		{
			MyReader reader(peff::default_allocator(), fp2);

			interbuf::deserialize_struct(string_pool, peff::default_allocator(), &test2, sizeof(test2), &reader, struct_layout);
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
		interbuf::ObjectPtr<interbuf::ClassLayoutObject> class_layout;

		if (!(class_layout = interbuf::make_object<interbuf::ClassLayoutObject>(peff::default_allocator(), &document, peff::default_allocator()))) {
			std::terminate();
		}

		interbuf::ClassField field;
		{
			field = { "u32", interbuf::DataType{ interbuf::DataTypeKind::U32 }, interbuf_offsetof(ClassTest, u32) };

			if (!class_layout->add_field(std::move(field))) {
				std::terminate();
			}
		}
		{
			field = { "f32", interbuf::DataType{ interbuf::DataTypeKind::F32 }, interbuf_offsetof(ClassTest, f32) };

			if (!class_layout->add_field(std::move(field))) {
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
			MyWriter writer(peff::default_allocator(), fp);

			interbuf::serialize_class(peff::default_allocator(), &test, &writer, class_layout);
		}

		FILE *fp2;

		if (!(fp2 = fopen("test.bin", "rb"))) {
			puts("Error opening test.bin");
			return -1;
		}

		ClassTest test2;

		{
			MyReader reader(peff::default_allocator(), fp2);

			interbuf::deserialize_class(peff::default_allocator(), &test2, sizeof(test2), &reader, class_layout);
		}

		assert(test2.u32 == 0x12);
		assert(test2.f32 == 0.34f);
	}

	return 0;
}
