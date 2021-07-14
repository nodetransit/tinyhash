#include <stdio.h>
#include <stdlib.h>

#include <simhash.h>

#define return_if(c, v) if (c) { return v; }

static int
test_dissimilarity()
{
    static const char lorem[] = "Lorem ipsum dolor sit amet, conse"
                                "ctetur adipiscing elit, sed do ei"
                                "usmod tempor incididunt ut labore"
                                " et dolore magna aliqua. Ut enim "
                                "ad minim veniam, quis nostrud exe"
                                "rcitation ullamco laboris nisi ut"
                                " aliquip ex ea commodo consequat."
                                " Duis aute irure dolor in reprehe"
                                "nderit in voluptate velit esse ci"
                                "llum dolore eu fugiat nulla paria"
                                "tur. Excepteur sint occaecat cupi"
                                "datat non proident, sunt in culpa"
                                " qui officia deserunt mollit anim"
                                " id est laborum.";
    static const char ipsum[] = "But I must explain to you how all"
                                " this mistaken idea of denouncing"
                                " pleasure and praising pain was b"
                                "orn and I will give you a complet"
                                "e account of the system, and expo"
                                "und the actual teachings of the g"
                                "reat explorer of the truth, the m"
                                "aster-builder of human happiness."
                                " No one rejects, dislikes, or avo"
                                "ids pleasure itself, because it i"
                                "s pleasure, but because those who"
                                " do not know how to pursue pleasu"
                                "re rationally encounter consequen"
                                "ces that are extremely painful. N"
                                "or again is there anyone who love"
                                "s or pursues or desires to obtain"
                                " pain of itself, because it is pa"
                                "in, but because occasionally circ"
                                "umstances occur in which toil and"
                                " pain can procure him some great "
                                "pleasure.";

    unsigned long int hash1 = simhash_hash(lorem, strlen(lorem), 24);
    unsigned long int hash2 = simhash_hash(ipsum, strlen(ipsum), 24);

    double similarity = simhash_similarity(hash1, hash2);

    // printf("%f%% similar\n", similarity);
    return similarity < 20;
}

static int
test_similarity_utf8()
{
    static const char address1[] = "〒 532-0023"
                                   "大阪府大阪市淀川区十三東1丁目11番22号"
                                   "タウンコト晴光502号室";
    static const char address2[] = "〒5320023"
                                   "大阪府大阪市淀川区十三東1丁目11番22号"
                                   "タウンコート晴光503号室";

    unsigned long int hash1 = simhash_hash(address1, strlen(address1), 18);
    unsigned long int hash2 = simhash_hash(address2, strlen(address2), 18);

    double similarity = simhash_similarity(hash1, hash2);

    // printf("%f%% similar\n", similarity);
    return similarity > 50;
}

static int
test_similarity()
{
    static const char address1[] = "aa aabbbb bbbbbbbbbbbbbbbbb\n cccccccccccc.";
    static const char address2[] = "a aaabbbb xxxxxuubbbbbbbbbb\n dddddccccccc.";

    unsigned long int hash1 = simhash_hash(address1, strlen(address1), 18);
    unsigned long int hash2 = simhash_hash(address2, strlen(address2), 18);

    double similarity = simhash_similarity(hash1, hash2);

    // printf("%f%% similar\n", similarity);
    return similarity > 50;
}

static int
simhash_test()
{
    return_if (test_similarity() == 0, 0);
    return_if (test_similarity_utf8() == 0, 0);
    return_if (test_dissimilarity() == 0, 0);

    return 1;
}

// typedef struct
// {
//     char* content;
//     unsigned int size;
// } test_file;
//
// static test_file
// read_content(const char* filename)
// {
//     char* fcontent = NULL;
//     unsigned int fsize = 0;
//     test_file ff;
//
//     FILE* fp = fopen(filename, "r");
//
//     if (fp) {
//         fseek(fp, 0, SEEK_END);
//         fsize = ftell(fp);
//         rewind(fp);
//
//         fcontent = (char*)malloc(sizeof(char) * fsize);
//         fread(fcontent, 1, fsize, fp);
//
//         fclose(fp);
//     }
//
//     ff.content = fcontent;
//     ff.size    = fsize;
//
//     return ff;
// }

int
main(int argc, char* argv[])
{
    printf("SimHash tests: %s\n", simhash_test() ? "SUCCEEDED" : "FAILED");

    // if(argc == 4) {
    //     test_file f1 = read_content(argv[1]);
    //     test_file f2 = read_content(argv[2]);
    //     unsigned int block_size = atoi(argv[3]);
    //
    //     unsigned long int hash1 = simhash_hash(f1.content, f1.size, block_size);
    //     unsigned long int hash2 = simhash_hash(f2.content, f2.size, block_size);
    //
    //     free (f1.content);
    //     free (f2.content);
    //
    //     double similarity = simhash_similarity(hash1, hash2);
    //
    //     printf("%f%% similar\n", similarity);
    // }

    return 0;
}
