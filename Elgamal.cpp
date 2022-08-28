#include "Elgamal.h"
#include <QDebug>

Elgamal::Elgamal()
{
    mpz_init(generated_num);
    mpz_init(p_prime);
    mpz_init(q_prime);

    mpz_init(aliceOpenKey);
    mpz_init(aliceClosedKey);
    mpz_init(bobOpenKey);
    mpz_init(bobClosedKey);
    mpz_init(firstBlockKey);
    mpz_init(firstSignatureBlockKey);
    mpz_init(generator);
    moduleByteCount = 0;
    gmp_randinit_default(state);
    gmp_randseed_ui (state,time(nullptr) );

}

Elgamal::~Elgamal()
{
    mpz_clear(generated_num);
    mpz_clear(p_prime);
    mpz_clear(q_prime);
    mpz_clear(generator);
    mpz_clear(aliceOpenKey);
    mpz_clear(aliceClosedKey);
    mpz_clear(bobOpenKey);
    mpz_clear(bobClosedKey);
    mpz_clear(firstBlockKey);

}

void Elgamal::setBufferSize(size_t bufferSize)
{
    if(bufferSize > moduleByteCount)
        result = new char[bufferSize * 2];
    else
        result = new char[moduleByteCount * 4];
}

bool Elgamal::isPrime(const mpz_t candidate)
{
        if(mpz_even_p (candidate))
        {
            return false;
        }

        if(mpz_cmp_ui (candidate, 2)<1)
        {
            return false;
        }
        mpz_class num(candidate);
        mpz_class num_m(candidate);
        mpz_class random;
        num_m = num_m - 1;
        num = num -1;

        mpz_t pow_result;
        mpz_t sqr_result;

        mpz_init(pow_result);
        mpz_init(sqr_result);

        uint64_t s = 0;

        while ( num % 2 == 0) {
            num = num / 2;
            s++;
        }

        gmp_randclass r1 (gmp_randinit_default);
        r1.seed(time(nullptr));

        for(int i = 0; i < 40; i++)
        {
            random = r1.get_z_range(num_m - 3);
            random = random + 2;
            mpz_powm(pow_result, random.get_mpz_t(), num.get_mpz_t(), candidate);

            mpz_class pow_res(pow_result);

            if((pow_res ==  1) || (pow_res == num_m) ) {
                continue;
            }

            for(uint64_t j = 0; j < (s); j++) {
                mpz_powm_ui(sqr_result, pow_res.get_mpz_t(), 2, candidate);

                mpz_class sqr_res(sqr_result);

                if(sqr_res == 1)  {
                    mpz_clear(pow_result);
                    mpz_clear(sqr_result);
                    return false;
                }

                if(sqr_res == num_m) {
                    break;
                }
                pow_res = sqr_res;
                if (j  == (s-1)) {
                    mpz_clear(pow_result);
                    mpz_clear(sqr_result);
                    return false;
                }
            }


        }
        mpz_clear(pow_result);
        mpz_clear(sqr_result);
        return true;


    }


void Elgamal::generatePrimeNumbers(const int digits_count)
{

     mpz_t temp1;
     mpz_t temp2;
     mpz_t temp3;


     mpz_init(temp1);
     mpz_init(temp2);
     mpz_init(temp3);
     do {

     mpz_init_set_ui(temp1,10);


     mpz_pow_ui(temp2, temp1,digits_count);
     mpz_mul_ui(temp3,temp2,9);
     mpz_urandomm(temp1, state, temp3);
     mpz_add(temp3,temp2,temp1);
     mpz_nextprime(temp1,temp3);

     mpz_mul_ui(temp3,temp1,2);
     mpz_add_ui(temp2,temp3,1);

    } while(!isPrime(temp2));

    mpz_set(q_prime, temp1);
    mpz_set(p_prime,temp2);

    moduleByteCount = (mpz_sizeinbase(p_prime, 2) + 7) / 8;

    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(temp3);

}


void Elgamal::generateGeneratorNumber()
{

    mpz_t g_candidate;
    mpz_t temp;
    mpz_t lim;
    mpz_t random;

    mpz_init(g_candidate);
    mpz_init(temp);
    mpz_init(random);

    mpz_init_set(lim,p_prime);

    mpz_sub_ui(temp,lim,2);
    mpz_swap(temp,lim);

    do {
        mpz_urandomm(temp, state, lim);
        mpz_add_ui(random,temp,1);
        mpz_powm(temp,random, q_prime,p_prime);


    } while(!mpz_cmp_ui(temp,1));


    mpz_set(generator,random);

    mpz_clear(g_candidate);
    mpz_clear(temp);
    mpz_clear(lim);
    mpz_clear(random);
}

void Elgamal::generateClosedKey(mpz_t closedKey)
{
    mpz_t temp1;
    mpz_t temp2;
    mpz_t temp3;
    mpz_t temp4;
    mpz_t gcdR;

    mpz_init_set(temp1,p_prime);
    mpz_init(temp2);
    mpz_init(temp3);
    mpz_init(temp4);
    mpz_init(gcdR);

    mpz_sub_ui(temp2, temp1,1);
    mpz_sub_ui(temp3, temp1,2);

    do
    {
        mpz_urandomm(temp1, state, temp3);
        mpz_add_ui(temp4,temp1,1);
        mpz_gcd(gcdR,temp4,temp2);
    } while(mpz_cmp_ui(gcdR,1) !=0);
    mpz_set(closedKey,temp4);

    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(temp3);
    mpz_clear(temp4);
    mpz_clear(gcdR);
}

void Elgamal::generateOpenKey(mpz_t openKey, const mpz_t closedKey)
{
    mpz_powm(openKey,generator,closedKey,p_prime);
}


void Elgamal::generateAliceKeys()
{
    generateClosedKey(aliceClosedKey);
    generateOpenKey(aliceOpenKey,aliceClosedKey);
    qDebug()<< "y = " << mpz_get_str(NULL,10,aliceOpenKey);
    qDebug()<< "x = " << mpz_get_str(NULL,10,aliceClosedKey);

}
void Elgamal::generateBobKeys()
{
    generateClosedKey(bobClosedKey);
    generateOpenKey(bobOpenKey,bobClosedKey);
    qDebug()<< "r = " << mpz_get_str(NULL,10,bobOpenKey);
    qDebug()<< "k = " << mpz_get_str(NULL,10,bobClosedKey);
}

bool Elgamal::setOpenKeyParametrs(const char *y, const char *g, const char *p)
{
    mpz_set_str(p_prime, p,10);
    if(!isPrime(p_prime))
    {
        return false;
    }
    mpz_set_str(generator,g,10);
    if(mpz_cmp(generator,p_prime) >= 0)
    {
        return false;
    }

    mpz_set_str(aliceOpenKey, y,10);
    if(mpz_cmp(aliceOpenKey,p_prime) >= 0)
    {
        return false;
    }

    return true;

}
bool Elgamal::setPrivateKeyParametrs(const char *x, const char *k, const char *g, const char *p)
{
    mpz_set_str(p_prime, p,10);
    if(!isPrime(p_prime))
    {
        return false;
    }
    mpz_set_str(generator,g,10);
    if(mpz_cmp(generator,p_prime) >= 0)
    {
        return false;
    }

    mpz_set_str(bobClosedKey, k,10);
    if(mpz_cmp(bobClosedKey,p_prime) >= 0)
    {
        return false;
    }

    mpz_set_str(aliceClosedKey, x,10);
    if(mpz_cmp(aliceClosedKey,p_prime) >= 0)
    {
        return false;
    }
    generateOpenKey(bobOpenKey,bobClosedKey);
    generateOpenKey(aliceOpenKey,aliceClosedKey);
    return true;
}

void Elgamal::encryptBlock(mpz_t result,const mpz_t openKey, const mpz_t closedKey,const mpz_t message)
{
    mpz_t temp1;
    mpz_t temp2;
    mpz_init(temp1);
    mpz_init(temp2);

    mpz_powm(temp1,openKey,closedKey,p_prime);
    mpz_mul(temp2,temp1, message);

    mpz_mod(result, temp2,p_prime);

   mpz_clear(temp1);
   mpz_clear(temp2);



}

void Elgamal::decryptBlock(mpz_t result,const mpz_t openKey, const mpz_t closedKey,const mpz_t message)
{
    mpz_t temp1;
    mpz_t temp2;
    mpz_init(temp1);
    mpz_init(temp2);

    mpz_sub(temp1, p_prime, closedKey);
    mpz_sub_ui(temp2,temp1,1);
    mpz_powm(temp1,openKey,temp2,p_prime);
    mpz_mul(temp2,message,temp1);
    mpz_mod(result,temp2,p_prime);

    mpz_clear(temp1);
    mpz_clear(temp2);

}
char* Elgamal::writeFirstBlock(size_t *result_size)
{
    char *res;
    mpz_t firstBlock;
    mpz_init(firstBlock);
    mpz_powm(firstBlock,generator,aliceClosedKey,p_prime);
    size_t encrypted_data_byte_count = (mpz_sizeinbase(firstBlock, 2) + 7) / 8;
    res = new char [moduleByteCount];
    size_t g = 0;
    mpz_export(res + (moduleByteCount - encrypted_data_byte_count), &g, 1, sizeof(char), 0, 0, firstBlock);
    *result_size = moduleByteCount;
    return res;
}

void Elgamal::readFirstBlock(const char *buffer,const size_t len)
{
    mpz_import(firstBlockKey, len, 1, sizeof(char), 0, 0, buffer);
}

char* Elgamal::encrypt(const char *buffer, const size_t len, size_t *result_size, bool last_block)
{
    //qDebug()<<"encryption -----------------------";
   *result_size = 0;
   size_t left_count = len % (moduleByteCount-1);
   size_t count = 0;
   if(left_count == 0)
   {
        count = len / (moduleByteCount-1); // количество раз, которое нужно будет считать по Модуль-1 байт
        left_count = (moduleByteCount-1);
   }
   else
   {
        count = len / (moduleByteCount-1) + 1;
        if ( len < (moduleByteCount-1))
        {
            left_count = (moduleByteCount-1);
        }
   }
   size_t size = count * moduleByteCount;
   memset(result,0,size);
   mpz_t data;
   mpz_t encrypted_data;

   mpz_init(data);
   mpz_init(encrypted_data);
   mpz_t temp1;
   mpz_t temp2;
   mpz_init(temp1);
   mpz_init(temp2);

   for(size_t i = 0; i < count; i++)
   {

       size_t g;

       if ( last_block && i == (count-1))
       {
           char *temp_data = new char[moduleByteCount-1];
           memset(temp_data,0,moduleByteCount-1);
           memcpy(temp_data,buffer + (moduleByteCount-1) * i,left_count);
           mpz_import(data, (moduleByteCount-1), 1, sizeof(char), 0, 0, temp_data);
           delete [] temp_data;
       } else {
           mpz_import(data, moduleByteCount-1, 1, sizeof(char), 0, 0, buffer + (moduleByteCount-1) * i);
       }
       mpz_powm(temp1,bobOpenKey,aliceClosedKey,p_prime);
       mpz_mul(temp2,temp1, data);

       mpz_mod(encrypted_data, temp2,p_prime);

       size_t encrypted_data_byte_count = (mpz_sizeinbase(encrypted_data, 2) + 7) / 8;
       mpz_export(result + moduleByteCount * i + (moduleByteCount - encrypted_data_byte_count), &g, 1, sizeof(char), 0, 0, encrypted_data);
       *result_size+=moduleByteCount;
   }

   mpz_clear(data);
   mpz_clear(encrypted_data);
   mpz_clear(temp1);
   mpz_clear(temp2);



   return result;
}

char* Elgamal::decrypt(const char *buffer, const size_t len, size_t *result_size)
{
    //qDebug()<<"decryption -------------------------";
   *result_size = 0;
   size_t left_count = len % moduleByteCount;
   size_t count = 0;

   if(left_count == 0)
   {
        count = len / moduleByteCount; // количество раз, которое нужно будет считать по Модуль-1 байт
        left_count = moduleByteCount;
   }
   else
   {
        count = len / moduleByteCount + 1;
        if ( len < moduleByteCount)
        {
            left_count = moduleByteCount;
        }
   }

  // char *result = new char[len];
   memset(result,0,len);

   mpz_t data;
   mpz_t encrypted_data;

   mpz_init(data);
   mpz_init(encrypted_data);
   mpz_t temp1;
   mpz_t temp2;
   mpz_init(temp1);
   mpz_init(temp2);




   for(size_t i = 0; i < count; i++)
   {
       size_t g = 0;
       mpz_import(encrypted_data, moduleByteCount, 1, sizeof(char), 0, 0, buffer + moduleByteCount * i);
       mpz_sub(temp1, p_prime, bobClosedKey);
       mpz_sub_ui(temp2,temp1,1);
       mpz_powm(temp1,firstBlockKey,temp2,p_prime);
       mpz_mul(temp2,encrypted_data,temp1);
       mpz_mod(data,temp2,p_prime);
       size_t encrypted_data_byte_count = (mpz_sizeinbase(data, 2) + 7) / 8;
       if( (moduleByteCount - 1 - encrypted_data_byte_count) < moduleByteCount)
       {
          mpz_export(result + (moduleByteCount - 1) * i + (moduleByteCount - 1 - encrypted_data_byte_count), &g, 1, sizeof(char), 0, 0, data);
       } else {
          mpz_export(result + (moduleByteCount - 1) * i, &g, 1, sizeof(char), 0, 0, data);
       }
       *result_size+=(moduleByteCount - 1);
   }

   mpz_clear(data);
   mpz_clear(encrypted_data);
   mpz_clear(temp1);
   mpz_clear(temp2);



   return result;
}

char* Elgamal::writeSignatureFirstBlock(size_t *result_size)
{
    char *res;
    mpz_t firstBlock;
    mpz_init(firstBlock);
    mpz_powm(firstBlock,generator,bobClosedKey,p_prime);
    size_t encrypted_data_byte_count = (mpz_sizeinbase(firstBlock, 2) + 7) / 8;
    res = new char [moduleByteCount];
    memset(res,0,moduleByteCount);
    size_t g = 0;
    mpz_export(res + (moduleByteCount - encrypted_data_byte_count), &g, 1, sizeof(char), 0, 0, firstBlock);
    qDebug()<<"write r ="<< mpz_get_str(NULL,10,firstBlock);
    *result_size = moduleByteCount;
    return res;
}

void Elgamal::readSignatureFirstBlock(const char *buffer,const size_t len)
{
    mpz_import(firstSignatureBlockKey, len, 1, sizeof(char), 0, 0, buffer);
    qDebug()<< "read r = " <<mpz_get_str(NULL,10,firstSignatureBlockKey);
}



char* Elgamal::sign(const char *buffer, const size_t len, size_t *result_size)
{

   *result_size = 0;
   size_t left_count = len % (moduleByteCount-1);
   size_t count = 0;
   if(left_count == 0)
   {
        count = len / (moduleByteCount-1); // количество раз, которое нужно будет считать по Модуль-1 байт
        left_count = (moduleByteCount-1);
   }
   else
   {
        count = len / (moduleByteCount-1) + 1;
        if ( len < (moduleByteCount-1))
        {
            left_count = (moduleByteCount-1);
        }
   }
   size_t size = count * moduleByteCount;
   memset(result,0,size);
   mpz_t data;
   mpz_t encrypted_data;

   mpz_init(data);
   mpz_init(encrypted_data);
   mpz_t temp1;
   mpz_t temp2;
   mpz_init(temp1);
   mpz_init(temp2);

   mpz_t prime_1;
   mpz_init(prime_1);
   mpz_sub_ui(prime_1, p_prime,1);

   mpz_t temp3;
   mpz_init(temp3);

   for(size_t i = 0; i < count; i++)
   {
       size_t g;

       if (  i == (count-1))
       {
           char *temp_data = new char[moduleByteCount-1];
           memset(temp_data,0,moduleByteCount-1);
           memcpy(temp_data,buffer + (moduleByteCount-1) * i,left_count);
           mpz_import(data, (moduleByteCount-1), 1, sizeof(char), 0, 0, temp_data);
           delete [] temp_data;
       } else {
           mpz_import(data, moduleByteCount-1, 1, sizeof(char), 0, 0, buffer + (moduleByteCount-1) * i);
       }
       mpz_mul(temp1,aliceClosedKey,bobOpenKey);
       mpz_sub(temp2, data,temp1);
       mpz_invert(temp1,bobClosedKey,prime_1);
       mpz_mul(temp3,temp1,temp2);
       mpz_mod(encrypted_data, temp3,prime_1);
       size_t encrypted_data_byte_count = (mpz_sizeinbase(encrypted_data, 2) + 7) / 8;
       mpz_export(result + moduleByteCount * i + (moduleByteCount - encrypted_data_byte_count), &g, 1, sizeof(char), 0, 0, encrypted_data);
       *result_size+=moduleByteCount;
   }

   mpz_clear(data);
   mpz_clear(encrypted_data);
   mpz_clear(temp1);
   mpz_clear(temp2);
   mpz_clear(temp3);
   mpz_clear(prime_1);



   return result;
}

bool Elgamal::checkSign(const char *buffer, const size_t len, const char *hash, const size_t hashLen, size_t *result_size)
{
    //qDebug()<<"decryption -------------------------";
   *result_size = 0;
   size_t left_count = len % moduleByteCount;
   size_t count = 0;

   if(left_count == 0)
   {
        count = len / moduleByteCount; // количество раз, которое нужно будет считать по Модуль-1 байт
        left_count = moduleByteCount;
   }
   else
   {
        count = len / moduleByteCount + 1;
        if ( len < moduleByteCount)
        {
            left_count = moduleByteCount;
        }
   }

   //-------------------------------
   size_t hash_left_count = hashLen % (moduleByteCount-1);
   size_t hash_count = 0;
   if(hash_left_count == 0)
   {
        hash_count = hashLen / (moduleByteCount-1); // количество раз, которое нужно будет считать по Модуль-1 байт
        hash_left_count = (moduleByteCount-1);
   }
   else
   {
        hash_count = hashLen / (moduleByteCount-1) + 1;
        if ( hashLen < (moduleByteCount-1))
        {
            hash_left_count = (moduleByteCount-1);
        }
   }
   //-------------------------------

  // char *result = new char[len];
   memset(result,0,len);

   mpz_t data;
   mpz_t hashData;
   mpz_t encrypted_data;

   mpz_init(data);
   mpz_init(hashData);
   mpz_init(encrypted_data);
   mpz_t temp1;
   mpz_t temp2;
   mpz_init(temp1);
   mpz_init(temp2);
   mpz_t prime_1;
   mpz_init(prime_1);
   mpz_sub_ui(prime_1, p_prime,1);


   if(mpz_cmp(firstSignatureBlockKey,p_prime) > 0)
   {
       return false;
   }

   for(size_t i = 0; i < count; i++)
   {
       size_t g = 0;

       if (  i == (hash_count-1))
       {
           char *temp_data = new char[moduleByteCount-1];
           memset(temp_data,0,moduleByteCount-1);
           memcpy(temp_data,hash + (moduleByteCount-1) * i,hash_left_count);
           mpz_import(hashData, (moduleByteCount-1), 1, sizeof(char), 0, 0, temp_data);
           delete [] temp_data;
       } else {
           mpz_import(hashData, moduleByteCount-1, 1, sizeof(char), 0, 0, hash + (moduleByteCount-1) * i);
       }

       mpz_import(encrypted_data, moduleByteCount, 1, sizeof(char), 0, 0, buffer + moduleByteCount * i);

       if(mpz_cmp(encrypted_data,prime_1) > 0)
       {
           return false;
       }
       mpz_powm(temp1,aliceOpenKey,firstSignatureBlockKey,p_prime);
       mpz_powm(temp2,firstSignatureBlockKey,encrypted_data,p_prime);
       mpz_mul(data,temp1,temp2);
       mpz_mod(temp2,data,p_prime);

        mpz_powm(temp1,generator,hashData,p_prime);
       if(mpz_cmp(temp2,temp1) != 0)
       {
           return false;
       }

   }

   mpz_clear(data);
   mpz_clear(hashData);
   mpz_clear(encrypted_data);
   mpz_clear(temp1);
   mpz_clear(temp2);
   mpz_clear(prime_1);
   return true;
}


char* Elgamal::getStrParam(size_t *size, mpz_t param)
{
    *size = mpz_sizeinbase(param, 10);
    char *strparam = new(std::nothrow) char[*size+1];
    *size += 1;
    mpz_get_str(strparam, 10, param);
    if (strparam == nullptr)
    {
        *size = 0;
        return nullptr;
    } else return strparam;
}

char* Elgamal::getStrAliceOpenKey(size_t *size)
{
    return getStrParam(size, aliceOpenKey);
}
char* Elgamal::getStrAliceClosedKey(size_t *size)
{
    return getStrParam(size, aliceClosedKey);
}
char* Elgamal::getStrBobOpenKey(size_t *size)
{
    return getStrParam(size, bobOpenKey);
}
char* Elgamal::getStrBobClosedKey(size_t *size)
{
    return getStrParam(size, bobClosedKey);
}
char* Elgamal::getStrFirstBlockdKey(size_t *size)
{
    return getStrParam(size, firstBlockKey);
}

char* Elgamal::getStrGenerator(size_t *size)
{
    return getStrParam(size, generator);
}
char* Elgamal::getStrModule(size_t *size)
{
    return getStrParam(size, p_prime);
}

size_t Elgamal::getModuleByteCount()
{
    return moduleByteCount;
}

