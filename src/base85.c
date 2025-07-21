#include <string.h>

#include "base85.h"

static char const ascii_table[] = {
#if defined(BASE85_VARIANT_Z85)
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      '.', '-', ':', '+', '=', '^', '!', '/', '*', '?', '&', '<', '>', '(', ')', '[', ']', '{', '}', '@', '%', '$', '#',
#elif defined(BASE85_VARIANT_BASE85)
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
      '!', '#', '$', '%', '&', '(', ')', '*', '+', '-', ';', '<', '=', '>', '?', '@', '^', '_', '`', '{', '|', '}', '~',
#elif defined(BASE85_VARIANT_ASCII85)
      '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/',
      '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
      ':', ';', '<', '=', '>', '?', '@',
      'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y',
      'Z', '[', '\\', ']', '^', '_', '`',
      'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u',
#endif
};

static unsigned char const binary_table[] = {
#if defined(BASE85_VARIANT_Z85)
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 68, 85, 84, 83, 82, 72, 85, 75, 76, 70, 65, 85, 63, 62, 69,
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 64, 85, 73, 66, 74, 71,
      81, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 77, 85, 78, 67, 85,
      85, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
      25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 79, 85, 80, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
#elif defined(BASE85_VARIANT_BASE85)
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 62, 85, 63, 64, 65, 66, 85, 67, 68, 69, 70, 85, 71, 85, 85,
      0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 85, 72, 73, 74, 75, 76,
      77, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
      25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 85, 85, 85, 78, 79,
      80, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50,
      51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 81, 82, 83, 84, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
#elif defined(BASE85_VARIANT_ASCII85)
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
      15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
      31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46,
      47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
      63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
      79, 80, 81, 82, 83, 84, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
      85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
#endif
};

static unsigned long const powerful_table[] = {
      1ul,        /*< 85^0 */
      85ul,       /*< 85^1 */
      7225ul,     /*< 85^2 */
      614125ul,   /*< 85^3 */
      52200625ul, /*< 85^4 */
};


static short const endianness = 1;

static char const* const littleEndian = (char const*)&endianness;

static inline int
is_little_endian()
{
    return *littleEndian == 1 ? 1 : 0;
}

/**
 * @brief Converts a integer of 4 bytes in 5 digits of base 85.
 * @param dest Memory block where to put the 5 digits of base 85.
 * @param value Value of the integer of 4 bytes.
 */
static void
word_to_base85(char* dest, unsigned int long value)
{
    for (unsigned int i = 0; i < 5; ++i) {
        const unsigned long pow = powerful_table[4 - i];
        unsigned int const  bin = value / pow;
        dest[i] = ascii_table[bin];
        value -= bin * pow;
    }
}

/**
 * @brief Convert a big-endian array of  bytes in a unsigned long.
 * @param src Pointer to array of bytes.
 * @param sz Size in bytes of array from 0 until 4.
 * @return  The unsigned long value.
 */
static unsigned long
big_endian_to_word(void const* src, int sz)
{
    unsigned long value = 0;
    char* const d = (char*)&value;
    char const* const s = (char const*)src;

    for (int i = 0; i < sz; ++i) {
        d[is_little_endian() ? 3 - i : i] = s[i];
    }

    for (int i = sz; i < 4; ++i) {
        d[is_little_endian() ? 3 - i : i] = 0;
    }

    return value;
}

/**
 * Copy a unsigned long in a big-endian array of 4 bytes.
 * @param dest Destination memory block.
 * @param value Value to copy.
 * @return  dest + 4
 */
static void*
word_to_big_endian(void* dest, unsigned long value)
{
    char* const d = (char*)dest;
    char const* const s = (char*)&value;

    for (int i = 0; i < 4; ++i) {
        d[i] = s[is_little_endian() ? 3 - i : i];
    }

    return d + 4;
}

size_t
get_encode_length(size_t size)
{
    return ((size) / 4) * 5 + (size % 4 > 0 ? 5 : 0);
}

size_t
get_decode_length(size_t size)
{
    return ((size / 5) * 4) + 5;
}

size_t
base85_encode(char* restrict destination, const void* restrict source, size_t size)
{
    size_t len = get_encode_length(size);

    if (destination == NULL) {
        return len;
    }

    size_t const quartets = size / 4;
    char const* s = (char*)source + 4 * quartets;
    destination += 5 * quartets;

    int const remainder = size % 4;

    if (remainder) {
        word_to_base85(destination, big_endian_to_word(s, remainder));
    }

    char* rslt = destination + (remainder ? 5 : 0); // pointer to the null character of the base85 null-terminated string
    *rslt = '\0';

    for (size_t i = 0; i < quartets; ++i) {
        word_to_base85(destination -= 5, big_endian_to_word(s -= 4, 4));
    }

    len = strlen(destination);

    if (remainder > 0) {
        destination[len - 5 + remainder] = 0;
    }

    return len;
}

size_t
base85_decode(void* restrict destination, const char* restrict source)
{
    int size = strlen(source);

    if(size <= 0) {
        return 0;
    }

    size_t len = get_decode_length(size);

    if (destination == NULL) {
        return len;
    }

    char* start = destination;
    int missing = size % 5;

    for (unsigned char const* s = (unsigned char const*)source;;) {
        unsigned int  binary = 85u;
        unsigned long value  = 0;

        for (long unsigned int i = 0; i < 5; ++i, ++s) {
            binary = binary_table[*s];

            if (binary == 85u && missing == 0) {
                break;
            }

            const unsigned long pow = powerful_table[4 - i];

            value += binary * pow;

            if (binary == 85u) {
                break;
            }
        }

        destination = word_to_big_endian(destination, value);

        if (binary == 85u) {
            break;
        }
    }

    len = strlen(start);

    if (missing > 0) {
        start[len - 5 + missing] = 0;
    }

    return len;
}
