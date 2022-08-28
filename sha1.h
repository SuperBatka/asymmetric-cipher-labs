#ifndef SHA1_H
#define SHA1_H
#include <stdint.h>
#include <string.h>
#include "HashI.h"

#define GET_32BIT_MSB_FIRST(cp) \
    (((uint32_t)(unsigned char)(cp)[3]) | \
    ((uint32_t)(unsigned char)(cp)[2] << 8) | \
    ((uint32_t)(unsigned char)(cp)[1] << 16) | \
    ((uint32_t)(unsigned char)(cp)[0] << 24))

#define PUT_32BIT_MSB_FIRST(cp, value) do { \
    (cp)[3] = (value); \
    (cp)[2] = (value) >> 8; \
    (cp)[1] = (value) >> 16; \
    (cp)[0] = (value) >> 24; } while (0)

#define rotl(value,shift) (((value) << (shift)) | ((value) >> (32 - (shift))));


class SHA1 : public HashI
{
public:
    SHA1();
    ~SHA1();
    void hashBlock(const unsigned char *buf);
    void hashFinaleBlock(const unsigned char *buf, unsigned short int len, size_t fileSize);
    unsigned char* getHashResult();
    int GetMode() {return 20;};
    void Clear();
    void Init();
private:
    uint32_t  buf[5];
    void round0(uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e);
    void round16( uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e);
    void round20( uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e);
    void round40( uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e);
    void round60( uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e);
    uint32_t transformW(uint32_t *M, const uint32_t position);


};

#endif // SHA1_H



