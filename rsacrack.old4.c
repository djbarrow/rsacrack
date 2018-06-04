#include <stdio.h>
#include <math.h>

typedef long long uintmax_t;
typedef double float_t; 


uintmax_t euclid_calc_gcd(uintmax_t x,uintmax_t y)
{
   uintmax_t temp;

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
   
   uintmax_t a,b,c,x,y,gcd,y_p2,y_p2_modc,y_minus_x,maxy_mod_p2,loopcnt,gcdcnt;
   uintmax_t sqrt_c,start_y,end_y,diff_yxp2,n;

   if(argc!=3)
   {
      fprintf(stderr,"usage rsacrack p1 p2");
      return -1;
   }
   a=atoll(argv[1]);
   b=atoll(argv[2]);
  
   c=a*b;
   sqrt_c=sqrt(c);
   gcdcnt=0;
   loopcnt=0;
   for(n=1;n<20;n++)
   {
      start_y=sqrt_c+1;
      end_y=(n+1)*sqrt_c;
      for(y=start_y;y<end_y;y++)
      {
	 loopcnt++;
	 y_p2=y*y;
	 y_p2_modc=y_p2%c;
	 x=sqrt(y_p2_modc);
	 diff_yxp2=y_p2_modc-(x*x);
	 printf("%lld %lld %lld %lld %lld\n",loopcnt,diff_yxp2,x,y,c);
	 if(diff_yxp2==0)
	 {
	    y_minus_x=y-x;
	    if((y_minus_x%c)!=0)
	    {
	       gcd=euclid_calc_gcd(c,y_minus_x);
	       gcdcnt++;
	       if(gcd!=1&&gcd!=c)
	       {
		  if((c%gcd)==0)
		  {
		     printf("primes found %lld %lld loopcnt=%lld gcdcnt=%lld\n",gcd,c/gcd,loopcnt,gcdcnt);
		     return 0;
		  }
	       }
	    }
	 }
      }
   }
   printf("No primes found loopcnt=%lld gcdcnt=%lld\n",loopcnt,gcdcnt);
   return -1;
}









