#ifndef TINYHASH_UUID4_H__
#define TINYHASH_UUID4_H__

#ifdef __cplusplus
extern "C" {
#endif

unsigned int uuid4_get_length();
void* uuid4_generate(char* destination);

#ifdef __cplusplus
} //!extern "C"
#endif

#endif /* TINYHASH_UUID4_H__ */

