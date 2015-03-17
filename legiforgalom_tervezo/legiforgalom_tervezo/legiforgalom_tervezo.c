#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 128
#define ADATOK "repulo.csv"
#define HSZ sizeof(int)/sizeof(short)*5  /* 5 v 10 jegyu max az int meretetol fuggoen */

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

int getline(char s[], int lim)
{
	int i;
	char c;

	for (i = 0; i<lim && (c = getchar()) != EOF && c != '\n'; ++i)
		s[i] = c;

	s[i] = '\0';

	while (c != EOF && c != '\n')
		c = getchar();
	return i;

}

int egesze(char s[])
{
	int i = 0;
	int kezd;

	/* feher karakterek atlepese */
	while (s[i] == ' ' || s[i] == '\n' || s[i] == '\t')
		i++;
	if (s[i] == '+' || s[i] == '-')
		i++;
	kezd = i;

	while (s[i] >= '0' && s[i] <= '9' && i - kezd<HSZ)
		++i;

	if (s[i] != 0 && s[i] != '\n' && s[i] != '\t' && s[i] != ' ')
		return 0;
	else
		return 1;
}

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
			//getchar();
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
		for (p = strsep(running, ";"); p != NULL; p = strsep(NULL, ";"))					//strtok: tokenek alapján "szétdarabolja" a 
		{																					//stringet és visszatér a token helyének 
			strcpy(seged2, p);																//pointerével

			switch (i)
			{
				case 0:
					u->jaratszam = atoi(p);													//Ha nincs járatszám 0-val tér vissza
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

void set_charset(){									//Ékezetes karakterek hálleluja
	system("chcp 1250");
	system("cls");
}

void jaratszamkereses(Data *lista)					//Keressük meg a járatunkat
{
	Data *iter;
	char seged[10];
	int jaratszam = 0;

	do{
		printf("Kerem adja meg a jaratszamot: "); getline(seged, 9);	//Bekérjük a számot, addig amíg nem egész
		if (egesze(seged) != 1) printf("Hibás járatszám ! Kérem adja meg újra !\n\n");
	} while (egesze(seged) != 1);

	jaratszam = atoi(seged);											//Átalakítom int-re

	for (iter = lista; iter != NULL; iter = iter->kov){					//Végigfutok a listán és kiírok
		if (iter->jaratszam == jaratszam){
			printf("Járatszám: [%d]\n", iter->jaratszam);
			printf("Honnan: [%s]\n", iter->repter1);
			printf("Hová: [%s]\n", iter->repter2);
			printf("Távolság: [%.2f km]\n", iter->tavolsag);
			printf("Osztályok: [%s]\n\n", iter->osztalyok);
		}
	}

}

void main()
{
	int akt_menu_elem;

	set_charset();
	fajlellenorzes();
	Data *lis = listaletrehoz();
	//listakiir(lis);

	do
	{
		akt_menu_elem = menu();
		
		system("cls");

		switch (akt_menu_elem)
		{
		case 1:
			listakiir(lis);
			break;
		case 2:
			
			break;
		case 3:
			break;
		case 4:
			jaratszamkereses(lis);
			break;
		case 5:
			exit(0);
			break;
		}

		getchar();
		system("cls");
	} while (1==1);
}