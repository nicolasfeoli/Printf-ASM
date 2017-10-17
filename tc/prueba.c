#include "string.h"
struct a{
	int x,y,z;
};
struct b{
	struct a*j[12];
};
struct a*newa(int x,int y,int z){
	struct a* p = malloc(sizeof(struct a));
	p->x=x;
	p->y=y;
	p->z=z;
	return p;
}

void printb(struct b*be){
	int cont=12;
	while(cont>0){
		printf("%d|%d|%d ",be->j[cont]->x,be->j[cont]->y,be->j[cont]->z);
		cont--;
	}
}
int main (void){
	struct b*be2;
	int a=28;
	int b=12;
	struct b*be= malloc(sizeof(struct b));
	while(b>0){
		a--;
		a--;
		be->j[b]=newa(a-2,a+2,a);
		b--;
	}
	*be2=*be;
	be2->j[1]->x=555;
	printb(be);
	printf("\n");
	printb(be2);
	return 0;
}