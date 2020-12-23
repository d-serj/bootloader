/**
 * @file img-header.h
 * @brief Header for shared ROM between application and bootloader
 */

#ifndef IMG_HEADER_H_
#define IMG_HEADER_H_

#include <stdint.h>

extern uint32_t __image_hdr_start;
extern uint32_t __image_hdr_end;

#define IMAGE_MAGIC           (0xA5A5)
#define IMAGE_VERSION_CURRENT 1

typedef struct __attribute__((packed))
{
  uint16_t u16_image_magic;       ///< Magic number to indicate the header start
  uint16_t u16_image_hdr_version; ///< Header version
  uint32_t u32_crc;               ///< Image CRC32
  uint32_t u32_data_size;         ///< Sizeof image
  uint8_t  u8_image_type;         ///< Type of image. Force update or normal
  uint8_t  u8_version_major;      ///< Major version of image
  uint8_t  u8_version_minor;      ///< Minor version of image
  uint8_t  u8_version_patch;      ///< Patch version of image
  uint32_t u32_vector_addr;       ///< Vector address of the application
  uint32_t u32_reserved;          ///< Reserved
  char     cP_git_sha[8];         ///< Git SHA
} image_hdr_t;

#endif // IMG_HEADER_H_
