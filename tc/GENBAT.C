
#include <stdio.h>;
#include <string.h>;

void main(int argc,char *argv[])
{ char c,linea[80]="ren \"                              ";
  int i=5,j=0,k=8,l=0;

   while ((-1!=(c=getchar()))&& (c!='*'))
   {
      if (argc-1) k=8+strlen(argv[1]);
      else k=8;
      linea[i++]=c;
      while((c!='\n')&&(c!=-1))
	  linea[i++]=c=getchar();
      --i;
      linea[i++]='"';
      linea[i++]=' ';
      l=0;
      if (argc-1) { while (linea[i++]=argv[1][l++]); };
      i--;
      k=i-k;
      while(linea[k]!=' ') k--;
      k++;
      if (linea[k+1]=='.')
	linea[i++]='0';
      else
	linea[i++]=linea[k++];
      for (j=k;linea[j]!='"';linea[i++]=linea[j++]);
     linea[i++]='\n';
     linea[i]=0;
     printf(linea);
     i=5;
   }


}