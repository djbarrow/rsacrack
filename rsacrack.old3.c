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

#define MAX_XY 10

int main(int argc,char *argv[])
{
   
   uintmax_t a,b,c,n,x1,x,y,i,j,gcd,y_minus_x;
   uintmax_t poss_xy[MAX_XY];


   if(argc!=3)
   {
      fprintf(stderr,"usage rsacrack p1 p2");
      return -1;
   }
   a=atoll(argv[1]);
   b=atoll(argv[2]);
   c=a*b;
   for(x=0;x<1000000;x++)
   {
      if((x*x)%c==1)
	 printf("%lld\n",x);
   }
#if 0
   for(x1=1;x1<=1;x1++)
   {
      for(i=0;i<MAX_XY;i++)
      {
	 y=poss_xy[i]=sqrt((i*i*c*c)+(2*i*x1*c)+(x1*x1));
	 printf("poss_xy %lld\n",y);
	 if(i>0)
	 {
	    for(j=0;j<i;j++)
	    {
	       x=poss_xy[j];
	       y_minus_x=y-x;
	       if((y_minus_x%c)!=0)
	       {
		  gcd=euclid_calc_gcd(c,y_minus_x);
		  if(gcd!=1&&gcd!=c)
		  {
		     if((c%gcd)==0)
			printf("primes found %lld %lld",gcd,c/gcd);
		  }
	       }
	    }
	 }
      }
   }
   printf("No primes found\n");
#endif
}









