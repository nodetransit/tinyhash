#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <base85.h>

int
test_illegal_characters()
{
    {
        static char empty[] = {0};

        size_t decode_len = base85_decode(NULL, empty);
        char* decoded = (char*)calloc(decode_len + 1, sizeof(char));
        base85_decode(decoded, empty);

        free(decoded);
    }

    {
        static char illegal[] = {'0', -1, '1', '2', '3', 0};

        size_t decode_len = base85_decode(NULL, illegal);
        char* decoded = (char*)calloc(decode_len + 1, sizeof(char));
        base85_decode(decoded, illegal);

        free(decoded);
    }

    {
        static char illegal[] = {'A', 'K', 'a', 'n', -1, '1', '2', '3', 0};

        size_t decode_len = base85_decode(NULL, illegal);
        char* decoded = (char*)calloc(decode_len + 1, sizeof(char));
        base85_decode(decoded, illegal);

        free(decoded);
    }

    return 1;
}

int
test_decode()
{
    static char const expected[] =
                            "Base85\n"
                            "Ascii85, also called Base85, is a form "
                            "of binary-to-text encoding used to "
                            "communicate arbitrary binary data over "
                            "channels that were designed to carry "
                            "only English language human-readable "
                            "text.\n"
                            "\n"
                            "Why use Base85?\n"
                            "The advantage over Base64 is that "
                            "the resulting text is only 25% larger "
                            "from the original, in comparison with "
                            "the 33% extra space needed for a "
                            "Base64 encoded text.\n"
                            "\n"
                            "The disadvantage is that ASCII85 uses "
                            "more characters in the encoding, "
                            "opposed to Base64 which uses only 64 "
                            "and can be easily transported on "
                            "text channels, used in xml, or in uris.";

    static char const encoded[] =
#if defined(BASE85_VARIANT_Z85)
                            "lsB}Di5I4eB7n]#i5JlfvqH16ay!?@y?mc1lsB}D"
                            "i5Jlfx(+z9azbLjz6i+eayY55vrcSBBz&:MwQ5qt"
                            "wO#0@wnb{]aA.:pwft#jay/tbzfo-gv{%E<ayPq1"
                            "x([x7A^n^px(mv1C{3TaByuqgC4CXpv}Yp*zE)T9"
                            "aARp9BrC^sA+cw7wPIM#zE)u3Bz&pivrcylaA8dl"
                            "C{2K[xkzp6xDRb1zFa92xjSi0B.$M8ePLAhwmn}-"
                            "wGV8aCYULB3rH[^aA.:pavoHTwJhLi3rgXIayO/7"
                            "vqZf}xjSi7C4CXplsB}Dhz=K<B0bNrvrrSlxK@rb"
                            "wPJ0gBzkS9aARgtBrCpsaA8dlC{1>1b{m2jA+w%7"
                            "azbUjz6i}lwGU]ix>Ic$vqEFqx(k56z/6D4A+PPk"
                            "zxKdqBz8!sxK@qxgBjePCYW$maAIHgv}u^2wO2y-"
                            "wftG5AV!^klsB}Dhz=K/zEWP}wN({9wQ5qH3jo:C"
                            "wGUI$B75Q5vqZf}xjSi1B0bNrvrrRTq:TbFi5I?3"
                            "B7GznzeTDaay/7[A:-E1wPz&mx(k5nxK@q$zEWP}"
                            "x(mMmaA8jrz/YG2aARJAlsB}Dhz=L2xLy%&aA.:p"
                            "B0bysy<vcYg:3jAwftw<zxJz1az1#5x(55qBAg/c"
                            "B8V5rBy!#9z/cXtwQ5qtv}Yp*zE)T9efG7RwN(]$"
                            "zxKgvy+cYEAV/7laA.ZsB1N";
#elif defined(BASE85_VARIANT_ASCII85)
                            "6=FqH3&M%/F(8ou3&N60@;L\"'+CehrCh7-\"6=F"
                            "qH3&N60BlbD*+D,P4D'3b/+C]&&@<-WFFDiaQAU&"
                            ";>ASu!rA8,po+E_a:A0>u4+Cf>,D09`1@psIj+CT"
                            ";\"BlnB(Ed8d:Bl7@\"Gp$X+FC?;1G%G\\:@q]:g"
                            "DImX*+EV:*F<Gd=Eb-A(ATMQuDIm?$FDi:3@<-C6"
                            "+E).6Gp#OnB5D:'BHV,\"DJ+*#B4W3!F_tQ)/TPE"
                            "2A78q`AKZ)+G]YPF$<Lnd+E_a:+@9LXAN2P3$<1\\"
                            "M+CSf(@;^0qB4W3(G%G\\:6=FqH2DcOjF!,R<@<<"
                            "W6BOr<,ATN!1FD5W*+EV1>F<G:=+E).6Gp\"k\","
                            "p7#4EbAs(+D,Y4D'3q6AKYo3BkM-t@;IJ;Bl5&'D"
                            "f'H%EbTT5DBO.;FD)e=BOr;B1F4/TG][t7+EML1@"
                            "q?d#AS#C`A0>K&EZed56=FqH2DcOfDI[TqARlp*A"
                            "U&;L$49aGAKYMtF(&U&@;^0qB4W3\"F!,R<@<<VX"
                            ";aX,J3&Mh$F(KD8D/XH++Cf(nEa`I\"ATDi7Bl5&"
                            "8BOr;tDI[TqBl7Q7+E)4<Df]K#+EVNE6=FqH2DcP"
                            "#BPCsi+E_a:F!,C=Cj@-]1a$4EA0>AjDBND\"+D\""
                            "u&Bl&&;FE1f-F)Z&<FCeu*Df-\\>AU&;>@q]:gDI"
                            "mX*/0K(VARlotDBO1@Cb-]IEZf(6+E_^=F\"R";
#elif defined(BASE85_VARIANT_BASE85)
                            "LSb`dI5i4Eb7N^~I5jLFVQh16AY)-{Y-MC1LSb`d"
                            "I5jLFX>%Z9AZBlJZ6I%EAYy55VRCsbbZ;$mWq5QT"
                            "Wo~0{WNB_^Aa!$PWFT~JAY*TBZFO#GV_|e<AYpQ1"
                            "X>@X7a(N(PX>MV1c_3tAbYUQGc4cxPV`yP+Ze?t9"
                            "AarP9bRc(Sa%CW7Wpim~Ze?U3bZ;PIVRCYLAa8DL"
                            "c_2k@XKZP6XdrB1ZfA92XJsI0b!}m8EplaHWMN`#"
                            "Wgv8Acyulb3Rh@(Aa!$PAVOhtWjHlI3RGxiAYo*7"
                            "VQzF`XJsI7c4cxPLSb`dHZ&k<b0BnRVRRsLXk{RB"
                            "Wpj0GbZKs9AarGTbRcPSAa8DLc_1=1B_M2Ja%W|7"
                            "AZBuJZ6I`LWgu^IX=iC}VQefQX>K56Z*6d4a%ppK"
                            "ZXkDQbZ8)SXk{QXGbJEpcyw}MAaihGV`U(2Wo2Y#"
                            "WFTg5av)(KLSb`dHZ&k*Zewp`Wn>_9Wq5Qh3JO$c"
                            "Wgui}b75q5VQzF`XJsI1b0BnRVRRrtQ$tBfI5i-3"
                            "b7gZNZEtdAAY*7@a$#e1WpZ;MX>K5NXk{Q}Zewp`"
                            "X>MmMAa8JRZ*yg2AarjaLSb`dHZ&l2XlY|;Aa!$P"
                            "b0BYSY<VCyG$3JaWFTW<ZXjZ1AZ1~5X>55QbaG*C"
                            "b8v5RbY)~9Z*CxTWq5QTV`yP+Ze?t9EFg7rWn>^}"
                            "ZXkGVY%Cyeav*7LAa!zSb1nb?";
#endif

    size_t decode_len = base85_decode(NULL, encoded);
    char* decoded = (char*)calloc(decode_len + 1, sizeof(char));
    base85_decode(decoded, encoded);

    if (strlen(decoded) <= 0) {
        free(decoded);

        return 0;
    }

    if (strcmp(expected, decoded) != 0) {
        free(decoded);

        return 0;
    }

    free(decoded);

    return 1;
}

int
test_encode()
{
    static char const digits[] = "0123456789"
                                 "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "!#$%&()*+-;<=>?@^_`{|}~";

    size_t len = base85_encode(NULL, digits, sizeof(digits));
    char* encoded = (char*)calloc(len + 1, sizeof(char));
    base85_encode(encoded, digits, sizeof(digits));

    if (strlen(encoded) <= 0) {
        free(encoded);

        return 0;
    }

    size_t decode_len = base85_decode(NULL, encoded);
    char* decoded = (char*)calloc(decode_len + 1, sizeof(char));
    base85_decode(decoded, encoded);

    if (strlen(decoded) <= 0) {
        free(encoded);
        free(decoded);

        return 0;
    }

    if (strcmp(digits, decoded) != 0) {
        free(encoded);
        free(decoded);

        return 0;
    }

    free(encoded);
    free(decoded);

    return 1;
}

int
test_unicode()
{
    static char const digits[] = "茜";

    size_t len = base85_encode(NULL, digits, sizeof(digits));
    char* encoded = (char*)calloc(len + 1, sizeof(char));
    base85_encode(encoded, digits, sizeof(digits));

    size_t decode_len = base85_decode(NULL, encoded);
    char* decoded = (char*)calloc(decode_len + 1, sizeof(char));
    base85_decode(decoded, encoded);

    // printf("%s\n", encoded);
    // printf("%s\n", decoded);

    if(strcmp(decoded, digits) != 0) {
        free(encoded);
        free(decoded);

        return 0;
    }

    free(encoded);
    free(decoded);

    return 1;
}

int
base85_test()
{
    if(test_unicode() == 0) {
        return 0;
    }

    if (test_encode() == 0) {
        return 0;
    }

    if (test_decode() == 0) {
        return 0;
    }

    if (test_illegal_characters() == 0) {
        return 0;
    }

    return 1;
}

int
main()
{
    printf("Base85 tests: %s\n", base85_test() ? "SUCCEEDED" : "FAILED");

    return 0;
}
