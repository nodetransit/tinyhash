#ifndef TINYHASH_BASE85_H__
#define TINYHASH_BASE85_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define @TINYHASH_BASE85_VARIANT@

size_t base85_encode(char* restrict destination, void const* restrict source, size_t size);

size_t base85_decode(void* restrict destination, char const* restrict source);

#ifdef __cplusplus
} //!extern "C"
#endif

#endif /* TINYHASH_BASE85_H__ */
