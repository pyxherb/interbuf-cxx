#include "varint_encode.h"

using namespace interbuf;

void interbuf::encode_var_int64(uint64_t data, uint8_t buffer[10], size_t &sz_buffer_out) {
	if (data < (1ULL << 7)) {
		buffer[0] = 0b10000000 | (data & 0b01111111);
		sz_buffer_out = 1;

	} else if (data < (1ULL << 14)) {
		buffer[0] = 0b00000000 | (data & 0b01111111);
		buffer[1] = 0b10000000 | ((data >> 7) & 0b01111111);
		sz_buffer_out = 2;

	} else if (data < (1ULL << 21)) {
		buffer[0] = 0b00000000 | (data & 0b01111111);
		buffer[1] = 0b00000000 | ((data >> 7) & 0b01111111);
		buffer[2] = 0b10000000 | ((data >> 14) & 0b01111111);
		sz_buffer_out = 3;

	} else if (data < (1ULL << 28)) {
		buffer[0] = 0b00000000 | (data & 0b01111111);
		buffer[1] = 0b00000000 | ((data >> 7) & 0b01111111);
		buffer[2] = 0b00000000 | ((data >> 14) & 0b01111111);
		buffer[3] = 0b10000000 | ((data >> 21) & 0b01111111);
		sz_buffer_out = 4;

	} else if (data < (1ULL << 35)) {
		buffer[0] = 0b00000000 | (data & 0b01111111);
		buffer[1] = 0b00000000 | ((data >> 7) & 0b01111111);
		buffer[2] = 0b00000000 | ((data >> 14) & 0b01111111);
		buffer[3] = 0b00000000 | ((data >> 21) & 0b01111111);
		buffer[4] = 0b10000000 | ((data >> 28) & 0b01111111);
		sz_buffer_out = 5;

	} else if (data < (1ULL << 42)) {
		buffer[0] = 0b00000000 | (data & 0b01111111);
		buffer[1] = 0b00000000 | ((data >> 7) & 0b01111111);
		buffer[2] = 0b00000000 | ((data >> 14) & 0b01111111);
		buffer[3] = 0b00000000 | ((data >> 21) & 0b01111111);
		buffer[4] = 0b00000000 | ((data >> 28) & 0b01111111);
		buffer[5] = 0b10000000 | ((data >> 35) & 0b01111111);
		sz_buffer_out = 6;

	} else if (data < (1ULL << 49)) {
		buffer[0] = 0b00000000 | (data & 0b01111111);
		buffer[1] = 0b00000000 | ((data >> 7) & 0b01111111);
		buffer[2] = 0b00000000 | ((data >> 14) & 0b01111111);
		buffer[3] = 0b00000000 | ((data >> 21) & 0b01111111);
		buffer[4] = 0b00000000 | ((data >> 28) & 0b01111111);
		buffer[5] = 0b00000000 | ((data >> 35) & 0b01111111);
		buffer[6] = 0b10000000 | ((data >> 42) & 0b01111111);
		sz_buffer_out = 7;

	} else if (data < (1ULL << 56)) {
		buffer[0] = 0b00000000 | (data & 0b01111111);
		buffer[1] = 0b00000000 | ((data >> 7) & 0b01111111);
		buffer[2] = 0b00000000 | ((data >> 14) & 0b01111111);
		buffer[3] = 0b00000000 | ((data >> 21) & 0b01111111);
		buffer[4] = 0b00000000 | ((data >> 28) & 0b01111111);
		buffer[5] = 0b00000000 | ((data >> 35) & 0b01111111);
		buffer[6] = 0b00000000 | ((data >> 42) & 0b01111111);
		buffer[7] = 0b10000000 | ((data >> 49) & 0b01111111);
		sz_buffer_out = 8;

	} else if (data < (1ULL << 63)) {
		buffer[0] = 0b00000000 | (data & 0b01111111);
		buffer[1] = 0b00000000 | ((data >> 7) & 0b01111111);
		buffer[2] = 0b00000000 | ((data >> 14) & 0b01111111);
		buffer[3] = 0b00000000 | ((data >> 21) & 0b01111111);
		buffer[4] = 0b00000000 | ((data >> 28) & 0b01111111);
		buffer[5] = 0b00000000 | ((data >> 35) & 0b01111111);
		buffer[6] = 0b00000000 | ((data >> 42) & 0b01111111);
		buffer[7] = 0b00000000 | ((data >> 49) & 0b01111111);
		buffer[8] = 0b10000000 | ((data >> 56) & 0b01111111);
		sz_buffer_out = 9;

	} else {
		buffer[0] = 0b00000000 | (data & 0b01111111);
		buffer[1] = 0b00000000 | ((data >> 7) & 0b01111111);
		buffer[2] = 0b00000000 | ((data >> 14) & 0b01111111);
		buffer[3] = 0b00000000 | ((data >> 21) & 0b01111111);
		buffer[4] = 0b00000000 | ((data >> 28) & 0b01111111);
		buffer[5] = 0b00000000 | ((data >> 35) & 0b01111111);
		buffer[6] = 0b00000000 | ((data >> 42) & 0b01111111);
		buffer[7] = 0b00000000 | ((data >> 49) & 0b01111111);
		buffer[8] = 0b00000000 | ((data >> 56) & 0b01111111);
		buffer[9] = 0b10000000 | ((data >> 63));
		sz_buffer_out = 10;
	}
}

bool interbuf::decode_var_int64(VarInt64DecodeReader decoder_reader, void *user_data, uint64_t &data_out, size_t &sz_read_buffer_out) {
	uint64_t result = 0;
	size_t cur_off = 0;

	uint8_t b;
	for (uint8_t i = 0; i < 10; ++i) {
		if (!decoder_reader(user_data, b))
			return false;

		++cur_off;

		result <<= 7;
		result |= b & 0b01111111;

		if (b & 0b10000000)
			break;
	}

	sz_read_buffer_out = cur_off;
	data_out = result;

	return true;
}
