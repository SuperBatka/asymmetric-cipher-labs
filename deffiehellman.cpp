#include "deffiehellman.h"

Deffiehellman::Deffiehellman()
{
    gmp_randinit_default(state);
    gmp_randseed_ui (state,time(nullptr) );
}


bool Deffiehellman::isPrime(const mpz_t candidate)
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

char* Deffiehellman::generatePrimeNumbers(char *&q_prime_str,const int digits_count)
{

     mpz_t temp1;
     mpz_t temp2;
     mpz_t temp3;

     size_t size;
     char *result_str;
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

     size = (mpz_sizeinbase(temp1, 2) + 7) / 8;
     q_prime_str = new char[100];
     q_prime_str = mpz_get_str(q_prime_str,10,temp1);

     size = (mpz_sizeinbase(temp2, 2) + 7) / 8;
     result_str = new char[100];
    result_str = mpz_get_str(result_str,10,temp2);
    mpz_clear(temp1);
    mpz_clear(temp2);
    mpz_clear(temp3);
    return result_str;

}


char* Deffiehellman::generateGeneratorNumber(const char *pprime, const char *qprime)
{
    mpz_t prime_p;
    mpz_t prime_q;
    mpz_t g_candidate;
    mpz_t temp;
    mpz_t lim;
    mpz_t random;
    char *result;

    mpz_init_set_str(prime_p, pprime,10);
    mpz_init_set_str(prime_q,qprime,10);
    mpz_init(g_candidate);
    mpz_init(temp);
    mpz_init(random);

    mpz_init_set(lim,prime_p);

    mpz_sub_ui(temp,lim,2);
    mpz_swap(temp,lim);

    do {
        mpz_urandomm(temp, state, lim);
        mpz_add_ui(random,temp,1);
        mpz_powm(temp,random, prime_q,prime_p);


    } while(!mpz_cmp_ui(temp,1));

    size_t size = (mpz_sizeinbase(random, 2) + 7) / 8;
    result = new char[100];
    result = mpz_get_str(result,10,random);


    mpz_clear(prime_p);
    mpz_clear(prime_q);
    mpz_clear(g_candidate);
    mpz_clear(temp);
    mpz_clear(lim);
    mpz_clear(random);
    return result;
}

char* Deffiehellman::generateNumber(const int digits_count)
{
    mpz_t temp1;
    mpz_t temp2;
    mpz_t temp3;

    size_t size;
    char *result_str;
    mpz_init(temp1);
    mpz_init(temp2);
    mpz_init(temp3);


    mpz_init_set_ui(temp1,10);


    mpz_pow_ui(temp2, temp1,digits_count);
    mpz_mul_ui(temp3,temp2,9);
    mpz_urandomm(temp1, state, temp3);
    mpz_add(temp3,temp2,temp1);



    size = (mpz_sizeinbase(temp3, 2) + 7) / 8;
    result_str = new char[100];
   result_str = mpz_get_str(result_str,10,temp3);

   mpz_clear(temp1);
   mpz_clear(temp2);
   mpz_clear(temp3);
   return result_str;

}
char* Deffiehellman::calculate(const char *baseBuffer, const char *expBuffer, const char *modBuffer)
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
     result_str = new char[100];
     result_str = mpz_get_str(result_str,10,result);


     mpz_clear(base);
     mpz_clear(exp);
     mpz_clear(mod);
     mpz_clear(result);

     return result_str;

}
