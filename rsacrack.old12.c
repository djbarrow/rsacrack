#define TRUE  (1)
#define FALSE (0)
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#include <stdio.h>
#include <gmp.h>

#define START_MAX_CONGRUENCES   (16)
#define UINT_FORMAT "%lld"
#define MODULO_T_FORMAT "%lu"

typedef unsigned long long uintmax_t;
typedef unsigned long modulo_t;

typedef struct
{
      modulo_t p1;
      modulo_t p2;
} cong;

typedef struct
{
      uintmax_t  max_congruences;
      uintmax_t  cong_cnt;
      cong       congruence_possibility[0]; 
} congruence_entry;


mpz_t dummy;

modulo_t euclid_calc_gcd(modulo_t x,modulo_t y)
{
   modulo_t temp;

   for(;;)
   {
      if(x==y)
	 return x;
      if(x<y)
      {
	 temp=(y%x);
	 if(temp==0)
	    return(x);
	 else
	    y=temp;
      }
      else
      {
	 temp=(x%y);
	 if(temp==0)
	    return(y);
	 else
	    x=temp;
      }
   }
}




int main(int argc,char *argv[])
{

   modulo_t n,n2,k1,k2,composite_mod_n,gcd_k1k2,gcd_nk1k2;
   modulo_t root=1,root_sqr=root*root,max_n;
   mpz_t composite,sqrt_composite,temp;
   modulo_t loop_cnt,eliminate_cnt;

   if(argc!=2)
   {
      fprintf(stderr,"usage rsacrack number\n");
      return -1;
   }
   mpz_init_set_str(composite,argv[1],10);
   mpz_init(dummy);
   mpz_init(sqrt_composite);
   mpz_init(temp);
   mpz_sqrt(sqrt_composite,composite);
   if(mpz_cmp_ui(sqrt_composite,0xfffff000)<=0)
      max_n=mpz_get_ui(sqrt_composite);
   else
      max_n=0xfffff000;
#if 0
   for(n=2;n<max_n;n++)
   {
#else
   for(n=2;n<max_n;(n>=3 ? n+=2:n++))
   {
      if(root_sqr<=n)
      {
	 root++;
	 root_sqr=root*root;
      }
      
      for(n2=2;n2<=root;n2++)
      {
	 if(n!=2&&(n%n2)==0)
	    goto Skip;
      }
#endif
     composite_mod_n=mpz_mod_ui(dummy,composite,n);
     loop_cnt=0;
     eliminate_cnt=0;
     for(k1=1;k1<n;k1++)
     {
	 for(k2=k1;k2<n;k2++)
	 {
	    /* n*i1+k1 */
	    /* n*i2+k2 */
	    /*======== */
	    /* n(n*i1*i2 + i2*k1 + i1*k2) + k1*k2 == composite */
	    /* => (composite-k1*k2)/n = n*i1*i2 + i2*k1 + i1*k2 */
	    /* => (composite-k1*k2)/n % gcd(n,k1,k2) = 0 for non prime n */
	    /* so we should be able to eliminate some values */
	    if(((k1*k2)%n)==composite_mod_n)
	    {
#if 0
	       gcd_k1k2=euclid_calc_gcd(k1,k2);
	       if(gcd_k1k2!=1)
	       {
		  gcd_nk1k2=euclid_calc_gcd(gcd_k1k2,n);
		  if(gcd_nk1k2!=1)
		  {
		     mpz_set_ui(temp,k1);
		     mpz_mul_ui(temp,temp,k2);
		     mpz_sub(temp,composite,temp);
		     mpz_div_ui(temp,temp,n);
		     if(mpz_mod_ui(dummy,temp,gcd_nk1k2)!=0)
			eliminate_cnt++;
		  }
	       }
#endif
	       loop_cnt++;
	       printf("possibility found n="MODULO_T_FORMAT" c_mod_n="MODULO_T_FORMAT
		      " k1="MODULO_T_FORMAT" k2="MODULO_T_FORMAT"\n",n,composite_mod_n,
		      k1,k2);
	      
	       
	     }
         }
     	
     }
     //printf(" n="MODULO_T_FORMAT" loop_cnt="MODULO_T_FORMAT" eliminate_cnt="MODULO_T_FORMAT"\n",n,loop_cnt,eliminate_cnt);
     Skip:
   }

}









