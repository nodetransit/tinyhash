#include <stdio.h>
#include <stdlib.h>

#include "simhash.h"


static const int simhash_bit = 64;

static unsigned long int
hash_block(const char* array_key, unsigned int length)
{
    register unsigned long int hash = 5381;

    for (; length >= 8; length -= 8) {
        hash = ((hash << 5) + hash) + *array_key++;
        hash = ((hash << 5) + hash) + *array_key++;
        hash = ((hash << 5) + hash) + *array_key++;
        hash = ((hash << 5) + hash) + *array_key++;
        hash = ((hash << 5) + hash) + *array_key++;
        hash = ((hash << 5) + hash) + *array_key++;
        hash = ((hash << 5) + hash) + *array_key++;
        hash = ((hash << 5) + hash) + *array_key++;
    }

    switch (length) {
    case 7:
        hash = ((hash << 5) + hash) + *array_key++;
        // fall through
    case 6:
        hash = ((hash << 5) + hash) + *array_key++;
        // fall through
    case 5:
        hash = ((hash << 5) + hash) + *array_key++;
        // fall through
    case 4:
        hash = ((hash << 5) + hash) + *array_key++;
        // fall through
    case 3:
        hash = ((hash << 5) + hash) + *array_key++;
        // fall through
    case 2:
        hash = ((hash << 5) + hash) + *array_key++;
        // fall through
    case 1:
        hash = ((hash << 5) + hash) + *array_key++;
        break;
    case 0:
        break;
    }

    return hash;
}

static unsigned long int
hash_bucket(const char** tokens, unsigned int length)
{
    float hash_vector[simhash_bit];

    memset(hash_vector, 0, simhash_bit * sizeof(float));

    unsigned long int token_hash  = 0;
    unsigned long int simhash     = 0;
    unsigned long     current_bit = 0;

    for (unsigned int i = 0; i < length; i++) {
        token_hash = hash_block(tokens[i], strlen(tokens[i]));

        for (int j = simhash_bit - 1; j >= 0; j--) {
            current_bit = token_hash & 0x1;

            if (current_bit == 1) {
                hash_vector[j] += 1;
            } else {
                hash_vector[j] -= 1;
            }

            token_hash = token_hash >> 1;
        }
    }

    for (int i = 0; i < simhash_bit; i++) {
        if (hash_vector[i] > 0) {
            simhash = (simhash << 1) + 0x1;
        } else {
            simhash = simhash << 1;
        }
    }

    return simhash;
}

unsigned long int
simhash_hash(const char* string, unsigned int length, unsigned int block_size)
{
    const unsigned int count     = length / block_size + 1;
    const unsigned int lb_length = length - (count - 1) * block_size;

    char** bucket = (char**)calloc(count, sizeof(char*));

    for (unsigned int i = 0, j = 0; i < count; i++, j = j + block_size) {
        char* block = (char*)calloc(block_size + 1, sizeof(char));

        strncpy(block, string + j, i == count - 1 ? lb_length : block_size);

        bucket[i] = block;
    }

    unsigned long int hash = hash_bucket((const char**)bucket, count);

    for (unsigned int i = 0; i < count; i++) {
        char* block = bucket[i];

        free(block);
    }

    free(bucket);

    return hash;
}

double
simhash_similarity(const unsigned long int hash1, const unsigned long int hash2)
{
    double similiarity = 0;

    if (hash1 < hash2) {
        similiarity = hash1 * 1.0f / hash2;
    } else {
        similiarity = hash2 * 1.0f / hash1;
    }

    return similiarity * 100;
}
