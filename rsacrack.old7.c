q#define TRUE  (1)
#define FALSE (0)
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#include <stdio.h>
#include <gmp.h>
#include <signal.h>
#include <setjmp.h>

typedef unsigned char primediff_t;
uintmax_t numprimes=0;
primediff_t *primelist;
jmpbuf      timeoutenv;
 
void *timeout(int signal)
{
   longjmp(timeoutenv,-1);
}

void makeprimelist(int kb,int timeout)
{
   volatile uintmax_t max_numprimes,numprimes=0;
   uintmax_t last_prime=0,max_delta=0,curr_delta;
   uintmax_t hi_int,idx1;
   uintmax_t idx2,numprimes=0;
   uintmax_t root=1,root_sqr;
   
   signal(SIGALRM,mytimeout);
   if(setjmp(timeoutenv)==0)
   {
      
      numprimes=0;
      maxnumprimes=((1024*kb)/sizeof(primediff_t));
      primelist=malloc(sizeof(primediff_t)*maxnumprimes);
      if(primelist==NULL)
      {
	 fprintf(stderr,"Couldn't allocate primelist\n");
	 exit(-1);
      }
      alarm(timeout);
      root_sqr=root*root;
      for(idx1=1;numprimes<max_numprimes;(idx1>=3 ? idx1+=2:idx1++))
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
	 curr_delta=idx1-last_prime;
	 if(curr_delta>(1<<(sizeof(primediff_t)*8)))
	    break;
	 primelist[numprimes++]=curr_delta;
	 last_prime=idx1;
	Skip: 
      }
   }
   alarm(0);
   if(numprimes<max_numprimes)
      primelist=realloc(primelist,numprimes*sizeof(primediff_t));
}

typedef unsigned long uint_t;
#define UINT_MAX_T ((uint_t)-1)
typedef unisigned char u8; 


typedef struct
{
   uint_t power;
   uint_t val;
} factor_t;



mpz_t composite,temp_composite,test_num;
mpz_t temp_quotent,temp_remainder;

static int
factor (mpz_t test_composite,factor_t *factor,factor_t *end_factor)
{
   uint_t  divisor, q;
   uint_t n_factors = 0;

   memset(factors,0,(u8 *)end_factor-(u8 *)factor);
   mpz_set(temp_composite,composite);
   /* temp_composite < 1 return 0 */ 
  if (mpz_cmp_ui(temp_composite,1)<0)
    return 0;

  /* temp_composite % 2 == 0 */
  mpz_tdiv_qr_ui (temp_quotent,temp_remainder, temp_composite,2);
  factor->val=2;
  for(;;)
  {
     if(factor>=end_factor)
	return -1;
     mpz_tdiv_qr_ui (temp_quotent,temp_remainder, temp_composite,2);
     /* temp_composite % 2 == 0 */ 
     if(mpz_cmp_ui(temp_remainder,0)==0)
     {
	factor->power++;
	mpz_set(temp_composite,temp_quotent);
	continue;
     }
     if(factor->power)
	factor++;
     break;
  }

  /* The exit condition in the following loop is correct because
     any time it is tested one of these 3 conditions holds:
      (1) d divides n
      (2) n is prime
      (3) n is composite but has no factors less than d.
     If (1) or (2) obviously the right thing happens.
     If (3), then since n is composite it is >= d^2. */

  divisor = 3;
  factors_val=0;
  do
    {
       if(factor>=end_factor)
	  return -1;
       mpz_tdiv_qr_ui (temp_quotent,temp_remainder, temp_composite,divisor);
       if(mpz_cmp_ui(temp_remainder,0)==0)
       {
	  factor->power++;
	  factor->val=divisor;
	  mpz_set(temp_composite,temp_quotent);
	  continue;
       }
       factor++;
       divisor += 2;
       if(divisor>max_divisor)
	  return -2;
    }
  while (mpz_cmp_ui(temp_composite,1)<=0);
  return(0);
}


int main(int argc,char *argv[])
{
   uint_max_t comp_mult;
   if(argc!=2)
   {
      fprintf(stderr,"usage rsacrack number\n");
      return -1;
   }
   loopcnt=0;
   mpz_init_set_str(composite,argv[1],10);
   mpz_init(temp_composite);
   mpz_init(temp_quotent);
   mpz_init(temp_remainder);
   mpz_init(composite_mult);
   for(test_num_add1=2;test_num_add1<UINT_MAX_T;test_num_add1++)
   {
      for(comp_multiplier=1,mpz_set(composite_multiple,composite);comp_mult<test_num_add;mpz_add(composite_multiple,composite_multiple,composite),comp_multiplier++)
      {
	 test_num_add=test_num_add1-comp_multiplier;
	 mpz_add_ui(test_composite,composite_multiple,test_num_add);
	 
      }
   }
}  
