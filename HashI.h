#ifndef HASHI_H
#define HASHI_H
#include <stdint.h>
class HashI
{
public:
    virtual void hashBlock(const unsigned char *buf) = 0;
    virtual void hashFinaleBlock(const unsigned char *buf, unsigned short int len, size_t fileSize) = 0;
    virtual unsigned char* getHashResult() = 0;
    virtual int GetMode() = 0;
    virtual void Clear() = 0;
    virtual ~HashI() = default;
protected:
    HashI() {};
};

#endif // HASHI_H
