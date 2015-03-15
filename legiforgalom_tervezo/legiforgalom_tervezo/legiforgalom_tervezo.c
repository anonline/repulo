#include <stdio.h>
#include <conio.h>

#define MAX 128

typedef struct Data {
	int jaratszam;
	char repter1[MAX];
	char repter2[MAX];
	//indulas
	//erkezes
	double tavolsag;
	char osztalyok[10];


	struct Data *kov;
}Data;

void fajlellenorzes(){
	printf("Fajl ellenorzese...\n\n");
	FILE *txt = fopen("repulo.csv", "r");
	
	if (txt == NULL) 
		{ 
			printf("Hiba van a fajl beolvasasa kozben\nKerem ellenorizze a fajlt !");
			getchar();
			exit(0);
		}
	else 
		{
			printf("Sikeres !");
			fclose(txt);
			getchar();
			system("cls");
		}
	
}

Data *listaletrehoz(){
	Data *l = NULL;
	int i;

	for (i = 0; i<=7; ++i){
		Data *u = (Data*)malloc(sizeof(Data));
		u->kov = l;
		u->jaratszam = 1;
		l = u;
	}
	return l;
}

void listakiir(Data *lista){

	Data *iter;

	for (iter = lista; iter != NULL; iter = iter->kov)
		printf("[%d] ", iter->jaratszam);
	printf("\n");
}

void main()
{
	fajlellenorzes();
	Data *lis = listaletrehoz();
	listakiir(lis);
	getchar();
}