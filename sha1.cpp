#include "sha1.h"

SHA1::SHA1()
{
    Init();
}
void SHA1::Init()
{
    buf[0] = 0x67452301;
    buf[1] = 0xefcdab89;
    buf[2] = 0x98badcfe;
    buf[3] = 0x10325476;
    buf[4] = 0xc3d2e1f0;


}
void SHA1::Clear()
{
    Init();
}
SHA1::~SHA1()
{

}

void SHA1::hashBlock(const unsigned char *byteBuf)
{
        uint32_t in[16];
        uint32_t a, b, c, d,e;

        for(int i = 0; i < 16; i++)
        {
            in[i] = GET_32BIT_MSB_FIRST(byteBuf+i*4);
        }


        a = buf[0];
        b = buf[1];
        c = buf[2];
        d = buf[3];
        e = buf[4];

        // Round1 0<= i <= 15
        round0(in,  0, a,b,c,d,e);
        round0(in,  1, e,a,b,c,d);
        round0(in,  2, d,e,a,b,c);
        round0(in,  3, c,d,e,a,b);
        round0(in,  4, b,c,d,e,a);
        round0(in,  5, a,b,c,d,e);
        round0(in,  6, e,a,b,c,d);
        round0(in,  7, d,e,a,b,c);
        round0(in,  8, c,d,e,a,b);
        round0(in,  9, b,c,d,e,a);
        round0(in, 10, a,b,c,d,e);
        round0(in, 11, e,a,b,c,d);
        round0(in, 12, d,e,a,b,c);
        round0(in, 13, c,d,e,a,b);
        round0(in, 14, b,c,d,e,a);
        round0(in, 15, a,b,c,d,e);

        // Round1 16<= i <= 19
        round16(in, 16, e,a,b,c,d);
        round16(in, 17, d,e,a,b,c);
        round16(in, 18, c,d,e,a,b);
        round16(in, 19, b,c,d,e,a);

        // Round2 20<= i <= 39
        round20(in, 20, a,b,c,d,e);
        round20(in, 21, e,a,b,c,d);
        round20(in, 22, d,e,a,b,c);
        round20(in, 23, c,d,e,a,b);
        round20(in, 24, b,c,d,e,a);
        round20(in, 25, a,b,c,d,e);
        round20(in, 26, e,a,b,c,d);
        round20(in, 27, d,e,a,b,c);
        round20(in, 28, c,d,e,a,b);
        round20(in, 29, b,c,d,e,a);
        round20(in, 30, a,b,c,d,e);
        round20(in, 31, e,a,b,c,d);
        round20(in, 32, d,e,a,b,c);
        round20(in, 33, c,d,e,a,b);
        round20(in, 34, b,c,d,e,a);
        round20(in, 35, a,b,c,d,e);
        round20(in, 36, e,a,b,c,d);
        round20(in, 37, d,e,a,b,c);
        round20(in, 38, c,d,e,a,b);
        round20(in, 39, b,c,d,e,a);

        // Round3 40<= i <= 59
        round40(in, 40, a,b,c,d,e);
        round40(in, 41, e,a,b,c,d);
        round40(in, 42, d,e,a,b,c);
        round40(in, 43, c,d,e,a,b);
        round40(in, 44, b,c,d,e,a);
        round40(in, 45, a,b,c,d,e);
        round40(in, 46, e,a,b,c,d);
        round40(in, 47, d,e,a,b,c);
        round40(in, 48, c,d,e,a,b);
        round40(in, 49, b,c,d,e,a);
        round40(in, 50, a,b,c,d,e);
        round40(in, 51, e,a,b,c,d);
        round40(in, 52, d,e,a,b,c);
        round40(in, 53, c,d,e,a,b);
        round40(in, 54, b,c,d,e,a);
        round40(in, 55, a,b,c,d,e);
        round40(in, 56, e,a,b,c,d);
        round40(in, 57, d,e,a,b,c);
        round40(in, 58, c,d,e,a,b);
        round40(in, 59, b,c,d,e,a);

        // Round4 60<= i <= 70
        round60(in, 60, a,b,c,d,e);
        round60(in, 61, e,a,b,c,d);
        round60(in, 62, d,e,a,b,c);
        round60(in, 63, c,d,e,a,b);
        round60(in, 64, b,c,d,e,a);
        round60(in, 65, a,b,c,d,e);
        round60(in, 66, e,a,b,c,d);
        round60(in, 67, d,e,a,b,c);
        round60(in, 68, c,d,e,a,b);
        round60(in, 69, b,c,d,e,a);
        round60(in, 70, a,b,c,d,e);
        round60(in, 71, e,a,b,c,d);
        round60(in, 72, d,e,a,b,c);
        round60(in, 73, c,d,e,a,b);
        round60(in, 74, b,c,d,e,a);
        round60(in, 75, a,b,c,d,e);
        round60(in, 76, e,a,b,c,d);
        round60(in, 77, d,e,a,b,c);
        round60(in, 78, c,d,e,a,b);
        round60(in, 79, b,c,d,e,a);

        buf[0] += a;
        buf[1] += b;
        buf[2] += c;
        buf[3] += d;
        buf[4] += e;

}

void SHA1::hashFinaleBlock(const unsigned char *byteBuf, unsigned short int len, size_t fileSize)
{
    uint32_t in[16];
    uint32_t a, b, c, d, e;
    int count = 1;

    if (len > 64)
        return;
    unsigned char *temp = new unsigned char[128];
    memset(temp,0,128);
    memcpy(temp,byteBuf,len);
    temp[len] = 128;
    if(len + 9 >= 64)
    {
        for(int i = 0; i < 16; i++)
        {
            in[i] = GET_32BIT_MSB_FIRST(temp+i*4);
        }
        count = 2;
    } else
    {
        for(int i = 0; i < 14; i++)
        {
            in[i] = GET_32BIT_MSB_FIRST(temp+i*4);
        }
        in[14] =(uint32_t) (fileSize*8 >> 32);
        in[15] = (uint32_t) (fileSize*8);

         count = 1;
    }

    do
    {
        a = buf[0];
        b = buf[1];
        c = buf[2];
        d = buf[3];
        e = buf[4];

        // Round1 0<= i <= 15
        round0(in,  0, a,b,c,d,e);
        round0(in,  1, e,a,b,c,d);
        round0(in,  2, d,e,a,b,c);
        round0(in,  3, c,d,e,a,b);
        round0(in,  4, b,c,d,e,a);
        round0(in,  5, a,b,c,d,e);
        round0(in,  6, e,a,b,c,d);
        round0(in,  7, d,e,a,b,c);
        round0(in,  8, c,d,e,a,b);
        round0(in,  9, b,c,d,e,a);
        round0(in, 10, a,b,c,d,e);
        round0(in, 11, e,a,b,c,d);
        round0(in, 12, d,e,a,b,c);
        round0(in, 13, c,d,e,a,b);
        round0(in, 14, b,c,d,e,a);
        round0(in, 15, a,b,c,d,e);

        // Round1 16<= i <= 19
        round16(in, 16, e,a,b,c,d);
        round16(in, 17, d,e,a,b,c);
        round16(in, 18, c,d,e,a,b);
        round16(in, 19, b,c,d,e,a);

        // Round2 20<= i <= 39
        round20(in, 20, a,b,c,d,e);
        round20(in, 21, e,a,b,c,d);
        round20(in, 22, d,e,a,b,c);
        round20(in, 23, c,d,e,a,b);
        round20(in, 24, b,c,d,e,a);
        round20(in, 25, a,b,c,d,e);
        round20(in, 26, e,a,b,c,d);
        round20(in, 27, d,e,a,b,c);
        round20(in, 28, c,d,e,a,b);
        round20(in, 29, b,c,d,e,a);
        round20(in, 30, a,b,c,d,e);
        round20(in, 31, e,a,b,c,d);
        round20(in, 32, d,e,a,b,c);
        round20(in, 33, c,d,e,a,b);
        round20(in, 34, b,c,d,e,a);
        round20(in, 35, a,b,c,d,e);
        round20(in, 36, e,a,b,c,d);
        round20(in, 37, d,e,a,b,c);
        round20(in, 38, c,d,e,a,b);
        round20(in, 39, b,c,d,e,a);

        // Round3 40<= i <= 59
        round40(in, 40, a,b,c,d,e);
        round40(in, 41, e,a,b,c,d);
        round40(in, 42, d,e,a,b,c);
        round40(in, 43, c,d,e,a,b);
        round40(in, 44, b,c,d,e,a);
        round40(in, 45, a,b,c,d,e);
        round40(in, 46, e,a,b,c,d);
        round40(in, 47, d,e,a,b,c);
        round40(in, 48, c,d,e,a,b);
        round40(in, 49, b,c,d,e,a);
        round40(in, 50, a,b,c,d,e);
        round40(in, 51, e,a,b,c,d);
        round40(in, 52, d,e,a,b,c);
        round40(in, 53, c,d,e,a,b);
        round40(in, 54, b,c,d,e,a);
        round40(in, 55, a,b,c,d,e);
        round40(in, 56, e,a,b,c,d);
        round40(in, 57, d,e,a,b,c);
        round40(in, 58, c,d,e,a,b);
        round40(in, 59, b,c,d,e,a);

        // Round4 60<= i <= 70
        round60(in, 60, a,b,c,d,e);
        round60(in, 61, e,a,b,c,d);
        round60(in, 62, d,e,a,b,c);
        round60(in, 63, c,d,e,a,b);
        round60(in, 64, b,c,d,e,a);
        round60(in, 65, a,b,c,d,e);
        round60(in, 66, e,a,b,c,d);
        round60(in, 67, d,e,a,b,c);
        round60(in, 68, c,d,e,a,b);
        round60(in, 69, b,c,d,e,a);
        round60(in, 70, a,b,c,d,e);
        round60(in, 71, e,a,b,c,d);
        round60(in, 72, d,e,a,b,c);
        round60(in, 73, c,d,e,a,b);
        round60(in, 74, b,c,d,e,a);
        round60(in, 75, a,b,c,d,e);
        round60(in, 76, e,a,b,c,d);
        round60(in, 77, d,e,a,b,c);
        round60(in, 78, c,d,e,a,b);
        round60(in, 79, b,c,d,e,a);

        buf[0] += a;
        buf[1] += b;
        buf[2] += c;
        buf[3] += d;
        buf[4] += e;

        count--;
        for(int i = 0; i < 14; i++)
        {
            in[i] = GET_32BIT_MSB_FIRST(temp + 64 +i*4);
        }

        in[14] = (uint32_t) (fileSize*8 >> 32);
        in[15] = (uint32_t) (fileSize*8);

      } while (count);

    delete[] temp;

}

unsigned char* SHA1::getHashResult()
{
    unsigned char *result = new unsigned char[20];
    PUT_32BIT_MSB_FIRST(result, buf[0]);
    PUT_32BIT_MSB_FIRST(result+4, buf[1]);
    PUT_32BIT_MSB_FIRST(result+8, buf[2]);
    PUT_32BIT_MSB_FIRST(result+12, buf[3]);
    PUT_32BIT_MSB_FIRST(result+16, buf[4]);
    return result;
}


void SHA1::round0( uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e)
{
    e += (( b & c ) | (~b & d)) + M[position] + 0x5A827999 + rotl(a, 5);
    b = rotl(b, 30);
}
void SHA1::round16( uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e)
{
    e += (( b & c ) | (~b & d)) + transformW(M,position) + 0x5A827999 + rotl(a, 5);
    b = rotl(b, 30);
}
void SHA1::round20( uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e)
{
    e += (b ^ c ^ d) + transformW(M,position) + 0x6ed9eba1 + rotl(a, 5);
    b = rotl(b, 30);
}
void SHA1::round40( uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e)
{
    e += ((b & (c | d)) | (c & d)) + transformW(M,position) + 0x8f1bbcdc + rotl(a, 5);
    b = rotl(b, 30);
}
void SHA1::round60( uint32_t *M, const uint32_t position,uint32_t &a, uint32_t &b, uint32_t &c, uint32_t &d, uint32_t &e)
{
    e += (b ^ c ^ d) + transformW(M,position) + 0xca62c1d6 + rotl(a, 5);
    b = rotl(b, 30);
}

uint32_t SHA1::transformW(uint32_t *M, const uint32_t position)
{
    M[position & 0xf] = rotl(  M[(position+13) & 0xf]
                                                 ^ M[(position+ 8) & 0xf]
                                                 ^ M[(position+ 2) & 0xf]
                                                 ^ M[(position)    & 0xf], 1);
    return M[position & 0xf];
}
