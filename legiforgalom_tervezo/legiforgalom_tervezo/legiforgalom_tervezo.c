#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>


#define MAX 128
#define ADATOK "repulo.csv"

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
	printf("Fájl ellenõrzése...\n");
	FILE *txt = fopen(ADATOK, "r");
	
	if (txt == NULL) 
		{ 
			printf("Hiba történt a fájl beolvasása közben!\nKérem ellenõrizze a fájlt!");
			getchar();
			exit(0);
		}
	else 
		{
			printf("Az ellenõrzés sikeres, a fájl létezik!");
			fclose(txt);
			getchar();
			system("cls");
		}
	
}

Data *listaletrehoz(){
	Data *l = NULL;
	int i;
	char seged[1024];														//ebben van az aktuális sor
	char seged2[1024];														//ebben az aktuális sornak 1db eleme
	char *p;																//token kereséshez
	char *running;
	FILE *fajl;

	fajl = fopen(ADATOK, "r");

	while (fgets(seged,1024,fajl))											//file olvasás
	{
		Data *u = (Data*)malloc(sizeof(Data));
		u->kov = l;

		i = 0;
		running = (seged);
		for (p = strsep(*running, ";"); p != NULL;)						//strtok: tokenek alapján "szétdarabolja" a 
		{																					//stringet és visszatér a token helyének 
			strcpy(seged2, p);																//pointerével

			switch (i)
			{
				case 0:
					u->jaratszam = atoi(p);
					break;
				case 1:
					strcpy(u->repter1, seged2);
					break;
				case 2:
					strcpy(u->repter2, seged2);
					break;
				case 3:
					u->tavolsag = atof(p);
					break;
				case 4:
					strtok(seged2, "\n");
					strcpy(u->osztalyok, seged2);
					break;
			default:
				break;
			}
			i++;
		}

		l = u;
	}
	return l;
}

void listakiir(Data *lista){

	Data *iter;

	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		printf("Járatszám: [%d]\n", iter->jaratszam);
		printf("Honnan: [%s]\n", iter->repter1);
		printf("Hová: [%s]\n", iter->repter2);
		printf("Távolság: [%.2f km]\n", iter->tavolsag);
		printf("Osztályok: [%s]\n\n", iter->osztalyok);
	}
	printf("\n");
}

void set_charset(){						//Ékezetes karakterek hálleluja
	system("chcp 1250");
	system("cls");
}

void main()
{
	set_charset();
	fajlellenorzes();
	Data *lis = listaletrehoz();
	listakiir(lis);
	getchar();
}