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

	while (fgets(seged, 1024, fajl))											//file olvas�s
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

void repterkereses(Data *lista)											//Keresek repteret indul� �s �rkez� oldalon
{
	Data *iter = lista;
	char repternev[MAX];															//Ebbe bek�rek
	char tmp[2][MAX];
	int vane = 0;														//� csak az�rt van hogy seg�tsen eld�nteni, hogy l�tezik-e amit be�rtak...




	do
	{
		printf("K�rem adja meg a keresett rept�r nev�t: "); getline(repternev, 127);		//Bek�r

		//Ellen�rz�s, hogy �rtak-e be valamit egy�ltal�n
		if (strlen(repternev) < 1)
		{
			printf("\n\nNem �rt�l be semmit! Pr�b�ld �jra!\n\n");
		}
	} while (strlen(repternev) < 1);

	kisbetusito(&repternev);

	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		//Ki kell menteni v�ltoz�ba mert a kisbet�s�t� f�ggv�nynek csak pointerrel tudod �tadni a t�mb�t, �gy ha arra h�vn�m meg akkor a f� list�nkban v�ltoztatn� meg a reptereket.
		strcpy(tmp[0], iter->repter1);
		strcpy(tmp[1], iter->repter2);

		if (!strcmp(kisbetusito(tmp[0]), repternev) || !strcmp(kisbetusito(tmp[1]), repternev))		//Ha van akkor kiirat, plusz �t�ll�tja a v�ltoz�t 1-re
		{																				
			printf("J�ratsz�m: [%d]\n", iter->jaratszam);								
			printf("Honnan: [%s]\n", iter->repter1);
			printf("Hov�: [%s]\n", iter->repter2);
			printf("T�vols�g: [%.2f km]\n", iter->tavolsag);
			printf("Oszt�lyok: [%s]\n\n", iter->osztalyok);

			vane = 1;
		}
	}
	if (vane == 0) printf("Nincs ilyen nev� rept�r !");									//Ha nincs akkor ki�rja hogy bukta

}
void osztalyok_keresese(Data *lista)
{
	Data *iter = lista;
	char keresendo_osztaly[MAX];					//ide megy amit keres�nk
	int talalat[MAX];
	int helyes_input = 1;							//ciklus �tenged��
	int i, j;
	int counter=0;									//tal�latot sz�mol


	//Hogy legyen valami szab�ly is
	printf("Jelmagyar�zat:\n1 : Pr�mium\n2 : Bussiness\n3 : Turista\n\n");

	do
	{
		printf("Adja meg a keresett oszt�lyokat k�zvetlen�l egym�s mell� �rva (pl.: 123 -> mind a 3 oszt�ly): ");
		getline(keresendo_osztaly, (MAX - 1));

		helyes_input = 1;

		//Check input
		if (strlen(keresendo_osztaly) > 0)
		{
			for (i = 0; i < strlen(keresendo_osztaly); i++)
			{
				//Ha nem egyezik 1-el,2-vel �s 3-al 3 akkor ugye hiba�zenet
				if (keresendo_osztaly[i] != '1' && keresendo_osztaly[i] != '2' && keresendo_osztaly[i] != '3')
				{
					helyes_input = 0;
					printf("Helytelen karakter a %d. helyen!\nPr�b�ld �jra!\n\n", i + 1);
				}
			}
		}
		else
		{
			helyes_input = 0;
		}
	} while (helyes_input == 0);


	//Keres�s maga

	//v�gig a list�n
	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		//v�gig a be�rt sz�mokon
		for (i = 0; i < strlen(keresendo_osztaly); i++)
		{
			//A standard a csv-ben a 1,2,3 �gy v�gigmegy�nk azokon is.
			for (j = 0; j < strlen(iter->osztalyok); j++)
			{
				//ha van egyez�s
				if (iter->osztalyok[j]==keresendo_osztaly[i])								
				{
					//megn�zz�k, hogy aktu�lis j�ratot megtal�ltuk-e m�r (ha pl egy j�ratn�l t�bb oszt�ly van megadva)
					if (array_contain(iter->jaratszam, &talalat, MAX) != 1)
					{
						//Ha nincs megtal�lva, akkor felvessz�k �s kiiratjuk, �gy megsp�rolunk egy + ciklust.
						talalat[counter] = iter->jaratszam;
						counter=counter+1;

						printf("J�ratsz�m: [%d]\n", iter->jaratszam);
						printf("Honnan: [%s]\n", iter->repter1);
						printf("Hov�: [%s]\n", iter->repter2);
						printf("T�vols�g: [%.2f km]\n", iter->tavolsag);
						printf("Oszt�lyok: [%s]\n\n", iter->osztalyok);
					}
					
				}
			}
		}
	}
}



//Megn�zi egy t�mbben, hogy tartalmazza-e az aktu�lis elemet.
int array_contain(int val, int *arr, int size){
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == val)
			return 1;
	}
	return 0;
}


//egy stringet (char[]-t) kisbet�ss� alak�t
char * kisbetusito(char str[])
{
	int i;

	for (i = 0; str[i]; i++){
		str[i] = tolower(str[i]);
	}
	return str;
}

void ido()										//Lek�rdezi az id�t �s a d�tumot
{												//Valahogy majd visszat�r�si �rt�nek kellene haszn�lni, m�g nem j�ttem r� hogyan !
	time_t t;									//(De kezdek agysorvad�st kapni a mai napra !)
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
		printf("\n\nNyomj egy <Enter>-t a men�be val� visszat�r�shez...");
		getchar();
		system("cls");
	} while (1 == 1);
}