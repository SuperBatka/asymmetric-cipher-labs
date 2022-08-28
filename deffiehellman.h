#ifndef DEFFIEHELLMAN_H
#define DEFFIEHELLMAN_H

#include "gmp.h"
#include "gmpxx.h"
#include <time.h>

class Deffiehellman
{
public:
    Deffiehellman();
    char* generatePrimeNumbers(char *&q_prime_str,const int digits_count);
    char* generateNumber(const int digits_count);

    char* generateGeneratorNumber(const char *pprime, const char *qprime);
    char* calculate(const char *baseBuffer, const char *expBuffer, const char *modBuffer);
private:
    gmp_randstate_t state;
    bool isPrime(const mpz_t candidate);

};

#endif // DEFFIEHELLMAN_H
