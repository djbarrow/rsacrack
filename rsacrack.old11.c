#define TRUE  (1)
#define FALSE (0)
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#include <stdio.h>
#include <gmp.h>
#include <signal.h>
#include <setjmp.h>

typedef unsigned long uint_t;
#define UINT_FMT "%ld"
#define UINT_MAX_T ((uint_t)-1)
typedef unsigned char u8; 

mpz_t composite,temp_composite,test_num;
mpz_t temp_quotent,temp_remainder;
mpz_t temp_prime,temp_modulo;




typedef unsigned long prime_t;
#define MAX_PRIME ((prime_t)-10000)


volatile uint_t numprimes;
prime_t *primelist;
jmp_buf      timeoutenv;
 
void timeoutfunc(int signal)
{
   longjmp(timeoutenv,-1);
}

void makeprimelist(volatile uint_t max_numprimes,int timeout)
{
   uint_t last_prime=2,max_delta=0,curr_delta;
   uint_t hi_int,idx1;
   uint_t idx2;
   uint_t root=2,root_sqr;

   numprimes=0;
   if(timeout>=0)
      signal(SIGALRM,timeoutfunc);
   if(setjmp(timeoutenv)==0)
   {
      
      numprimes=0;
      primelist=(prime_t *)malloc(sizeof(prime_t)*max_numprimes);
      if(primelist==NULL)
      {
	 fprintf(stderr,"Couldn't allocate primelist\n");
	 exit(-1);
      }
      if(timeout>=0)
	 alarm(timeout);
      root_sqr=root*root;
      for(idx1=2;numprimes<max_numprimes&&idx1<MAX_PRIME;(idx1>=3 ? idx1+=2:idx1++))
      {
	 if(root_sqr<=idx1)
	 {
	    root++;
	    root_sqr=root*root;
	 }
	 for(idx2=2;idx2<=root;idx2++)
	 {
	    if((idx1%idx2)==0)
	       goto Skip;
	 }
	 //printf("%lu %lu\n",numprimes,idx1);
	 primelist[numprimes++]=idx1;
	 last_prime=idx1;
	Skip: 
      }
   }
   if(timeout>=0)
      alarm(0);
   if(numprimes<max_numprimes)
      primelist=(prime_t *)realloc(primelist,numprimes*sizeof(prime_t));
   printf("primelist made numprimes="UINT_FMT" largest prime="UINT_FMT"\n",numprimes,idx1); 
}





void factor(mpz_t test_composite)
{
   uint_t  divisor, q;
   uint_t  n_factors = 0;
   uint_t  primeidx=0;
   int     temp_composite_cmp;

   mpz_set(temp_composite,test_composite);
   for(primeidx=0,divisor=2;primeidx<numprimes&&
	  (temp_composite_cmp=mpz_cmp_ui(temp_composite,1))>0;)
  {
       mpz_tdiv_qr_ui (temp_quotent,temp_remainder, temp_composite,divisor);
       if(mpz_cmp_ui(temp_remainder,0)==0)
       {
	  mpz_set(temp_composite,temp_quotent);
	  continue;
       }
       divisor=primelist[primeidx++];
  }
}

void primes_found(char *str,mpz_t prime)
{
   mpz_tdiv_qr(temp_prime,temp_modulo,composite,prime);
   if(mpz_cmp_ui(temp_modulo,0)==0)
   {
      printf("primes found %s",str);
      mpz_out_str(stdout,10,prime);
      printf(" ");
      mpz_out_str(stdout,10,temp_prime);
      printf(".\n");
      exit(0);
   }
}

int main(int argc,char *argv[])
{
   uint_t comp_multiplier,factorsize,test_num_add,test_num_add1;
   mpz_t test_composite,sqrt_composite,n,composite_mod_n;
   if(argc!=2)
   {
      fprintf(stderr,"usage rsacrack number\n");
      return -1;
   }
   mpz_init_set_str(composite,argv[1],10);
   mpz_init(temp_composite);
   mpz_init(temp_quotent);
   mpz_init(temp_remainder);
   mpz_init(test_composite); 
   mpz_init(sqrt_composite);
   mpz_init(temp_prime);
   mpz_init(temp_modulo);
   mpz_init(n);
   mpz_init(composite_mod_n);
   makeprimelist(256,-1);
   mpz_sqrt(sqrt_composite,composite);
   for(mpz_set(n,sqrt_composite);mpz_cmp(n,composite)<0;mpz_add_ui(n,n,1))
   {
      primes_found("by brute force",n);
      mpz_mod(composite_mod_n,composite,n);
      factor(composite_mod_n);
      primes_found("by brute force",temp_composite);
   }

}  







