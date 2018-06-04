#define TRUE  (1)
#define FALSE (0)
#include <stdio.h>
#include <gmp.h>


int main(int argc,char *argv[])
{
   mpz_t composite,n,test_yp2,test_y,rem_y,test_xp2;

   if(argc!=2)
   {
      fprintf(stderr,"usage rsacrack number\n");
      return -1;
   }
   mpz_init_set_str(composite,argv[1],10);
   mpz_init(n);
   mpz_init(test_y);
   mpz_init(rem_y);
   mpz_init(test_xp2);
   mpz_init(test_yp2);
   for(mpz_init_set_ui(n,1);;mpz_add_ui(n,n,1))
   {
      mpz_mul(test_yp2,composite,n);
      mpz_sqrtrem(test_y,rem_y,test_yp2);
      if(mpz_cmp_ui(rem_y,0)!=0)
	 mpz_add_ui(test_y,test_y,1);
       mpz_mul(test_yp2,test_y,test_y);
       mpz_mod(test_xp2,test_yp2,composite);
       if(mpz_perfect_square_p (test_xp2))
       {
	  printf("Found perfect square ");
	  mpz_out_str(stdout,10,test_xp2);
	  printf("\n");
	  return(0);
       }
       if(test_xp2->_mp_size<=2)
       {
	  printf("Found small xp2");
	  mpz_out_str(stdout,10,test_xp2);
	  printf("\n");
       }
   }
}  







