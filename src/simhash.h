#ifndef TINYHASH_SIMHASH_H__
#define TINYHASH_SIMHASH_H__

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

unsigned long int simhash_hash(const char* string, unsigned int length, unsigned int block_size);
double simhash_similarity(const unsigned long int hash1, const unsigned long int hash2);

#ifdef __cplusplus
} //!extern "C"
#endif

#endif /* TINYHASH_SIMHASH_H__ */
