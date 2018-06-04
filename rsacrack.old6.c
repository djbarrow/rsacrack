#define TRUE  (1)
#define FALSE (0)
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#include <stdio.h>
#include <gmp.h>

#define START_MAX_CONGRUENCES   (16)
#define UINT_FORMAT "%lld"
typedef unsigned long long uintmax_t;

typedef struct
{
      mpz_t p1;
      mpz_t p2;
} cong;

typedef struct
{
      uintmax_t  max_congruences;
      uintmax_t  cong_cnt;
      cong       congruence_possibility[0]; 
} congruence_entry;


uintmax_t cong_idx,old_cong_idx,loopcnt;
congruence_entry *congruence_array[2];
mpz_t  p1,p2,test_modulo;
mpz_t  product,mod_product,temp_prime,temp_modulo,composite,composite_mod_idx;
mpz_t  remainder,test_modulo_mask;
mpz_t  composite_modulo,product_modulo;
congruence_entry *curr_congruence,*prev_congruence;


void primes_found(mpz_t prime)
{
   if(mpz_cmp_ui(prime,1)!=0)
   {
      mpz_tdiv_qr(temp_prime,temp_modulo,composite,prime);
      if(mpz_cmp_ui(temp_modulo,0)==0)
      {
	 printf("primes found ");
	 mpz_out_str(stdout,10,prime);
	 printf(" ");
	 mpz_out_str(stdout,10,temp_prime);
	 printf(".\n");
	 exit(0);
      }
   }
}

void print_primes(char * string,mpz_t number,int base)
{
   printf("%s",string);
   mpz_out_str(stdout,base,number);
   printf(" p1=");
   mpz_out_str(stdout,base,p1);
   printf(" p2=");
   mpz_out_str(stdout,base,p2);
   printf(" loopcnt="UINT_FORMAT".\n",loopcnt);
}


void add_congruence_possibility()
{
   uintmax_t i,new_max_congruences;
   cong      *temp_cong;
   
   loopcnt++;
   mpz_mul (product,p1,p2);
   primes_found(p1);
   primes_found(p2);
   mpz_tdiv_r(remainder,product,test_modulo);
   if((mpz_cmp(remainder,composite_mod_idx)==0))
   {
      print_primes("congruence possibility test_modulo=",test_modulo,10);
      mpz_sub_ui(test_modulo_mask,test_modulo,1);
      mpz_and(product_modulo,product,test_modulo_mask);
      mpz_and(composite_modulo,composite,test_modulo_mask);
      if(mpz_cmp(product_modulo,composite_modulo)==0)
      {
	 if(curr_congruence->cong_cnt>=(curr_congruence->max_congruences-1))
	 {
	    new_max_congruences=curr_congruence->max_congruences<<1;
	    curr_congruence=(congruence_entry *)realloc(curr_congruence,offsetof(congruence_entry,
				      congruence_possibility[new_max_congruences]));
	    if(!curr_congruence)
	    {
	       fprintf(stderr,"failed to realloc congruence\n");
	       exit(-1);
	    }
	    for(i=curr_congruence->max_congruences;i<new_max_congruences;i++)
	    {
	       temp_cong=&curr_congruence->congruence_possibility[i];
	       mpz_init(temp_cong->p1);
	       mpz_init(temp_cong->p2);
	    }
	    curr_congruence->max_congruences=new_max_congruences;
	    congruence_array[cong_idx]=curr_congruence;
	 }
	 temp_cong=&curr_congruence->congruence_possibility[curr_congruence->cong_cnt++];
	 mpz_set(temp_cong->p1,p1);
	 mpz_set(temp_cong->p2,p2);
	 print_primes("multiplicative match product modulo=",product_modulo,2);
      }
   }
}

int main(int argc,char *argv[])
{
  
   int firstloop;
   uintmax_t i,j;
   mpz_t     sqrt_composite,sqrt_composite_x2,prev_test_modulo;
   cong      *temp_cong;

   if(argc!=2)
   {
      fprintf(stderr,"usage rsacrack number\n");
      return -1;
   }
   loopcnt=0;
   mpz_init_set_str(composite,argv[1],10);
   mpz_init(sqrt_composite);
   mpz_init(p1);
   mpz_init(p2);
   mpz_init(product);
   mpz_init(remainder);
   mpz_init(test_modulo_mask);
   mpz_init(product_modulo);
   mpz_init(composite_modulo);
   mpz_init(temp_prime);
   mpz_init(temp_modulo);
   mpz_init(prev_test_modulo);
   mpz_init(sqrt_composite_x2);
   mpz_sqrt(sqrt_composite,composite);
   mpz_mul_2exp(sqrt_composite_x2,sqrt_composite,1);
   for(i=0;i<2;i++)
   {
      curr_congruence=(congruence_entry *)malloc(offsetof(congruence_entry,congruence_possibility[START_MAX_CONGRUENCES]));
      if(!curr_congruence)
      {
	 fprintf(stderr,"failed to allocate congruence_array\n");
	 return -1;
      }
      for(j=0;j<START_MAX_CONGRUENCES;j++)
      {
	  cong      *curr_cong_poss=&curr_congruence->congruence_possibility[j];
	  mpz_init(curr_cong_poss->p1);
	  mpz_init(curr_cong_poss->p2);
      }
      curr_congruence->max_congruences=START_MAX_CONGRUENCES;
      curr_congruence->cong_cnt=0;
      congruence_array[i]=curr_congruence;
   }
   firstloop=TRUE;
   for(cong_idx=0,mpz_init_set_ui(test_modulo,2);mpz_cmp(test_modulo,sqrt_composite_x2)<0;
       mpz_mul_2exp(test_modulo,test_modulo,1),cong_idx=((cong_idx+1)&1))
   {
      curr_congruence=congruence_array[cong_idx];
      mpz_mod(composite_mod_idx,composite,test_modulo);
      
      if(firstloop)
      {
	 for(mpz_set_ui(p1,1);mpz_cmp(p1,test_modulo)<0;mpz_add_ui(p1,p1,1))
	 {
	    for(mpz_set(p2,p1);mpz_cmp(p2,test_modulo)<0;mpz_add_ui(p2,p2,1))
	    {
		  add_congruence_possibility();
	    }
	 }
	 firstloop=FALSE; 
      }
      else
      {
	 for(i=0;i<prev_congruence->cong_cnt;i++)
	 {
	    temp_cong=&prev_congruence->congruence_possibility[i];
	    mpz_set(p1,temp_cong->p1);
	    mpz_set(p2,temp_cong->p2);
	    add_congruence_possibility();
	    mpz_add(p2,p2,prev_test_modulo);
	    add_congruence_possibility();
	    mpz_add(p1,p1,prev_test_modulo);
	    add_congruence_possibility();
	    mpz_set(p2,temp_cong->p2);
	    add_congruence_possibility();
	 }
      }
      prev_congruence=curr_congruence;
      mpz_set(prev_test_modulo,test_modulo);
   }
   printf("no prime factors found loopcnt="UINT_FORMAT".\n",loopcnt);
   return(-1);
}









