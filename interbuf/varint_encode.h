#ifndef _INTERBUF_VARINT_ENCODE_H_
#define _INTERBUF_VARINT_ENCODE_H_

#include "except.h"

namespace interbuf {
	void encode_var_int64(uint64_t data, uint8_t buffer[10], size_t &sz_buffer_out);
	typedef bool (*VarInt64DecodeReader)(void *user_data, uint8_t &data_out);
	bool decode_var_int64(VarInt64DecodeReader decoder_reader, void *user_data, uint64_t &data_out, size_t &sz_read_buffer_out);
}

#endif
