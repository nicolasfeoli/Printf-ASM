/*
    /\  ____
    <> ( oo )
    <>_| ^^ |_
    <>   @    \
   /~~\ . . _ |
  /~~~~\    | |
 /~~~~~~\/ _| |
 |[][][]/ / [m]
 |[][][[m]
 |[][][]|
 |[][][]|
 |[][][]|
 |[|--|]|
 |[|  |]|
 ========
==========
|[[    ]]|
==========
*/
/*#include <iostream>
using namespace std;*/
#include "stdlib.h"
#include "stdio.h"
#include "ctype.h"
#include "time.h"
#include "math.h"
typedef int bool;
#define true 1
#define false 0
struct Movida;
struct Pieza;
struct Tablero;
void setTablero(struct Tablero *tablero);
struct Pieza* newPieza(int x,int y);
long horizontal(struct Tablero*tablero,int i, int j, int color,int colo);


struct Pieza{
	bool chequeada;		/*cuando se buscan los dolmens, para saber si ya se conto*/
	bool jugando;		/*si esta dentro del tablero de juego*/
	int letra,numero;		/*posiciones en el tablero*/
	bool pieza;			/*si tiene unapieza encima, las siguientes caracteristicas se les da valor si esta es true*/
	int color;			/*0=vacio, negativo=blanco, positivo=rojo*/
	bool vertical;		/*indica si la pieza esta envertical en ese lugar*/
	int colorNorte;		/*si la pieza esta en vertical se indica el color del norte, si no esta en vertical se pone un 0*/
	bool movible;		/*indica si esta pieza se puede mover(si acaba de ser puesta), despues se revisara que la jugada sea dentro del tablero*/
};
/*el tablero*/
struct Tablero{
	int dolmenRojo;
	int dolmenBlanco;
	struct Pieza *piezas[9][14];
	int vectorRojo[14];
	int vectorBlanco[14];
	int valorBlanco;			/*determina el valor del tablero para el color rojo y el blanco*/
	int valorRojo;
	int numRojo;
	int numBlanco;
};

struct Movida{
	int x;
	int y;
	int color;
	int colorN;
	
};


/*constructor de las piezas*/
struct Pieza* newPieza(int x,int y) { 
  struct Pieza* p = malloc(sizeof(struct Pieza));
  p->jugando = true;
  p->pieza=  false;
  p->color=0;
  p->colorNorte=0;
  p->letra=x;
  p->numero=y;
  p->chequeada=false;
  return p;
}



/*deep copy de una estructura a otra*/
void deepCopy(struct Tablero*tablero,struct Tablero*tablero2){
	int i,j;
	/*struct Tablero *tablero2 = malloc(sizeof(struct Tablero));*/
	tablero2->dolmenBlanco=tablero->dolmenBlanco;
	tablero2->dolmenRojo=tablero->dolmenRojo;
	tablero2->valorRojo=tablero->valorRojo;
	tablero2->valorBlanco=tablero->valorBlanco;
	i=j=0;
	while(i<9){
		j=0;
		while(j<14){
			tablero2->piezas[i][j]->color=tablero->piezas[i][j]->color;
			tablero2->piezas[i][j]->chequeada=tablero->piezas[i][j]->chequeada;
			tablero2->piezas[i][j]->letra=tablero->piezas[i][j]->letra;
			tablero2->piezas[i][j]->numero=tablero->piezas[i][j]->numero;
			tablero2->piezas[i][j]->pieza=tablero->piezas[i][j]->pieza;
			tablero2->piezas[i][j]->vertical=tablero->piezas[i][j]->vertical;
			tablero2->piezas[i][j]->colorNorte=tablero->piezas[i][j]->colorNorte;
			tablero2->piezas[i][j]->movible=tablero->piezas[i][j]->movible;
			j++;
		}
		i++;
	}
}
/*constructor del tablero*/
void setTablero(struct Tablero *tablero){
	/*struct Pieza* newPieza(int x,int y);*/
	int j=0;
	int i =0;
	while(i<9){
		j=0;
		while(j<14){
			tablero->piezas[i][j]=newPieza(i,j);
			j++;
		}
		i++;
	}
	
	
}

/*libera la memoria del tablero*/
void freeTablero(struct Tablero *tablero){
	int j=0;
	int i =0;
	while(i<9){
		j=0;
		while(j<14){
			free(tablero->piezas[i][j]);
			j++;
		}
		i++;
	}
	
	
}


/*guarda los datos de una jugada posible de la ia*/
struct Jugada{
	int x,y,color,norte;
	long delta;		/*delta es la ganancia de la jugada*/
};

/*crea una jugada*/
struct Jugada*newJugada(){
	struct Jugada* p = malloc(sizeof(struct Jugada));
	p->color=0;
	p->norte=0;
	p->x=0;
	p->y=0;
	p->delta=0L;
	return p;
}
/*le pone los datos a una jugada*/
void datosJugada(struct Jugada*p,int x, int y, int color, int norte, long delta ){

	p->color=color;
	p->norte=norte;
	p->x=x;
	p->y=y;
	p->delta=delta;
}
/* guarda las jugadas*/
struct Registro{
	int largo;		/*cuantas jugadas tiene*/
	struct Jugada*jugadas[255];
	
};
/*inicia el registro*/
void setRegistro(struct Registro*registro){
	int j=0;
	registro->largo=0;
	printf("RRR");
	/*
	while(j<252){
		printf("1");
		registro->jugadas[j]=newJugada();
		j++;
	}
	*/
	printf("RRR");
	
}


/*imprime datos del tablero*/
void imprimirTablero(struct Tablero*tablero){
		int k,m,j;
		k=j=0;
		while(k<9){
			m=0;
			while(m<14){
				if(tablero->piezas[k][m]->jugando==false){
					printf("| X |");
				}
				else{
					if(tablero->piezas[k][m]->color>=0){
						printf("| %d |",tablero->piezas[k][m]->color);/*,tablero->piezas[k][m]->numero);*/
					}
					else{
						printf("|%d |",tablero->piezas[k][m]->color);/*,tablero->piezas[k][m]->numero);*/
					}
				}
				m++;
				j++;
			}
			printf("\n");
			k++;
		}
	}

/*pone las piezas del tablero activas de acuerdo con el tamano del tablero  en el que se jugara*/
void setTipoTablero(struct Tablero*tablero,int tamano){
	if(tamano==3){			/*el mas grande, siberia*/
		/*no hace nada, el tablero esta todo activo*/
		
	}
	else if (tamano==2){	/*alemania*/
	
		int i,j;
		i=0;
		while(i<9){
			j=0;
			while(j<14){
				if(i<1||i>7||j<2||j>11)
					tablero->piezas[i][j]->jugando=false;		/*desactiva los bordes*/
				
				j++;
			}
			i++;
		}	
	}
	else{			/*el vaticano*/
	
		int i,j;
		i=0;
		while(i<9){
			j=0;
			while(j<14){
				if(i<2||i>6||j<3||j>10)
					tablero->piezas[i][j]->jugando=false;		/*desactiva los bordes*/
				
				j++;
			}
			i++;
		}
		
	}
}
/*hace fill de las cuadriculas del mismo color*/
void fillTablero(struct Tablero*tablero,int i,int j,int color,int *ret){
	int aux,aux2;
	if (tablero->piezas[i][j]->chequeada==false&&tablero->piezas[i][j]->color==color&&tablero->piezas[i][j]->jugando==true){
		aux=1;
		tablero->piezas[i][j]->chequeada=true;
		if(i>0)
			fillTablero(tablero,i-1,j,color,&aux);
		if(i<8)
			fillTablero(tablero,i+1,j,color,&aux);
		if(j<13)
			fillTablero(tablero,i,j+1,color,&aux);
		if(j>0)
			fillTablero(tablero,i,j-1,color,&aux);
		aux2=aux;
	}
	else{
		aux2=0;
	}
	aux2=aux2+*ret;
	(*ret)=aux2;
}

void valorTablero(struct Tablero*tablero){		/*determina el valor del tablero para cada color*/
	int rojo,blanco;
	int cont;
	rojo=0;
	blanco=0;
	cont=0;
	/*determina el valor del rojo y el blanco, el valor es el numero de dolmens mas un peso por el tama;o de ellos*/
	while(cont<14){
		if(tablero->vectorRojo[cont]<=0&&tablero->vectorBlanco[cont]<=0){
			
		}
		else if(tablero->vectorRojo[cont]==tablero->vectorBlanco[cont]){
			rojo+=pow(2,14-cont);
			blanco+=pow(2,14-cont);
		}
		else if (tablero->vectorRojo[cont]>tablero->vectorBlanco[cont]){
			rojo+=pow(2,14-cont);
		}
		else{
			blanco+=pow(2,14-cont);
		}
		cont++;
	}
	tablero->valorBlanco=blanco;
	tablero->valorRojo=rojo;		/*guarda los valores delpeo del tablero para cada archivo*/
}

void contarDolmens(struct Tablero*tablero){
	int i,j;
	int*k;
	int storage=0;
	int l=0;
	int top,aux;		/*usadas para indexar el vector con los dolmens mas grandes y como auxiliar*/
	k=&storage;
	i=0;
	tablero->dolmenBlanco=0;
	tablero->dolmenRojo=0;
	
	while(i<9){
		j=0;
		while(j<14){
			tablero->piezas[i][j]->chequeada=false;
			if(l<14){
				tablero->vectorBlanco[l]=0;		/*limpia losvectores de los top dolments*/
				tablero->vectorRojo[l]=0;
				l++;
			}
			j++;
		}
		i++;
	}
	tablero->numBlanco=0;
	tablero->numRojo=0;
	i=0;
	while(i<9){
		j=0;
		while(j<14){
			if(tablero->piezas[i][j]->color!=0&&tablero->piezas[i][j]->chequeada==false&&tablero->piezas[i][j]->jugando){
				if(tablero->piezas[i][j]->color==1){
					tablero->numBlanco++;
				}
				else{
					tablero->numRojo++;
				}
				storage=0;
				fillTablero(tablero,i,j,tablero->piezas[i][j]->color,k);
				if(storage>2){
					if(tablero->piezas[i][j]->color==1){
						top=0;
						tablero->dolmenBlanco++;
						
						while(top<14){
							if(tablero->vectorBlanco[top]<storage){		/*pone el vector con los valores mas altos de los dolmens, el tamano*/
								aux=tablero->vectorBlanco[top];
								tablero->vectorBlanco[top]=storage;
								storage=aux;
								
							}
							top++;
						}
						
					}
					else{
						tablero->dolmenRojo++;
						top=0;
						
						while(top<14){			/*pone el vector con los valores mas altos de los dolmens, el tamano*/
							if(tablero->vectorRojo[top]<storage){
								aux=tablero->vectorRojo[top];
								tablero->vectorRojo[top]=storage;
								storage=aux;
								
							}
							top++;
						}
						
					}
				}
			}
			j++;
		}
		i++;
	}
	valorTablero(tablero);
}

long getDelta(struct Tablero*tablero,int color){		/*obtiene el delta de una jugada*/
	long deltaR,deltaB,r;
	contarDolmens(tablero);
	deltaR=(long)tablero->dolmenRojo*(long)32768;
	deltaB=(long)tablero->dolmenBlanco*(long)32768;
	/*deltaR+=(long)tablero->valorRojo;
	deltaR+=(long)tablero->valorBlanco;*/
	
	if(color==1){
		r= deltaB-deltaR;
	}
	else{
		r= deltaR-deltaB;
	}
	return r;
}
/*funcion auxiliar para llamar a el contadora de dolmens limpiando antes las banderas de chequeo*/
void fillAux(struct Tablero*tablero,int x,int y,int color,int *ret){
	int i,j;
	i=0;
	while(i<9){
		j=0;
		while(j<14){
			tablero->piezas[i][j]->chequeada=false;
			j++;
		}
		i++;
	}
	fillTablero(tablero,x,y,color,ret);
}

/*long vistaFutura(struct Tablero*tablero,)*/
void MustafaB(struct Tablero*tablero,int i,int j,int colo){
	long deltaT,temp;
	int norte,este;			/*colores para cada lado respectivo*/
	int dir;		/*direccion a donde se bota, 1arriba, 2izquierda,3abajo,4derecha, 0 no se mueve*/
	int salvado;		/*se salva el color de una pieza*/
	deltaT=getDelta(tablero,colo);		/*el valor inicial es suponer que no se bota*/
	deltaT-=1L;
	dir=0;
	norte=tablero->piezas[i][j]->colorNorte;
	este=norte*-1;
	if(i>2&&tablero->piezas[i-1][j]->jugando&&tablero->piezas[i-2][j]->jugando){
		if(tablero->piezas[i-1][j]->color==0&&tablero->piezas[i-2][j]->color==0){
			tablero->piezas[i-2][j]->color=norte;
			tablero->piezas[i-1][j]->color=norte;
			salvado=tablero->piezas[i][j]->color;
			tablero->piezas[i][j]->color=0;
			temp=getDelta(tablero,colo);
			if(colo!=norte){		/*leve desventaja para moverlo a lados que no son del mismo color*/
				temp-=100L;
			}
			if(temp>=deltaT){
				deltaT=temp;
				dir=1;
			}
			tablero->piezas[i][j]->color=salvado;
			tablero->piezas[i-2][j]->color=0;
			tablero->piezas[i-1][j]->color=0;
		}
	}
	if(i<7&&tablero->piezas[i+1][j]->jugando&&tablero->piezas[i+2][j]->jugando){
		if(tablero->piezas[i+1][j]->color==0&&tablero->piezas[i+2][j]->color==0){
			tablero->piezas[i+2][j]->color=norte;
			tablero->piezas[i+1][j]->color=norte;
			salvado=tablero->piezas[i][j]->color;
			tablero->piezas[i][j]->color=0;
			temp=getDelta(tablero,colo);
			if(colo!=norte){		/*leve desventaja para moverlo a lados que no son del mismo color*/
				temp-=100L;
			}
			if(temp>=deltaT){
				deltaT=temp;
				dir=3;
			}
			tablero->piezas[i][j]->color=salvado;
			tablero->piezas[i+2][j]->color=0;
			tablero->piezas[i+1][j]->color=0;
		}
	}
	if(j>2&&tablero->piezas[i][j-1]->jugando&&tablero->piezas[i][j-2]->jugando){
		if(tablero->piezas[i][j-1]->color==0&&tablero->piezas[i][j-2]->color==0){
			tablero->piezas[i][j-1]->color=este;
			tablero->piezas[i][j-2]->color=este;
			salvado=tablero->piezas[i][j]->color;
			tablero->piezas[i][j]->color=0;
			temp=getDelta(tablero,colo);
			if(colo!=este){		/*leve desventaja para moverlo a lados que no son del mismo color*/
				temp-=100L;
			}
			if(temp>=deltaT){
				deltaT=temp;
				dir=2;
			}
			tablero->piezas[i][j]->color=salvado;
			tablero->piezas[i][j-2]->color=0;
			tablero->piezas[i][j-1]->color=0;
		}
	}
	if(j<11&&tablero->piezas[i][j+1]->jugando&&tablero->piezas[i][j+2]->jugando){
		if(tablero->piezas[i][j+1]->color==0&&tablero->piezas[i][j+2]->color==0){
			tablero->piezas[i][j+1]->color=este;
			tablero->piezas[i][j+2]->color=este;
			salvado=tablero->piezas[i][j]->color;
			tablero->piezas[i][j]->color=0;
			temp=getDelta(tablero,colo);
			if(colo!=este){		/*leve desventaja para moverlo a lados que no son del mismo color*/
				temp-=100L;
			}
			if(temp>=deltaT){
				deltaT=temp;
				dir=4;
			}
			tablero->piezas[i][j]->color=salvado;
			tablero->piezas[i][j+2]->color=0;
			tablero->piezas[i][j+1]->color=0;
		}
	}
	printf("IABOTA: %d\n",dir);
}
	
	


void Mustafa(struct Tablero*tablero,int color,struct Movida*movida){		/*color es para que color juega*/
	int xM[255];
	int yM[255];
	int colorM[255];
	int norteM[255];		/*donde se guarda informacion de las posibles jugadas*/ 
	long extra;			/*una bonificacion que se le da a las jugadas si se cumplen ciertas condiciones*/
	int largoM;		/*el indice de los vectores*/
	int x1,y1,c1;
	int i,j,w/*color del norte*/;			/*los indices que se van evaluando*/
	int norteCte;		/*cte que se usa para evaluar amobos nortes*/
	int*k;					/*para llamar a la funcion de fill*/
	int storage;				/*donde se guarda el valorde la funcion*/
	long deltaT;		/*donde se guardara el delta de la jugada, se busca que sea lo mas alto*/
	long deltaTT;		/*delta temporal*/
	struct Registro* r;		/*donde se guarda el registro de jugadas*/
	struct Tablero*sim = (struct Tablero*)malloc(sizeof(struct Tablero));		/*tablero donde se simula lo que puede pasar*/
	setTablero(sim);
	deltaT=-10000000L;
	norteCte=1;
	/*r = (struct Registro*)malloc(sizeof(struct Registro));*/
	/*setRegistro(r);		/*inicializa el registro*/
	i=0;
	storage=0;
	k=&storage;
	deepCopy(tablero,sim);		/*copia el tablero a un clon*/
	
	printf("COLOR| %d\n",color);
	while(i<9){		/*de este ciclo sale un vector con las mejores jugadas*/
		j=0;
		while(j<14){
			w=0;
			while(w<2){
				norteCte=norteCte*-1;		/*para que evalue el norte rojo u blanco*/
			if(tablero->piezas[i][j]->color==0&&tablero->piezas[i][j]->jugando){	/*condiciones que debe cumplir, vacia y activa*/
				extra=0L;
				if(i>0&&tablero->piezas[i-1][j]->color==color){
					extra+=3000L;				/*dar bonificacion si el color de a la par es el mismo*/
					storage=0;	
					k=&storage;
					fillAux(sim,i-1,j,color,k);
					/*printf("Storage:%d ",storage);*/
					if(storage>2)				/*quitar bonificacion si es de un dolmen*/
						extra-=10000L;
				}
				if(i<8&&tablero->piezas[i+1][j]->color==color){
					extra+=3000L;					/*dar bonificacion si el color de a la par es el mismo*/
					storage=0;
					k=&storage;
					fillAux(sim,i+1,j,color,k);
					/*printf("Storage:%d ",storage);*/
					if(storage>2)				/*quitar bonificacion si es de un dolmen*/
						extra-=10000L;
				}
				if(j>0&&tablero->piezas[i][j-1]->color==color){
					extra+=1000L;				/*dar bonificacion si el color de a la par es el mismo*/
					storage=0;
					k=&storage;
					fillAux(sim,i,j-1,color,k);
					/*printf("Storage:%d ",storage);*/
					if(storage>2)			/*quitar bonificacion si es de un dolmen*/
						extra-=10000L;
				}
				if(j<13&&tablero->piezas[i][j+1]->color==color){
					extra+=3000L;			/*dar bonificacion si el color de a la par es el mismo*/
					storage=0;
					k=&storage;
					fillAux(sim,i,j+1,color,k);	
					/*printf("Storage:%d ",storage);*/
					if(storage>2)		/*quitar bonificacion si es de un dolmen*//*condiderar hacer que busque hacer dolmensa de cuatro*/
						extra-=10000L;
				}
				/*rojo*/
				
				sim->piezas[i][j]->color=color;
				/*printf("[%d]",sim->piezas[i][j]->color);*/
				deltaTT=getDelta(sim,color);
				deltaTT+=extra;
				deltaTT-=horizontal(sim,i,j,color,norteCte);
				/*printf("DELTA1| %ld",deltaTT);*/
				if(deltaTT>deltaT){
					xM[0]=i;
					yM[0]=j;colorM[0]=color;norteM[0]=norteCte;
					largoM=1;
					/*datosJugada(r->jugadas[0],i,j,-1,1,deltaTT);		/*temporalmente se pone el blanco como norte
					
					r->largo=1;*/
					deltaT=deltaTT;
				}
				else if (deltaTT==deltaT){
					xM[largoM]=i;
					yM[largoM]=j;colorM[largoM]=color;norteM[largoM]=norteCte;
					largoM++;
					/*datosJugada(r->jugadas[r->largo],i,j,-1,1,deltaTT);
					r->largo++;*/
					
					deltaT=deltaTT;
				}
				
				
				sim->piezas[i][j]->color=tablero->piezas[i][j]->color;
			}
			w++;
			}
				/*printf("Pos | x| %d y|%d col| %d delta| %ld\,||%d, %d|%d|\n",r->jugadas[0]->x,r->jugadas[0]->y,r->jugadas[0]->color,r->jugadas[0]->delta,i,j,r->largo);	*/
			j++;
		}
		i++;
	}
	i=0;
	/*printf("Pos x| %d y|%d col| %d delta| %ld\n",r->jugadas[i]->x,r->jugadas[i]->y,r->jugadas[0]->color,r->jugadas[i]->delta);*/
	
	srand(time(NULL));
	j = rand()%largoM;
	/*j=0;*/
	printf("RAND| %d",j);
	printf("X| %d, Y| %d, C| %d, CN| %d, long| %ld\n",xM[j],yM[j],colorM[j],norteM[j],deltaT);
	movida->x=xM[j];
	movida->y=yM[j];
	movida->color=colorM[j];
	movida->colorN=norteM[j];		/*guarda los valores de las jugadas*/
	/*
	i=0;
	printf("|%d|",r->largo);
	while(i<r->largo){
		printf("|%d|%d|",r->largo,i);
		printf("Pos %d: x: %d, y%d, col: %d, delta: %ld\n",r->jugadas[i]->x,r->jugadas[i]->y,r->jugadas[i]->color,r->jugadas[i]->delta);
		i++;
	}
	*/
	freeTablero(sim);
	free(sim);		/*libera la memoria*/
}

/*determina que color poner en el norte*/
long horizontal(struct Tablero*tablero,int i, int j, int color,int colo/*color del norte*/){
	long dprom,d2,d3,d4;
	int salvado;		/*se salva el color de una pieza*/
	dprom=0L;
	if(i>2&&tablero->piezas[i-1][j]->jugando&&tablero->piezas[i-2][j]->jugando){
		if(tablero->piezas[i-1][j]->color==0&&tablero->piezas[i-2][j]->color==0){
			tablero->piezas[i-2][j]->color=colo;
			tablero->piezas[i-1][j]->color=colo;
			salvado=tablero->piezas[i][j]->color;
			tablero->piezas[i][j]->color=0;
			dprom+=getDelta(tablero,color*-1);
			tablero->piezas[i][j]->color=salvado;
			tablero->piezas[i-2][j]->color=0;
			tablero->piezas[i-1][j]->color=0;
		}
	}
	if(i<7&&tablero->piezas[i+1][j]->jugando&&tablero->piezas[i+2][j]->jugando){
		if(tablero->piezas[i+1][j]->color==0&&tablero->piezas[i+2][j]->color==0){
			tablero->piezas[i+2][j]->color=colo;
			tablero->piezas[i+1][j]->color=colo;
			salvado=tablero->piezas[i][j]->color;
			tablero->piezas[i][j]->color=0;
			dprom+=getDelta(tablero,color*-1);
			tablero->piezas[i][j]->color=salvado;
			tablero->piezas[i+2][j]->color=0;
			tablero->piezas[i+1][j]->color=0;
		}
	}
	if(j>2&&tablero->piezas[i][j-1]->jugando&&tablero->piezas[i][j-2]->jugando){
		if(tablero->piezas[i][j-1]->color==0&&tablero->piezas[i][j-2]->color==0){
			tablero->piezas[i][j-1]->color=colo;
			tablero->piezas[i][j-2]->color=colo;
			salvado=tablero->piezas[i][j]->color;
			tablero->piezas[i][j]->color=0;
			dprom+=getDelta(tablero,color*-1);
			tablero->piezas[i][j]->color=salvado;
			tablero->piezas[i][j-2]->color=0;
			tablero->piezas[i][j-1]->color=0;
		}
	}
	if(j<11&&tablero->piezas[i][j+1]->jugando&&tablero->piezas[i][j+2]->jugando){
		if(tablero->piezas[i][j+1]->color==0&&tablero->piezas[i][j+2]->color==0){
			tablero->piezas[i][j+1]->color=colo;
			tablero->piezas[i][j+2]->color=colo;
			salvado=tablero->piezas[i][j]->color;
			tablero->piezas[i][j]->color=0;
			dprom+=getDelta(tablero,color*-1);
			tablero->piezas[i][j]->color=salvado;
			tablero->piezas[i][j+2]->color=0;
			tablero->piezas[i][j+1]->color=0;
		}
	}
	dprom=dprom/16;
	return dprom;
}

void jugar(struct Tablero*tablero){
	bool activo=true;
	int turno=1;		/*inicia el blanco*/
	int lastx,lasty;		/*coordenadas de la ultima movida*/
	char hola[5];		/*entrada*/
	char basura[5];		/*entrada*/
	char adios[2];		/*si se va a botar*/
	int piezas=0;	/*cuantas piezas se han jugado*/
	int cont;		/*operacion de caracteres contador en*/
	int letra,numero,color,norte;
	struct Movida* movida= malloc(sizeof(struct Movida));		/*variable global donde se guarda la jugada de la ia*/
	while(piezas<28&&activo){
		Mustafa(tablero,turno,movida);
		printf("Jugar? \n");
		fgets (hola, 6, stdin);
		fgets(basura,3,stdin);
		cont=0;
		while(cont<4){		/*pone en minusculas*/
			hola[cont]=hola[cont]|0x20;
			cont++;
		}
		letra=hola[0]-0x61;
		numero=hola[1]-0x30;
		if(hola[2]=='r')
			color=-1;
		else if (hola[2]=='b'){
			color=1;
		}
		else{
			color=0;
		}
		if(hola[3]=='r'){
			norte=-1;
		}
		else if (hola[3]=='b'){
			norte=1;
		}
		else{
			norte=0;
		}
		
		if(letra>=0&&letra<=8&&numero>=0&&numero<=13&&tablero->piezas[letra][numero]->pieza==false&&tablero->piezas[letra][numero]->jugando==true){
			contarDolmens(tablero);
			printf("DolmensR: %d | DolmensB: %d\n",tablero->dolmenRojo,tablero->dolmenBlanco);
			printf("Valor Tablero (r/b): %d | %d\n",tablero->valorRojo,tablero->valorBlanco);
			turno=turno*-1;		/*cambio de persona que debe jugar*/
			piezas++;
			printf("|%d,%d,%d,%d|\n",letra,numero,color,norte);
			tablero->piezas[letra][numero]->color=color;
			tablero->piezas[letra][numero]->colorNorte=norte;
			tablero->piezas[letra][numero]->vertical=true;
			tablero->piezas[letra][numero]->movible=true;
			tablero->piezas[letra][numero]->pieza=true;
			lastx=letra;
			lasty=numero;
			imprimirTablero(tablero);
			MustafaB(tablero,movida->x,movida->y,turno);
			printf("Volcar? \n");
			fgets (adios, 3, stdin);/*s=volcar*/
			fgets(basura,3,stdin);
			if(adios[0]=='s'){/*si lo va a volcar*/
				printf("|%c,%c|\n",adios[0],adios[1]);
				if(adios[1]=='n'){/*lo vuelca al norte*/
					if(lastx>=2){
						if(tablero->piezas[lastx-1][lasty]->pieza==false&&tablero->piezas[lastx-2][lasty]->pieza==false&&tablero->piezas[lastx-1][lasty]->jugando==true&&tablero->piezas[lastx-2][lasty]->jugando==true){
							tablero->piezas[lastx][lasty]->color=0;
							tablero->piezas[lastx][lasty]->colorNorte=0;
							tablero->piezas[lastx][lasty]->vertical=false;
							tablero->piezas[lastx][lasty]->pieza=false;
							/*nuevas posiciones datos*/
							tablero->piezas[lastx-1][lasty]->color=norte;
							tablero->piezas[lastx-1][lasty]->colorNorte=0;
							tablero->piezas[lastx-1][lasty]->vertical=false;
							tablero->piezas[lastx-1][lasty]->pieza=true;
							tablero->piezas[lastx-2][lasty]->color=norte;
							tablero->piezas[lastx-2][lasty]->colorNorte=0;
							tablero->piezas[lastx-2][lasty]->vertical=false;
							tablero->piezas[lastx-2][lasty]->pieza=true;
							
						}
						
					}
					
				}
				else if(adios[1]=='s'){/*lo vuelca al sur*/
					if(lastx<=6){
						if(tablero->piezas[lastx+1][lasty]->pieza==false&&tablero->piezas[lastx+2][lasty]->pieza==false&&tablero->piezas[lastx+1][lasty]->jugando==true&&tablero->piezas[lastx+2][lasty]->jugando==true){
							tablero->piezas[lastx][lasty]->color=0;
							tablero->piezas[lastx][lasty]->colorNorte=0;
							tablero->piezas[lastx][lasty]->vertical=false;
							tablero->piezas[lastx][lasty]->pieza=false;
							/*nuevas posiciones datos*/
							tablero->piezas[lastx+1][lasty]->color=norte;
							tablero->piezas[lastx+1][lasty]->colorNorte=0;
							tablero->piezas[lastx+1][lasty]->vertical=false;
							tablero->piezas[lastx+1][lasty]->pieza=true;
							tablero->piezas[lastx+2][lasty]->color=norte;
							tablero->piezas[lastx+2][lasty]->colorNorte=0;
							tablero->piezas[lastx+2][lasty]->vertical=false;
							tablero->piezas[lastx+2][lasty]->pieza=true;
							
						}
						
					}
					
				}
				else if(adios[1]=='o'){/*lo vuelca al oeste*/
					if(lasty>=2){
						if(tablero->piezas[lastx][lasty-1]->pieza==false&&tablero->piezas[lastx][lasty-2]->pieza==false&&tablero->piezas[lastx][lasty-1]->jugando==true&&tablero->piezas[lastx][lasty-2]->jugando==true){
							norte=-1*tablero->piezas[lastx][lasty]->colorNorte;
							tablero->piezas[lastx][lasty]->color=0;
							tablero->piezas[lastx][lasty]->colorNorte=0;
							tablero->piezas[lastx][lasty]->vertical=false;
							tablero->piezas[lastx][lasty]->pieza=false;
							/*nuevas posiciones datos*/
							tablero->piezas[lastx][lasty-1]->color=norte;
							tablero->piezas[lastx][lasty-1]->colorNorte=0;
							tablero->piezas[lastx][lasty-1]->vertical=false;
							tablero->piezas[lastx][lasty-1]->pieza=true;
							tablero->piezas[lastx][lasty-2]->color=norte;
							tablero->piezas[lastx][lasty-2]->colorNorte=0;
							tablero->piezas[lastx][lasty-2]->vertical=false;
							tablero->piezas[lastx][lasty-2]->pieza=true;
						}
						
					}
					
				}
				else if(adios[1]=='e'){/*lo vuelca al oeste*/
					if(lasty<=11){
						if(tablero->piezas[lastx][lasty+1]->pieza==false&&tablero->piezas[lastx][lasty+2]->pieza==false&&tablero->piezas[lastx][lasty+1]->jugando==true&&tablero->piezas[lastx][lasty+2]->jugando==true){
							norte=-1*tablero->piezas[lastx][lasty]->colorNorte;
							tablero->piezas[lastx][lasty]->color=0;
							tablero->piezas[lastx][lasty]->colorNorte=0;
							tablero->piezas[lastx][lasty]->vertical=false;
							tablero->piezas[lastx][lasty]->pieza=false;
							/*nuevas posiciones datos*/
							tablero->piezas[lastx][lasty+1]->color=norte;
							tablero->piezas[lastx][lasty+1]->colorNorte=0;
							tablero->piezas[lastx][lasty+1]->vertical=false;
							tablero->piezas[lastx][lasty+1]->pieza=true;
							tablero->piezas[lastx][lasty+2]->color=norte;
							tablero->piezas[lastx][lasty+2]->colorNorte=0;
							tablero->piezas[lastx][lasty+2]->vertical=false;
							tablero->piezas[lastx][lasty+2]->pieza=true;	
						}
					}
				}	
				
			}
			else{
				/*no cambia el turno de la persona*/
			}
			imprimirTablero(tablero);
			
		}
		if(hola[4]=='z'){
			activo=false;
		}
	}
printf("------------------FIN--------------------");	
	
}

extern int existeNivel(void);
extern void pintaNivel1(void);		/*pone el esqueleto del tablero*/
extern void pintaNivel(void);

void meterTablero(struct Tablero*tablero){		/*mete los datos del tablero a la pila*/
	int i,j,k,puta;
	
	int arreglo[126];
	
	for (i = 0; i < 9; i++)
	{
		for(j = 0; j < 14, j++)
		{
			arreglo[puta] = tablero->piezas[i][j]->color;
			puta++;
		}
	}
	
	
	pintaNivel();
	/*
	while(i<9){
		j=0;
		while(j<14){
			printf("f");
			k=tablero->piezas[i][j]->color;
			if(!tablero->piezas[i][j]->jugando){
				test(2);		/*no esta activa
			}
			else if(k==-1){
				test(4);		/*pone una en rojo
			}
			else if (k==1){
				test(1);
			}
			else{
				test(0);
			}
			j++;
		}
		i++;
	}
	*/
	
}

int main(){
	int i,j;
	struct Tablero *sim = (struct Tablero*)malloc(sizeof(struct Tablero));
	struct Tablero *tablero = (struct Tablero*)malloc(sizeof(struct Tablero));
	setTablero(tablero);
	setTipoTablero(tablero,3);
	meterTablero(tablero);
	/*printf("A");*/
	
	
	
	/*imprimirTablero(tablero);
	jugar(tablero);*/
	
	return 0;
}
