#include "shamir.h"

shamir::shamir()
{
    gmp_randinit_default(state);
    gmp_randseed_ui (state,time(nullptr) );
    mpz_init(prime);
    modSize = 0;

}

shamir::~shamir()
{
    mpz_clear(prime);

}




/*void shamir::generatePrimeNumber(const int digits_count)
{

     mpz_t temp1;
     mpz_t temp2;
     mpz_t temp3;

     mpz_init(temp1);
     mpz_init(temp2);
     mpz_init(temp3);

     mpz_init_set_ui(temp1,10);

     mpz_pow_ui(temp2, temp1,digits_count+5);
     mpz_mul_ui(temp3,temp2,9);
     mpz_urandomm(temp1, state, temp3);
     mpz_add(temp3,temp2,temp1);
     mpz_nextprime(prime,temp3);
     modSize = (mpz_sizeinbase(prime, 2) + 7) / 8;


    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(temp3);


}

void shamir::generatePair(char *&first, char *&second)
{
    mpz_t temp1;
    mpz_t temp2;
    mpz_t temp3;

    mpz_t mod_t;
    mpz_init(temp1);
    mpz_init(temp2);
    mpz_init(temp3);



    mpz_init_set(mod_t, prime);
    mpz_sub_ui(temp3,mod_t,1);

    do
    {
        mpz_urandomm(temp1, state, mod_t);
    } while(!mpz_invert(temp2,temp1,temp3));

    size_t size = (mpz_sizeinbase(temp2, 2) + 7) / 8;
    second = new char[size+2];
    second = mpz_get_str(second,10,temp2);

    size = (mpz_sizeinbase(temp1, 2) + 7) / 8;
    first = new char[size+2];
    first = mpz_get_str(first,10,temp1);

    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(temp3);
    mpz_clear(mod_t);

}


bool shamir::isPrime(const mpz_t candidate)
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


char* shamir::calculate(const char *baseBuffer, size_t len,const char *expBuffer)
{

    size_t left_count = len % (modSize-1);
    size_t count = 0;
    if(left_count == 0)
    {
         count = len / (modSize-1); // количество раз, которое нужно будет считать по Модуль-1 байт
         left_count = (modSize-1);
    }
    else
    {
         count = len / (modSize-1) + 1;
         if ( len < (modSize-1))
         {
             left_count = (modSize-1);
         }
    }
    size_t size = count * modSize;
     mpz_t base;
     mpz_t exp;
     mpz_t mod;

     mpz_t result;
     char *result_str;

     mpz_init_set_str(base,baseBuffer,10);
     mpz_init_set_str(exp,expBuffer,10);
     mpz_init_set(mod,prime);
     mpz_init(result);
     mpz_powm(result,base,exp,mod);
     size_t size = (mpz_sizeinbase(result, 2) + 7) / 8;
     result_str = new char[size+2];
     result_str = mpz_get_str(result_str,10,result);


     mpz_clear(base);
     mpz_clear(exp);
     mpz_clear(mod);
     mpz_clear(result);

     return result_str;

} */




char* shamir::generatePrimeNumber(const char *message, const int digits_count)
{

     mpz_t temp1;
     mpz_t temp2;
     mpz_t temp3;
     mpz_t message_t;

     size_t size;
     char *result_str;
     mpz_init(temp1);
     mpz_init(temp2);
     mpz_init(temp3);
     mpz_init_set_str(message_t,message,10);



     mpz_init_set_ui(temp1,10);


     mpz_pow_ui(temp2, temp1,digits_count+5);
     mpz_mul_ui(temp3,temp2,9);
     mpz_urandomm(temp1, state, temp3);
     mpz_add(temp3,temp2,temp1);
     mpz_add(temp2,temp3,message_t);
     mpz_nextprime(temp1,temp2);

     size = (mpz_sizeinbase(temp1, 2) + 7) / 8;
     result_str = new char[size+2];
    result_str = mpz_get_str(result_str,10,temp1);
    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(temp3);
    return result_str;

}

void shamir::generatePair(char *&first, char *&second, const char *mod)
{
    mpz_t temp1;
    mpz_t temp2;
    mpz_t temp3;

    mpz_t mod_t;
    mpz_init(temp1);
    mpz_init(temp2);
    mpz_init(temp3);



    mpz_init_set_str(mod_t, mod, 10);
    mpz_sub_ui(temp3,mod_t,1);

    do
    {
        mpz_urandomm(temp1, state, mod_t);
    } while(!mpz_invert(temp2,temp1,temp3));

    size_t size = (mpz_sizeinbase(temp2, 2) + 7) / 8;
    second = new char[size+2];
    second = mpz_get_str(second,10,temp2);

    size = (mpz_sizeinbase(temp1, 2) + 7) / 8;
    first = new char[size+2];
    first = mpz_get_str(first,10,temp1);

    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(temp3);
    mpz_clear(mod_t);

}


bool shamir::isPrime(const mpz_t candidate)
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


char* shamir::calculate(const char *baseBuffer, const char *expBuffer, const char *modBuffer)
{
     mpz_t base;
     mpz_t exp;
     mpz_t mod;

     mpz_t result;
     char *result_str;

     mpz_init_set_str(base,baseBuffer,10);
     mpz_init_set_str(exp,expBuffer,10);
     mpz_init_set_str(mod,modBuffer,10);
     mpz_init(result);
     mpz_powm(result,base,exp,mod);
     size_t size = (mpz_sizeinbase(result, 2) + 7) / 8;
     result_str = new char[size+2];
     result_str = mpz_get_str(result_str,10,result);


     mpz_clear(base);
     mpz_clear(exp);
     mpz_clear(mod);
     mpz_clear(result);

     return result_str;

}
