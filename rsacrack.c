#define TRUE  (1)
#define FALSE (0)
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#include <stdio.h>
#include <gmp.h>




int mpz_count_bits(mpz_t num)
{
   mp_limb_t last_limb=num->_mp_d[((num->_mp_size)-1)];
   int num_bits=((num->_mp_size)-1)*(sizeof(mp_limb_t)<<3);
   while(last_limb!=0)
   {
      last_limb>>=1;
      num_bits++;
   }
   return(num_bits);
}

void mpz_lshift(mpz_t rop,mpz_t op,unsigned long cnt)
{
   mpn_lshift(rop->_mp_d, op->mp_d,op->_mp_size,cnt);
}

void mpz_rshift(mpz_t rop,mpz_t op,unsigned long cnt)
{
   mpn_rshift(rop->_mp_d, op->mp_d,op->_mp_size,cnt);
}



int main(int argc,char *argv[])
{
   mpz_t composite,divisor,shifted_divisor,remainder,result,tmp;
   int num_composite_bits,num_divisor_bits,divisor_shift,divisor_shift2,cmp;

   if(argc!=2)
      goto error;
   mpz_init_set_str(composite,argv[1],10);
   if(mpz_cmp_si(composite,0)<=0)
      goto error;
   mpz_init(remainder);
   mpz_init(divisor);
   mpz_init(shifted_divisor);
   mpz_init(tmp);
   mpz_init(result);
   mpz_set_ui(result,0);
   num_composite_bits=mpz_count_bits(composite);
   mpz_init_set_ui(divisor,2);
   num_divisor_bits=2;
   mpz_set(remainder,composite);
   divisor_shift=num_composite_bits-num_divisor_bits;
   mpz_lshift(shifted_divisor,divisor,divisor_shift);

   /*long division loop */
   for(divisor_shift2=divisor_shift;divisor_shift2>=0;divisor_shift2--)
   {
      mpz_sub(tmp,remainder,shifted_divisor);
      cmp=mpz_cmp_ui(tmp,0);
      if(cmp==0)
      {
	 printf("perfect divisor found=");
	 mpz_out_str(stdout,10,divisor);
	 printf("\n");
      }
      if(cmp>0)
      {
	 mpz_set_bit(result,divisor_shift2);
	 mpz_set(remainder,tmp);
      }
      if(divisor_shift-divisor_shift2==num_divisor_bits)
      mpz_rshift(divisor,divisor,1);
   }
   printf("result ");
   mpz_out_str(stdout,10,result);
   printf("\n");
   return 0;

  error:
   fprintf(stderr,"usage rsacrack positive number\n");
   return -1;
}









