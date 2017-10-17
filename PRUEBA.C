#include "STDIO.H"

main()
{
  char* hola = "HOLA";
  int* entero;
  printf("TAMANO DE INT: %d\n",sizeof(int));
  printf("TAMANO DE CHAR: %d\n",sizeof(char));
  printf("TAMANO DE CHAR*: %d\n",sizeof(char*));
  printf("TAMANO DE HOLA: %d\n",sizeof(hola));
  printf("TAMANO DE ENTERO*: %d\n",sizeof(entero));
}