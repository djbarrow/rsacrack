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

typedef struct
{
      uint_t val;
      uint_t power; 
} factor_t;

typedef unsigned char primediff_t;
volatile uint_t numprimes;
primediff_t *primelist;
jmp_buf      timeoutenv;
 
void timeoutfunc(int signal)
{
   longjmp(timeoutenv,-1);
}

void makeprimelist(int kb,int timeout)
{
   volatile uint_t max_numprimes;
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
      max_numprimes=((1024*kb)/sizeof(primediff_t));
      primelist=(primediff_t *)malloc(sizeof(primediff_t)*max_numprimes);
      if(primelist==NULL)
      {
	 fprintf(stderr,"Couldn't allocate primelist\n");
	 exit(-1);
      }
      if(timeout>=0)
	 alarm(timeout);
      root_sqr=root*root;
      for(idx1=2;numprimes<max_numprimes;(idx1>=3 ? idx1+=2:idx1++))
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
   if(timeout>=0)
      alarm(0);
   if(numprimes<max_numprimes)
      primelist=(primediff_t *)realloc(primelist,numprimes*sizeof(primediff_t));
   printf("primelist made numprimes="UINT_FMT" largest prime="UINT_FMT"\n",numprimes,idx1); 
}




mpz_t composite,temp_composite,test_num;
mpz_t temp_quotent,temp_remainder;

/* returns 1 if factorisation is complete */
int factor(mpz_t test_composite,factor_t *factors,factor_t *end_factor)
{
   uint_t  divisor, q;
   uint_t  n_factors = 0;
   uint_t  primeidx=0;
   int     temp_composite_cmp;

   memset(factors,0,(u8 *)end_factor-(u8 *)factors);
   mpz_set(temp_composite,test_composite);
   /* temp_composite < 1 return 0 */ 
  if (mpz_cmp_ui(temp_composite,1)<0)
    return -1;
  for(primeidx=0,divisor=2;primeidx<numprimes&&
	 (temp_composite_cmp=mpz_cmp_ui(temp_composite,1))>0;)
  {
     
       if(factors>=end_factor)
	  return -1;
       mpz_tdiv_qr_ui (temp_quotent,temp_remainder, temp_composite,divisor);
#if 0
       mpz_tdiv_q_ui (temp_quotent,temp_composite,divisor);
       //mpz_tdiv_r_ui (temp_remainder,temp_composite,divisor);
       mpz_mod_ui (temp_remainder,temp_composite,divisor);
#endif
       
       if(mpz_cmp_ui(temp_remainder,0)==0)
       {
	  factors->power++;
	  factors->val=divisor;
	  mpz_set(temp_composite,temp_quotent);
	  continue;
       }
       if(factors->power)
	  factors++;
       divisor += primelist[primeidx++];
    }
  return(temp_composite_cmp<=0);
}


int main(int argc,char *argv[])
{
   uint_t comp_multiplier,factorsize,test_num_add,test_num_add1;
   factor_t *factors,*end_factor,*curr_factor;
   mpz_t test_composite,composite_multiple;

   if(argc!=2)
   {
      fprintf(stderr,"usage rsacrack number\n");
      return -1;
   }
   mpz_init_set_str(composite,argv[1],10);
   mpz_init(temp_composite);
   mpz_init(temp_quotent);
   mpz_init(temp_remainder);
   mpz_init(composite_multiple);
   mpz_init(test_composite);
   makeprimelist(1,-1);
   factorsize=strlen(argv[1])*4*sizeof(factor_t);
   factors=(factor_t *)malloc(factorsize);
   if(factors==NULL)
   {
      fprintf(stderr,"Couldn't allocate space for factors\n");
      exit(-1);
   }
   end_factor=(factor_t *)&((u8 *)factors)[factorsize];
   for(test_num_add1=2;test_num_add1<UINT_MAX_T;test_num_add1++)
   {
      for(comp_multiplier=1,mpz_set(composite_multiple,composite);comp_multiplier<test_num_add1;mpz_add(composite_multiple,composite_multiple,composite),comp_multiplier++)
      {
	 test_num_add=test_num_add1-comp_multiplier;
	 mpz_add_ui(test_composite,composite_multiple,test_num_add);
	 if(factor(test_composite,factors,end_factor))
	 {
	     mpz_out_str(stdout,10,test_composite);
	     printf(":");
	    for(curr_factor=factors;curr_factor->power!=0;curr_factor++)
	       printf(" "UINT_FMT"^"UINT_FMT,curr_factor->val,curr_factor->power);
	    printf("\n");
	 }
      }
   }
}  







