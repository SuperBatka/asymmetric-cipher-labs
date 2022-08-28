#ifndef SHAMIR_H
#define SHAMIR_H

#include "gmp.h"
#include "gmpxx.h"
#include <time.h>

class shamir
{
public:
    shamir();
    ~shamir();
    char* generatePrimeNumber(const char *message, const int digits_count);
    void generatePair(char *&first, char *&second, const char *mod);
    char* calculate(const char *baseBuffer, const char *expBuffer, const char *modBuffer);

private:
    gmp_randstate_t state;
    bool isPrime(const mpz_t candidate);
    mpz_t prime;
    mpz_t first;
    mpz_t second;
    size_t modSize;
};

#endif // SHAMIR_H
