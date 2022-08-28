#include "md5.h"

MD5::MD5() : HashI()
{
    Init();
}

void MD5::Init()
{
    buf[0] = 0x67452301;
    buf[1] = 0xefcdab89;
    buf[2] = 0x98badcfe;
    buf[3] = 0x10325476;

}
void MD5::hashBlock(const unsigned char *byteBuf)
{
        uint32_t in[16];
        uint32_t a, b, c, d;

        for(int i = 0; i < 16; i++)
        {
            in[i] = GET_32BIT_LSB_FIRST(byteBuf+i*4);
        }


        a = buf[0];
        b = buf[1];
        c = buf[2];
        d = buf[3];

        ROUND(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
        ROUND(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
        ROUND(F1, c, d, a, b, in[2] + 0x242070db, 17);
        ROUND(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
        ROUND(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
        ROUND(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
        ROUND(F1, c, d, a, b, in[6] + 0xa8304613, 17);
        ROUND(F1, b, c, d, a, in[7] + 0xfd469501, 22);
        ROUND(F1, a, b, c, d, in[8] + 0x698098d8, 7);
        ROUND(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
        ROUND(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
        ROUND(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
        ROUND(F1, a, b, c, d, in[12] + 0x6b901122, 7);
        ROUND(F1, d, a, b, c, in[13] + 0xfd987193, 12);
        ROUND(F1, c, d, a, b, in[14] + 0xa679438e, 17);
        ROUND(F1, b, c, d, a, in[15] + 0x49b40821, 22);

        ROUND(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
        ROUND(F2, d, a, b, c, in[6] + 0xc040b340, 9);
        ROUND(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
        ROUND(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
        ROUND(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
        ROUND(F2, d, a, b, c, in[10] + 0x02441453, 9);
        ROUND(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
        ROUND(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
        ROUND(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
        ROUND(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
        ROUND(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
        ROUND(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
        ROUND(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
        ROUND(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
        ROUND(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
        ROUND(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

        ROUND(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
        ROUND(F3, d, a, b, c, in[8] + 0x8771f681, 11);
        ROUND(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
        ROUND(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
        ROUND(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
        ROUND(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
        ROUND(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
        ROUND(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
        ROUND(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
        ROUND(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
        ROUND(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
        ROUND(F3, b, c, d, a, in[6] + 0x04881d05, 23);
        ROUND(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
        ROUND(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
        ROUND(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
        ROUND(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);
                
        ROUND(F4, a, b, c, d, in[0] + 0xf4292244, 6);
        ROUND(F4, d, a, b, c, in[7] + 0x432aff97, 10);
        ROUND(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
        ROUND(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
        ROUND(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
        ROUND(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
        ROUND(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
        ROUND(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
        ROUND(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
        ROUND(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
        ROUND(F4, c, d, a, b, in[6] + 0xa3014314, 15);
        ROUND(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
        ROUND(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
        ROUND(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
        ROUND(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
        ROUND(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

        buf[0] += a;
        buf[1] += b;
        buf[2] += c;
        buf[3] += d;

}

void MD5::hashFinaleBlock(const unsigned char *byteBuf, unsigned short int len, size_t fileSize)
{
    uint32_t in[16];
    uint32_t a, b, c, d;
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
            in[i] = GET_32BIT_LSB_FIRST(temp+i*4);
        }
        count = 2;
    } else
    {
        for(int i = 0; i < 14; i++)
        {
            in[i] = GET_32BIT_LSB_FIRST(temp+i*4);
        }
        in[14] = (uint32_t) (fileSize*8);
        in[15] =(uint32_t) (fileSize*8 >> 32);

         count = 1;
    }

    do
    {
        a = buf[0];
        b = buf[1];
        c = buf[2];
        d = buf[3];


        ROUND(F1, a, b, c, d, in[0] + 0xd76aa478, 7);
        ROUND(F1, d, a, b, c, in[1] + 0xe8c7b756, 12);
        ROUND(F1, c, d, a, b, in[2] + 0x242070db, 17);
        ROUND(F1, b, c, d, a, in[3] + 0xc1bdceee, 22);
        ROUND(F1, a, b, c, d, in[4] + 0xf57c0faf, 7);
        ROUND(F1, d, a, b, c, in[5] + 0x4787c62a, 12);
        ROUND(F1, c, d, a, b, in[6] + 0xa8304613, 17);
        ROUND(F1, b, c, d, a, in[7] + 0xfd469501, 22);
        ROUND(F1, a, b, c, d, in[8] + 0x698098d8, 7);
        ROUND(F1, d, a, b, c, in[9] + 0x8b44f7af, 12);
        ROUND(F1, c, d, a, b, in[10] + 0xffff5bb1, 17);
        ROUND(F1, b, c, d, a, in[11] + 0x895cd7be, 22);
        ROUND(F1, a, b, c, d, in[12] + 0x6b901122, 7);
        ROUND(F1, d, a, b, c, in[13] + 0xfd987193, 12);
        ROUND(F1, c, d, a, b, in[14] + 0xa679438e, 17);
        ROUND(F1, b, c, d, a, in[15] + 0x49b40821, 22);

        ROUND(F2, a, b, c, d, in[1] + 0xf61e2562, 5);
        ROUND(F2, d, a, b, c, in[6] + 0xc040b340, 9);
        ROUND(F2, c, d, a, b, in[11] + 0x265e5a51, 14);
        ROUND(F2, b, c, d, a, in[0] + 0xe9b6c7aa, 20);
        ROUND(F2, a, b, c, d, in[5] + 0xd62f105d, 5);
        ROUND(F2, d, a, b, c, in[10] + 0x02441453, 9);
        ROUND(F2, c, d, a, b, in[15] + 0xd8a1e681, 14);
        ROUND(F2, b, c, d, a, in[4] + 0xe7d3fbc8, 20);
        ROUND(F2, a, b, c, d, in[9] + 0x21e1cde6, 5);
        ROUND(F2, d, a, b, c, in[14] + 0xc33707d6, 9);
        ROUND(F2, c, d, a, b, in[3] + 0xf4d50d87, 14);
        ROUND(F2, b, c, d, a, in[8] + 0x455a14ed, 20);
        ROUND(F2, a, b, c, d, in[13] + 0xa9e3e905, 5);
        ROUND(F2, d, a, b, c, in[2] + 0xfcefa3f8, 9);
        ROUND(F2, c, d, a, b, in[7] + 0x676f02d9, 14);
        ROUND(F2, b, c, d, a, in[12] + 0x8d2a4c8a, 20);

        ROUND(F3, a, b, c, d, in[5] + 0xfffa3942, 4);
        ROUND(F3, d, a, b, c, in[8] + 0x8771f681, 11);
        ROUND(F3, c, d, a, b, in[11] + 0x6d9d6122, 16);
        ROUND(F3, b, c, d, a, in[14] + 0xfde5380c, 23);
        ROUND(F3, a, b, c, d, in[1] + 0xa4beea44, 4);
        ROUND(F3, d, a, b, c, in[4] + 0x4bdecfa9, 11);
        ROUND(F3, c, d, a, b, in[7] + 0xf6bb4b60, 16);
        ROUND(F3, b, c, d, a, in[10] + 0xbebfbc70, 23);
        ROUND(F3, a, b, c, d, in[13] + 0x289b7ec6, 4);
        ROUND(F3, d, a, b, c, in[0] + 0xeaa127fa, 11);
        ROUND(F3, c, d, a, b, in[3] + 0xd4ef3085, 16);
        ROUND(F3, b, c, d, a, in[6] + 0x04881d05, 23);
        ROUND(F3, a, b, c, d, in[9] + 0xd9d4d039, 4);
        ROUND(F3, d, a, b, c, in[12] + 0xe6db99e5, 11);
        ROUND(F3, c, d, a, b, in[15] + 0x1fa27cf8, 16);
        ROUND(F3, b, c, d, a, in[2] + 0xc4ac5665, 23);

        ROUND(F4, a, b, c, d, in[0] + 0xf4292244, 6);
        ROUND(F4, d, a, b, c, in[7] + 0x432aff97, 10);
        ROUND(F4, c, d, a, b, in[14] + 0xab9423a7, 15);
        ROUND(F4, b, c, d, a, in[5] + 0xfc93a039, 21);
        ROUND(F4, a, b, c, d, in[12] + 0x655b59c3, 6);
        ROUND(F4, d, a, b, c, in[3] + 0x8f0ccc92, 10);
        ROUND(F4, c, d, a, b, in[10] + 0xffeff47d, 15);
        ROUND(F4, b, c, d, a, in[1] + 0x85845dd1, 21);
        ROUND(F4, a, b, c, d, in[8] + 0x6fa87e4f, 6);
        ROUND(F4, d, a, b, c, in[15] + 0xfe2ce6e0, 10);
        ROUND(F4, c, d, a, b, in[6] + 0xa3014314, 15);
        ROUND(F4, b, c, d, a, in[13] + 0x4e0811a1, 21);
        ROUND(F4, a, b, c, d, in[4] + 0xf7537e82, 6);
        ROUND(F4, d, a, b, c, in[11] + 0xbd3af235, 10);
        ROUND(F4, c, d, a, b, in[2] + 0x2ad7d2bb, 15);
        ROUND(F4, b, c, d, a, in[9] + 0xeb86d391, 21);

        buf[0] += a;
        buf[1] += b;
        buf[2] += c;
        buf[3] += d;

        count--;
        for(int i = 0; i < 14; i++)
        {
            in[i] = GET_32BIT_LSB_FIRST(temp + 64 +i*4);
        }

        in[14] = (uint32_t) (fileSize*8);
        in[15] = (uint32_t) (fileSize*8 >> 32);

      } while (count);
    delete [] temp;

}

unsigned char* MD5::getHashResult()
{
    unsigned char *result = new unsigned char[16];
    PUT_32BIT_LSB_FIRST(result, buf[0]);
    PUT_32BIT_LSB_FIRST(result+4, buf[1]);
    PUT_32BIT_LSB_FIRST(result+8, buf[2]);
    PUT_32BIT_LSB_FIRST(result+12, buf[3]);
    return result;
}
