/*#pragma inline*/
/*extern int archivo(void);*/
/*extern int existeNivel(void);
extern void rayos(void);*/
extern void imprimirTablero(void);

void actualizarPantalla()
{
	int i, j;
	/*contarDolmens(tablero);*/
	Test(9);/*blancos*/
	/*test(5); /*rojos*/
	for(i = 8; i >= 0; i--)
	{
		for(j = 13; j >= 0; j--)
		{
			/*test(0);*/
			break;
		}
	}
	imprimirTablero(); /*invoca a la rutina de asm que pinta el tablero*/
	return;
}



int main(){
	int c;
	/*test(1);*/
	/*c=1;*/
	/*rayos();*/
	/*printf("12");*/
	/*printf("|%d|",c);*/
	actualizarPantalla();
	
	
	
	return 0;
}
