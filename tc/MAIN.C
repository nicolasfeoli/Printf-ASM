#include "main.h"
/*Si haciendo ligas le vuelve a salir el error de fixup overflow
  lo que tiene que hacer es poner -ml antes de los nombres de archivos cuando llama a tcc*/

/*
	Para compilar: $ tcc -ml MAIN.C H8PRINTF.ASM
*/


int main(int argc, char *argv[])
{
	const char* reves = "Numero al reves:";

	H8Printf("Tarea Ligas C-ASM. Lenguajes de Programacion. Grupo 2\n");
	H8Printf("Hecho por Nicolas Feoli. Carne 2016081332\n");
	H8Printf("Fecha: 19/10/17\n");
	if(argc == 1)
		H8Printf("Inserte un numero como parametro para que sea desplegado al reves. \n");
	else{
		ValorEspecial = pasarAEntero(argv[1]);
		H8Printf("\n...Dando vuelta al numero %d ...\n", ValorEspecial);
		voltearNumero();
		H8Printf("\nNumero Original: %d ; %s %d\n", pasarAEntero(argv[1]), reves, ValorEspecial);
	}
	
	return 0;
}

int pasarAEntero(char* tira)
{
	int final = 0, i = 0;
	while(tira[i])
		final = (final*10) + tira[i++]-0x30;
	return final;
}

void voltearNumero()
{
	int final = 0;
	while(ValorEspecial)
	{
	      final = (final*10) + ValorEspecial % 10;
	      ValorEspecial = ValorEspecial / 10;
	}
	ValorEspecial = final;
}