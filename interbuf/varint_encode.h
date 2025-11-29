#ifndef _INTERBUF_VARINT_ENCODE_H_
#define _INTERBUF_VARINT_ENCODE_H_

#include "except.h"
#include <peff/containers/bitarray.h>

namespace interbuf {
	void encodeVarInt64(uint64_t data, uint8_t buffer[10], size_t &szBufferOut);
	typedef bool (*VarInt64DecodeReader)(void *userData, uint8_t &dataOut);
	bool decodeVarInt64(VarInt64DecodeReader decoderReader, void *userData, uint64_t &dataOut, size_t &szReadBufferOut);
}

#endif
