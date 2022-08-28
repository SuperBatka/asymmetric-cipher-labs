#ifndef MD5_H
#define MD5_H
#include <stdint.h>
#include <string.h>
#include <HashI.h>

#define F1(x,y,z) (((x)&(y)) | (~(x) & (z)))
#define F2(x,y,z) (((x) & (z)) | (~(z) & (y)))
#define F3(x,y,z) ((x) ^ (y) ^ (z))
#define F4(x,y,z) ((y) ^( (x) | ~(z)))

#define ROUND(f,w,x,y,z,X,s) \
     (w += f(x,y,z) + X, w = (w<<s | w>>(32-s)) + x)

#define GET_32BIT_LSB_FIRST(cp) \
    (((uint32_t)(unsigned char)(cp)[0]) | \
    ((uint32_t)(unsigned char)(cp)[1] << 8) | \
    ((uint32_t)(unsigned char)(cp)[2] << 16) | \
    ((uint32_t)(unsigned char)(cp)[3] << 24))


#define PUT_32BIT_LSB_FIRST(cp, value) do { \
    (cp)[0] = (value); \
    (cp)[1] = (value) >> 8; \
    (cp)[2] = (value) >> 16; \
    (cp)[3] = (value) >> 24; } while (0)



class MD5 : public HashI
{
public:
    MD5();
    ~MD5() {};
    void hashBlock(const unsigned char *buf);
    void hashFinaleBlock(const unsigned char *buf, unsigned short int len, size_t fileSize);
    unsigned char* getHashResult();
    int GetMode() { return 16;};
    void Clear() {};
    void Init();
private:
    uint32_t  buf[4];

};

#endif // MD5_H
