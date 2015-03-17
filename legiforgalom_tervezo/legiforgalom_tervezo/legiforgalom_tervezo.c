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
	printf("F�jl ellen�rz�se...\n");
	FILE *txt = fopen(ADATOK, "r");
	
	if (txt == NULL) 
		{ 
			printf("Hiba t�rt�nt a f�jl beolvas�sa k�zben!\nK�rem ellen�rizze a f�jlt!");
			getchar();
			exit(0);
		}
	else 
		{
			printf("Az ellen�rz�s sikeres, a f�jl l�tezik!");
			fclose(txt);
			//getchar();
			system("cls");
		}
	
}

Data *listaletrehoz(){
	Data *l = NULL;
	int i;
	char seged[1024];														//ebben van az aktu�lis sor
	char seged2[1024];														//ebben az aktu�lis sornak 1db eleme
	char *p;																//token keres�shez
	char *running;
	FILE *fajl;

	fajl = fopen(ADATOK, "r");

	while (fgets(seged,1024,fajl))											//file olvas�s
	{
		Data *u = (Data*)malloc(sizeof(Data));
		u->kov = l;

		i = 0;
		running = (seged);
		for (p = strsep(running, ";"); p != NULL; p = strsep(NULL, ";"))					//strtok: tokenek alapj�n "sz�tdarabolja" a 
		{																					//stringet �s visszat�r a token hely�nek 
			strcpy(seged2, p);																//pointer�vel

			switch (i)
			{
				case 0:
					u->jaratszam = atoi(p);													//Ha nincs j�ratsz�m 0-val t�r vissza
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
		printf("J�ratsz�m: [%d]\n", iter->jaratszam);
		printf("Honnan: [%s]\n", iter->repter1);
		printf("Hov�: [%s]\n", iter->repter2);
		printf("T�vols�g: [%.2f km]\n", iter->tavolsag);
		printf("Oszt�lyok: [%s]\n\n", iter->osztalyok);
	}
	printf("\n");
}

void set_charset(){									//�kezetes karakterek h�lleluja
	system("chcp 1250");
	system("cls");
}

void jaratszamkereses(Data *lista)					//Keress�k meg a j�ratunkat
{
	Data *iter;
	char seged[10];
	int jaratszam = 0;

	do{
		printf("Kerem adja meg a jaratszamot: "); getline(seged, 9);	//Bek�rj�k a sz�mot, addig am�g nem eg�sz
		if (egesze(seged) != 1) printf("Hib�s j�ratsz�m ! K�rem adja meg �jra !\n\n");
	} while (egesze(seged) != 1);

	jaratszam = atoi(seged);											//�talak�tom int-re

	for (iter = lista; iter != NULL; iter = iter->kov){					//V�gigfutok a list�n �s ki�rok
		if (iter->jaratszam == jaratszam){
			printf("J�ratsz�m: [%d]\n", iter->jaratszam);
			printf("Honnan: [%s]\n", iter->repter1);
			printf("Hov�: [%s]\n", iter->repter2);
			printf("T�vols�g: [%.2f km]\n", iter->tavolsag);
			printf("Oszt�lyok: [%s]\n\n", iter->osztalyok);
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