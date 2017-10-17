
#include <stdio.H>


void mult(int X, int Y, int *R)
/* Procedimiento que multiplica dos números positivos X por Y
 retorna el resultado en R */
{  int aux,aux2;

   if (X==1) aux2 = Y ;
   else if (X==0) aux2 = 0;
   else
       { mult(X-1,Y,&aux);
	 aux2 = aux + Y;
         }
   *R = aux2;
}

main()
{
int X;

  mult(12,12,&X);
  printf("el valor de X es %d",X);

}