#ifndef ELGAMAL_H
#define ELGAMAL_H

#include <random>
#include <gmp.h>
#include <gmpxx.h>
#include <time.h>


class Elgamal
{
public:
    Elgamal();
    ~Elgamal();

    void generatePrimeNumbers(int digits_count);
    void generateGeneratorNumber();

    void setBufferSize(size_t bufferSize);
    size_t getModuleByteCount();
    char* encrypt(const char *buffer, const size_t len, size_t *result_size,bool last_block);
    char* decrypt(const char *buffer, const size_t len, size_t *result_size);
    bool  checkSign(const char *buffer, const size_t len, const char *hash, const size_t hashLen, size_t *result_size);
    char* sign(const char *buffer, const size_t len, size_t *result_size);
    void  readSignatureFirstBlock(const char *buffer,const size_t len);
    char* writeSignatureFirstBlock(size_t *result_size);
    bool setOpenKeyParametrs(const char *y, const char *g, const char *p);
    bool setPrivateKeyParametrs(const char *x, const char *k, const char *g, const char *p);


    char* getStrModule(size_t *size);
    char* getStrGenerator(size_t *size);
    char* getStrAliceOpenKey(size_t *size);
    char* getStrAliceClosedKey(size_t *size);
    char* getStrBobOpenKey(size_t *size);
    char* getStrBobClosedKey(size_t *size);
    char* getStrFirstBlockdKey(size_t *size);
    char* getStrParam(size_t *size, mpz_t param);
    void generateAliceKeys();
    void generateBobKeys();

    void clear();
    char* writeFirstBlock(size_t *result_size);
    void readFirstBlock(const char *buffer,const size_t len);



private:

    bool isPrime(const mpz_t candidate);


    void generateClosedKey(mpz_t closedKey);
    void generateOpenKey(mpz_t openKey, const mpz_t closedKey);
    void decryptBlock(mpz_t result,const mpz_t openKey, const mpz_t closedKey,const mpz_t message);
    void encryptBlock(mpz_t result,const mpz_t openKey, const mpz_t closedKey,const mpz_t message);
    void init();

    char *result;


    mpz_t generated_num;
    mpz_t p_prime;
    mpz_t q_prime;
    mpz_t generator;
    gmp_randstate_t state;
    size_t moduleByteCount;

    mpz_t aliceOpenKey;
    mpz_t aliceClosedKey;
    mpz_t bobOpenKey;
    mpz_t bobClosedKey;
    mpz_t firstBlockKey;
    mpz_t firstSignatureBlockKey;


};

#endif // ELGAMAL_H
