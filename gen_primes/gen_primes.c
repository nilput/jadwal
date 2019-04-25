//find prime near power of two using openssl
//author: github.com/nilput
#include <openssl/bn.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
void die(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(1);
}
bool fopenprint(const char *filename, const char *content) {
    if (strcmp(filename, "stdout") == 0) {
        fprintf(stdout, "%s", content);
        return true;
    }

    FILE *f = fopen(filename, "w");
    if (!f)
        return false;
    fprintf(f, "%s", content);
    fclose(f);
    return true;
}

bool ssl_print_prime(const char *fname, int nbits) {
    assert(nbits >= 2);
    if (nbits <= 8) {
        const char *content = "";
        switch (nbits) {
            case 2: content="2"; break;
            case 3: content="7"; break;
            case 4: content="13"; break;
            case 5: content="23"; break;
            case 6: content="61"; break;
            case 7: content="103"; break;
            case 8: content="251"; break;
            case 9: content="419"; break;
        }
        if (!fopenprint(fname, content)) {
            die("cant print to file");
        }
        return true;
    }


    BIGNUM *bign = BN_new();
    if (!bign)
        die("BN_new failed");
    int rv = BN_generate_prime_ex(bign,
                                  nbits,
                                  1, /*int safe*/
                                  NULL, /*const BIGNUM *add*/
                                  NULL, /*const BIGNUM *rem*/
                                  NULL /*BN_GENCB *cb*/);
    if (rv == 0) {
        die("BN_generate_prime_ex failed");
    }
    char *as_str = BN_bn2dec(bign);
    if (!as_str) 
        die("cant generate bn2dec");

    if (!fopenprint(fname, as_str)) {
        die("cant print to out.txt");
    }

    BN_clear_free(bign);
    OPENSSL_free(as_str);
    return true;
}

int main(int argc, const char *argv[]) {
    if (argc < 2 || (atoi(argv[1]) < 2)) 
        die("usage: prog <nbits> [fname]");
    int nbits = atoi(argv[1]);
    const char *fname = "stdout";
    if (argc >= 3) {
        fname = argv[2];
    }
    if (!ssl_print_prime(fname, nbits))
        die("failed to generate prime");
}
