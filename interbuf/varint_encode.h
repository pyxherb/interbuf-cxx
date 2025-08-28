#ifndef _INTERBUF_VARINT_ENCODE_H_
#define _INTERBUF_VARINT_ENCODE_H_

#include "except.h"
#include <peff/containers/bitarray.h>

namespace interbuf {
	void encodeVarInt64(uint64_t data, uint8_t buffer[10], size_t &szBufferOut);
	bool decodeVarInt64(const uint8_t *buffer, size_t szBuffer, size_t &szReadBufferOut, uint64_t &dataOut);
}

#endif
