#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX 128
#define ADATOK "repulo.csv"
#define HSZ sizeof(int)/sizeof(short)*5  /* 5 v 10 jegyu max az int meretetol fuggoen */

char * kisbetusito(char str[]);

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

	while (fgets(seged, 1024, fajl))											//file olvasás
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

void repterkereses(Data *lista)											//Keresek repteret induló és érkezõ oldalon
{
	Data *iter = lista;
	char repternev[MAX];															//Ebbe bekérek
	char tmp[2][MAX];
	int vane = 0;														//Õ csak azért van hogy segítsen eldönteni, hogy létezik-e amit beírtak...




	do
	{
		printf("Kérem adja meg a keresett reptér nevét: "); getline(repternev, 127);		//Bekér

		//Ellenörzés, hogy írtak-e be valamit egyáltalán
		if (strlen(repternev) < 1)
		{
			printf("\n\nNem írtál be semmit! Próbáld újra!\n\n");
		}
	} while (strlen(repternev) < 1);

	kisbetusito(&repternev);

	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		//Ki kell menteni változóba mert a kisbetûsítõ függvénynek csak pointerrel tudod átadni a tömböt, így ha arra hívnám meg akkor a fõ listánkban változtatná meg a reptereket.
		strcpy(tmp[0], iter->repter1);
		strcpy(tmp[1], iter->repter2);

		if (!strcmp(kisbetusito(tmp[0]), repternev) || !strcmp(kisbetusito(tmp[1]), repternev))		//Ha van akkor kiirat, plusz átállítja a változót 1-re
		{																				
			printf("Járatszám: [%d]\n", iter->jaratszam);								
			printf("Honnan: [%s]\n", iter->repter1);
			printf("Hová: [%s]\n", iter->repter2);
			printf("Távolság: [%.2f km]\n", iter->tavolsag);
			printf("Osztályok: [%s]\n\n", iter->osztalyok);

			vane = 1;
		}
	}
	if (vane == 0) printf("Nincs ilyen nevû reptér !");									//Ha nincs akkor kiírja hogy bukta

}
void osztalyok_keresese(Data *lista)
{
	Data *iter = lista;
	char keresendo_osztaly[MAX];					//ide megy amit keresünk
	int talalat[MAX];
	int helyes_input = 1;							//ciklus átengedõõ
	int i, j;
	int counter=0;									//találatot számol


	//Hogy legyen valami szabály is
	printf("Jelmagyarázat:\n1 : Prémium\n2 : Bussiness\n3 : Turista\n\n");

	do
	{
		printf("Adja meg a keresett osztályokat közvetlenül egymás mellé írva (pl.: 123 -> mind a 3 osztály): ");
		getline(keresendo_osztaly, (MAX - 1));

		helyes_input = 1;

		//Check input
		if (strlen(keresendo_osztaly) > 0)
		{
			for (i = 0; i < strlen(keresendo_osztaly); i++)
			{
				//Ha nem egyezik 1-el,2-vel és 3-al 3 akkor ugye hibaüzenet
				if (keresendo_osztaly[i] != '1' && keresendo_osztaly[i] != '2' && keresendo_osztaly[i] != '3')
				{
					helyes_input = 0;
					printf("Helytelen karakter a %d. helyen!\nPróbáld újra!\n\n", i + 1);
				}
			}
		}
		else
		{
			helyes_input = 0;
		}
	} while (helyes_input == 0);


	//Keresés maga

	//végig a listán
	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		//végig a beírt számokon
		for (i = 0; i < strlen(keresendo_osztaly); i++)
		{
			//A standard a csv-ben a 1,2,3 így végigmegyünk azokon is.
			for (j = 0; j < strlen(iter->osztalyok); j++)
			{
				//ha van egyezés
				if (iter->osztalyok[j]==keresendo_osztaly[i])								
				{
					//megnézzük, hogy aktuális járatot megtaláltuk-e már (ha pl egy járatnál több osztály van megadva)
					if (array_contain(iter->jaratszam, &talalat, MAX) != 1)
					{
						//Ha nincs megtalálva, akkor felvesszük és kiiratjuk, így megspórolunk egy + ciklust.
						talalat[counter] = iter->jaratszam;
						counter=counter+1;

						printf("Járatszám: [%d]\n", iter->jaratszam);
						printf("Honnan: [%s]\n", iter->repter1);
						printf("Hová: [%s]\n", iter->repter2);
						printf("Távolság: [%.2f km]\n", iter->tavolsag);
						printf("Osztályok: [%s]\n\n", iter->osztalyok);
					}
					
				}
			}
		}
	}
}



//Megnézi egy tömbben, hogy tartalmazza-e az aktuális elemet.
int array_contain(int val, int *arr, int size){
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == val)
			return 1;
	}
	return 0;
}


//egy stringet (char[]-t) kisbetûssé alakít
char * kisbetusito(char str[])
{
	int i;

	for (i = 0; str[i]; i++){
		str[i] = tolower(str[i]);
	}
	return str;
}

void ido()										//Lekérdezi az idõt és a dátumot
{												//Valahogy majd visszatérési értének kellene használni, még nem jöttem rá hogyan !
	time_t t;									//(De kezdek agysorvadást kapni a mai napra !)
	t = time(NULL);
	printf("%s\n", ctime(&t));
}

void flush_in(FILE *in)
{
	int ch;

	do
		ch = fgetc(in);
	while (ch != EOF && ch != '\n');

	clearerr(in);
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
			ido();
			listakiir(lis);
			break;
		case 2:

			break;
		case 3:
			repterkereses(lis);
			break;
		case 4:
			jaratszamkereses(lis);
			break;
		case 5:
			osztalyok_keresese(lis);
			break;
		case 6:
			exit(0);
			break;
		}
		printf("\n\nNyomj egy <Enter>-t a menübe való visszatéréshez...");
		getchar();
		system("cls");
	} while (1 == 1);
}