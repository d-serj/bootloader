/**
 * @file crc32.h
 * @brief Simple public domain implementation of the standard CRC32 checksum.
 *
 * From http://home.thep.lu.se/~bjorn/crc/
 */

#ifndef CRC32_H_
#define CRC32_H_

#include <stdint.h>

uint32_t crc32(const void *data, size_t n_bytes, uint32_t crc_in);

#endif /* CRC32_H_ */
