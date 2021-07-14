#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <uuid4.h>

#define return_if(c, v) if (c) { return v; }

static int
test_generation()
{
    char* uuid_1 = (char*)calloc(uuid4_get_length() + 1, sizeof(char));
    char* uuid_2 = (char*)calloc(uuid4_get_length() + 1, sizeof(char));

    if(uuid4_generate(uuid_1) == NULL) {
        free(uuid_1);

        return 0;
    }

    if(uuid4_generate(uuid_2) == NULL) {
        free(uuid_1);
        free(uuid_2);

        return 0;
    }

    // printf("%s\n", uuid_1);
    // printf("%s\n", uuid_2);

    if(strcmp(uuid_1, uuid_2) == 0) {
        free(uuid_1);
        free(uuid_2);

        return 0;
    }

    free(uuid_1);
    free(uuid_2);

    return 1;
}

static int
uuid4_test()
{
    return_if (test_generation() == 0, 0);

    return 1;
}

int
main(int argc, char* argv[])
{
    printf("uuid4 tests: %s\n", uuid4_test() ? "SUCCEEDED" : "FAILED");

    return 0;
}
