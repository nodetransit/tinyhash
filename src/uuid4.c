#include <stdio.h>

#if defined(_WIN32)
#include <windows.h>
#include <wincrypt.h>
#endif

#include "uuid4.h"


/**
 * @see http://xorshift.di.unimi.it/xorshift128plus.c
 */
static inline unsigned int
xorshift128plus(unsigned long int* seed)
{
    unsigned int       s1 = seed[0];
    const unsigned int s0 = seed[1];

    seed[0] = s0;
    s1 ^= s1 << 23;
    seed[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);

    return seed[1] + s0;
}

unsigned int
uuid4_get_length()
{
    return 36u;
}

static int
uuid4_init(unsigned long int* seed)
{
#if defined(__linux__) || defined(__APPLE__) || defined(__FreeBSD__)
    FILE* fp = fopen("/dev/urandom", "rb");

    if (!fp) {
        return 0;
    }

    unsigned int res = fread(seed, 1, sizeof(seed), fp);

    fclose(fp);

    if (res != sizeof(seed)) {
        return 0;
    }

#elif defined(_WIN32)
    HCRYPTPROV hCryptProv;

    int res = CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT);

    if (!res) {
        return 0;
    }

    res = CryptGenRandom(hCryptProv, (DWORD)sizeof(seed), (PBYTE)seed);

    CryptReleaseContext(hCryptProv, 0);

    if (!res) {
        return 0;
    }

#else
#error "unsupported platform"
#endif

    return 1;
}

void*
uuid4_generate(char* destination)
{
    unsigned long int seed[2];

    if (uuid4_init(seed) == 0) {
        return NULL;
    }

    static const char* template = "xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx";
    static const char* chars    = "0123456789abcdef";

    union
    {
        unsigned char     b[16];
        unsigned long int word[2];
    }                s;

    const char* p_dest;
    int i = 0;

    /* get random */
    s.word[0] = xorshift128plus(seed);
    s.word[1] = xorshift128plus(seed);

    /* build string */
    p_dest = template;

    while (*p_dest) {
        int n = s.b[i >> 1];

        n = (i & 1) ? (n >> 4) : (n & 0xf);

        switch (*p_dest) {
        case 'x'  :
            *destination = chars[n];
            i++;
            break;
        case 'y'  :
            *destination = chars[(n & 0x3) + 8];
            i++;
            break;
        default   :
            *destination = *p_dest;
        }

        destination++, p_dest++;
    }

    *destination = '\0';

    return (void*)destination;
}
