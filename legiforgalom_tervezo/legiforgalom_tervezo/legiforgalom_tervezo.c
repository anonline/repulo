#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define HSZ sizeof(int)/sizeof(short)*5  /* 5 v 10 jegyu max az int meretetol fuggoen */
#define MAX 128
#define ADATOK "repulo.csv"



//Strukt�ra
typedef struct Data {
	int jaratszam;
	char repter1[MAX];
	char repter2[MAX];
	struct tm indulas;
	struct tm erkezes;
	int idoeltolodas;
	double tavolsag;
	char osztalyok[10];
	char repter1_country[MAX];
	char repter2_country[MAX];


	struct Data *kov;
}Data;

//�llom�s
typedef struct csucs { // a gr�f egy cs�cs�nhoz tartoz� inform�ci�k 
	int id;     // csucs azonosito sorszama
	char *allomas;
	char country[MAX];
	int parent_id;   // a csucs ose 
}csucs;



//J�rat
typedef struct el { // a gr�f egy �l�hez tartoz� inform�ci�k 
	int el_id;
	csucs kezdet;    // graf-el egyik vege 
	csucs veg;    // graf-el masik vege
	struct tm indulas;
	struct tm erkezes;
	int idoeltolodas;
	double tavolsag;
	char osztalyok[10];
}el;

//Eg�sz h�l�zat
typedef struct graf { // a teljes gr�f 
	csucs csucsok[21];
	el elek[MAX];
}graf;

//�tvonal h�l�zat
typedef	struct utvonal{
	csucs allomas;
	int parent;
	int el_id;
	struct utvonal *kov;
}utvonal;

//�kezetes bet�k
void set_charset(){									//�kezetes karakterek h�lleluja
	system("chcp 1250");
	system("cls");
}

//Eg�ssz�m ellen�rz�s
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

//Sz�veg �sszef�z�s
char* concat(char *s1, char *s2)
{
	char *result = malloc(strlen(s1) + strlen(s2) + 1);//+1 for the zero-terminator
	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

//Sz�vegbek�r�s
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

//Rendszerid� lek�rdez�se
struct tm ido()
{
	time_t most_ennyi_ido_van = time(NULL);
	struct tm helyi_ido = *localtime(&most_ennyi_ido_van);
	return helyi_ido;
}

//Egy stringet (char[]-t) kisbet�ss� alak�t
char * kisbetusito(char str[])
{
	int i;
	int len = strlen(str);
	char lower_str[MAX];
	
	strncpy(lower_str, str, len);
	lower_str[len] = '\0';

	for (i = 0; i<len; i++){
		switch (lower_str[i])
		{
		case '�':
			lower_str[i] = '�';
			break;
		case '�':
			lower_str[i] = '�';
			break;
		case '�':
			lower_str[i] = '�';
			break;
		case '�':
			lower_str[i] = '�';
			break;
		case '�':
			lower_str[i] = '�';
			break;
		case '�':
			lower_str[i] = '�';
			break;
		case '�':
			lower_str[i] = '�';
			break;
		case '�':
			lower_str[i] = '�';
			break;
		case '�':
			lower_str[i] = '�';
			break;
		default:
			lower_str[i] = tolower(str[i]);
			break;
		}
		
	}
		
	return lower_str;
}

//Megn�zi egy int t�mbben, hogy tartalmazza-e az aktu�lis elemet.
int array_contain(int val, int *arr, int size){
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == val)
			return 1;
	}
	return 0;
}

//Megn�zi egy char t�mbben, hogy tartalmazza-e az aktu�lis elemet.
int char_array_int_contain(int val, char *arr, int size){
	int i;
	for (i = 0; i < size; i++) {
		if (atoi(arr) == val)
			return 1;
	}
	return 0;
}

//Billenty� buffer �r�t�s
void flush_in(FILE *in)
{
	int ch;

	do
		ch = fgetc(in);
	while (ch != EOF && ch != '\n');

	clearerr(in);
}

//F�jlellen�rz�s
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

//Lita l�trehoz�sa
Data *listaletrehoz(){
	Data *l = NULL;
	int i,j,ora,perc,mp;
	int idotmp[2];
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
				sscanf_s(seged2, "%d:%d:%d", &ora,&perc,&mp);
				u->indulas.tm_hour = ora;
				u->indulas.tm_min = perc;
				u->indulas.tm_sec = mp;
				break;
			case 4:
				sscanf_s(seged2, "%d:%d:%d", &ora, &perc, &mp);
				u->erkezes.tm_hour = ora;
				u->erkezes.tm_min = perc;
				u->erkezes.tm_sec = mp;
				break;	
			case 5:
				u->idoeltolodas=atoi(seged2);
				break;
			case 6:
				strcpy(u->repter1_country, seged2);
				break;
			case 7:
				strcpy(u->repter2_country, seged2);
				break;
			case 8:
				u->tavolsag = atof(p);
				break;
			case 9:
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

//Lista ki�r�s
void listakiir(Data *lista){

	Data *iter;

	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		printf("J�ratsz�m: [%d]\n", iter->jaratszam);
		printf("Honnan: [%s]\n", iter->repter1);
		printf("Hov�: [%s]\n", iter->repter2);
		printf("Indul�si id�: [%d:%d]\n", iter->indulas.tm_hour, iter->indulas.tm_min);
		printf("�rkez�si id�: [%d:%d]\n", iter->erkezes.tm_hour, iter->erkezes.tm_min);
		printf("Id�eltolod�s: [GMT %s%d]\n", (iter->idoeltolodas>0?"+":""),iter->idoeltolodas);
		printf("Indul�si orsz�g: [%s]\n", iter->repter1_country);
		printf("�rekz�si orsz�g: [%s]\n", iter->repter2_country);
		printf("T�vols�g: [%.2f km]\n", iter->tavolsag);
		printf("Oszt�lyok: [%s]\n\n", iter->osztalyok);
	}
	printf("\n");
}

//J�ratsz�mkeres�s
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

//Rept�rkeres�s
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

	strcpy(repternev, kisbetusito(repternev));

	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		//Ki kell menteni v�ltoz�ba mert a kisbet�s�t� f�ggv�nynek csak pointerrel tudod �tadni a t�mb�t, �gy ha arra h�vn�m meg akkor a f� list�nkban v�ltoztatn� meg a reptereket.
		strcpy(tmp[0], iter->repter1);
		strcpy(tmp[1], iter->repter2);

		if ((!(strcmp(kisbetusito(tmp[0]), repternev))) || (!(strcmp(kisbetusito(tmp[1]), repternev))))		//Ha van akkor kiirat, plusz �t�ll�tja a v�ltoz�t 1-re
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

//Oszt�lykeres�s
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

//Orsz�gkeres�s
void orszagkereses(Data *lista){
	
	char indule[MAX];													//Indul�si vagy �rkez�si orsz�g eld�nt�se
	char orszag[MAX];													//Maga az orsz�g neve
	char tmp[2][MAX];													//kisbetusito fv haszn�lat�hoz seg�dv�ltozo
	Data *iter;


	printf("Jelmagyar�zat:\nI : Indul�si orsz�g\nE : �rkez�si orsz�g\n\n");
	
	do{																							//Bek�r am�g nem i, vagy e
		printf("Kerem adja meg a keresett poz�ci�t: "); getline(indule, MAX - 1);
		strcmp(indule, kisbetusito(indule));
		if ((strcmp(indule, "i") && strcmp(indule, "e")))
		{
			printf("Rossz poz�ci�!\n\n");
		}
	} while ((strcmp(indule,"i") && strcmp(indule, "e")));

	printf("Kerem adja meg az orsz�g nev�t: "); getline(orszag, MAX - 1);						//Bek�ri az orsz�got

	if (strcmp(indule, "i")==0)																	//Ha i akkor indul�si orsz�got keres �s �r ki...
	{	
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			strcpy(tmp[0], kisbetusito(iter->repter1_country));
			strcpy(tmp[1], kisbetusito(orszag));

			if (!(strcmp(tmp[0], tmp[1]))){
				printf("J�ratsz�m: [%d]\n", iter->jaratszam);
				printf("Honnan: [%s]\n", iter->repter1);
				printf("Hov�: [%s]\n", iter->repter2);
				printf("Indul�si orsz�g: [%s]\n", iter->repter1_country);
				printf("�rekz�si orsz�g: [%s]\n", iter->repter2_country);
				printf("T�vols�g: [%.2f km]\n", iter->tavolsag);
				printf("Oszt�lyok: [%s]\n\n", iter->osztalyok);
			}
		}

	}
	else																						//Ha nem i akkor erkezesi orsz�got keres �s �r ki...
	{
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			strcpy(tmp[0], kisbetusito(iter->repter2_country));
			strcpy(tmp[1], kisbetusito(orszag));

			if (!(strcmp(tmp[0], tmp[1]))){
				printf("J�ratsz�m: [%d]\n", iter->jaratszam);
				printf("Honnan: [%s]\n", iter->repter1);
				printf("Hov�: [%s]\n", iter->repter2);
				printf("Indul�si orsz�g: [%s]\n", iter->repter1_country);
				printf("�rekz�si orsz�g: [%s]\n", iter->repter2_country);
				printf("T�vols�g: [%.2f km]\n", iter->tavolsag);
				printf("Oszt�lyok: [%s]\n\n", iter->osztalyok);
			}
		}	
	}


}

//Id�pontkeres�s
void idopontkereses(Data *lista)
{
	int i, userhour, usermin, ok = 0;
	char indule[MAX];
	char idopont[MAX];
	Data *iter;

	printf("Jelmagyar�zat:\nI : Indul�si id�\nE : �rkez�si id�\n\n");

	do{																							//Bek�r am�g nem i, vagy e
		printf("Kerem adja meg a keresett poz�ci�t: "); getline(indule, MAX - 1);
		strcpy(indule, kisbetusito(indule));
		if (strcmp(indule, "i") && strcmp(indule, "e"))
		{
			printf("Rossz poz�ci�!\n\n");
		}
	} while (strcmp(indule, "i") && strcmp(indule, "e"));

	do{
		fflush(stdin);
		ok = 0;
		printf("Kerem adja meg az id�pontot: "); getline(idopont, MAX - 1);

	} while (sscanf_s(idopont, "%d:%d", &userhour, &usermin) != 2);

	if (strcmp(indule, "i") == 0)																	//Ha i akkor indul�si orsz�got keres �s �r ki...
	{
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			if (iter->indulas.tm_hour >= userhour && iter->indulas.tm_min >= usermin){
				printf("J�ratsz�m: [%d]\n", iter->jaratszam);
				printf("Honnan: [%s]\n", iter->repter1);
				printf("Hov�: [%s]\n", iter->repter2);
				printf("Indul�si id�: [%d:%d]\n", iter->indulas.tm_hour, iter->indulas.tm_min);
				printf("�rkez�si id�: [%d:%d]\n", iter->erkezes.tm_hour, iter->erkezes.tm_min);
				printf("Id�eltolod�s: [GMT %s%d]\n", (iter->idoeltolodas>0 ? "+" : ""), iter->idoeltolodas);
				printf("Indul�si orsz�g: [%s]\n", iter->repter1_country);
				printf("�rekz�si orsz�g: [%s]\n", iter->repter2_country);
				printf("T�vols�g: [%.2f km]\n", iter->tavolsag);
				printf("Oszt�lyok: [%s]\n\n", iter->osztalyok);
			}
		}

	}
	else																						//Ha nem i akkor erkezesi orsz�got keres �s �r ki...
	{
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			if (iter->erkezes.tm_hour >= userhour && iter->erkezes.tm_min >= usermin){
				printf("J�ratsz�m: [%d]\n", iter->jaratszam);
				printf("Honnan: [%s]\n", iter->repter1);
				printf("Hov�: [%s]\n", iter->repter2);
				printf("Indul�si id�: [%d:%d]\n", iter->indulas.tm_hour, iter->indulas.tm_min);
				printf("�rkez�si id�: [%d:%d]\n", iter->erkezes.tm_hour, iter->erkezes.tm_min);
				printf("Id�eltolod�s: [GMT %s%d]\n", (iter->idoeltolodas>0 ? "+" : ""), iter->idoeltolodas);
				printf("Indul�si orsz�g: [%s]\n", iter->repter1_country);
				printf("�rekz�si orsz�g: [%s]\n", iter->repter2_country);
				printf("T�vols�g: [%.2f km]\n", iter->tavolsag);
				printf("Oszt�lyok: [%s]\n\n", iter->osztalyok);
			}
		}
	}
}

float jegyar_kalkulator(int km, int osztaly)
{
	int kerozin_ar = 600;
	int fogasztas = 15;

	float jegyar = (km / 100)*fogasztas*kerozin_ar*(4-osztaly)*1.37;

	return jegyar;
}

graf * FillGraph(struct graf *Graf, Data *lista)
{
	Data *iter= (Data*)malloc(sizeof(Data));
	csucs csucsok[21];
	int csucs_check = 1;
	csucs csucs_buff;
	csucs csucs_buff2;
	el elek[MAX];
	int i =0;
	int j = 0;
	int k = 0;


	//Cs�cs felt�lt�s
	for (iter = lista; iter!= NULL; iter=iter->kov)
	{
		for (j = 0; j < i; j++)
		{
			if (strcmp(csucsok[j].allomas, iter->repter1) == 0)
			{
				csucs_check = 0;
			}
		}
		if (csucs_check == 1)
		{
			csucsok[i].id = iter->jaratszam;
			csucsok[i].allomas = iter->repter1;
			csucsok[i].parent_id = NULL;
			strcpy(csucsok[i].country, iter->repter1_country);
			i++;
		}
		csucs_check = 1;
	}

	//�l felt�lt�s
	i = 0;
	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		for (j = 0; j < 21; j++)
		{
			if (strcmp(csucsok[j].allomas, iter->repter1) == 0)
			{
				csucs_buff.id = csucsok[j].id;
			}
		}
		csucs_buff.allomas = iter->repter1;
		csucs_buff.parent_id = NULL;
		strcpy(csucs_buff.country, iter->repter1_country);
		elek[i].kezdet = csucs_buff;

		for (j = 0; j < 21; j++)
		{
			if (strcmp(csucsok[j].allomas, iter->repter2) == 0)
			{
				csucs_buff.id = csucsok[j].id;
			}
		}
		strcpy(csucs_buff.country, iter->repter2_country);
		csucs_buff.allomas = iter->repter2;

		elek[i].el_id = i;
		elek[i].indulas = iter->indulas;
		elek[i].erkezes = iter->erkezes;
		elek[i].tavolsag = iter->tavolsag;
		strcpy(elek[i].osztalyok, iter->osztalyok);
		elek[i].idoeltolodas = iter->idoeltolodas;
		elek[i].veg = csucs_buff;
		i++;
	}

	for (i = 0; i < MAX; i++)
	{
		Graf->elek[i] = elek[i];
	}
	
	for (i = 0; i < 21; i++)
	{
		Graf->csucsok[i] = csucsok[i];
	}

	

	return Graf;
}

int Kozos_pontok(struct graf *Graf, char *honnan, char *hova)
{
	int i, j, counter =0;
	el *elek[MAX];
	struct tm *varakozasi_ido = (struct tm*)malloc(sizeof(struct tm));
	int felhasznalt_elek;

	for (i = 0; i < MAX; i++)
	{
		if (strcmp(kisbetusito(Graf->elek[i].kezdet.allomas), honnan) == 0){
			elek[counter] = (el*)malloc(sizeof(el));
			*elek[counter] = Graf->elek[i];
			counter++;
		}
	}
	felhasznalt_elek = counter;

	for (i = 0; i < counter; i++)
	{
		for (j = 0; j < MAX; j++)
		{
			if (Graf->elek[j].kezdet.id == elek[i]->veg.id)
			{
				if (strcmp(kisbetusito(Graf->elek[j].veg.allomas), hova)==0)
				{
					printf("\n---------------\n");
					printf("\nHonnan: %s (%s)\n", elek[i]->kezdet.allomas, elek[i]->kezdet.country);
					printf("Hov�: %s (%s)\n", elek[i]->veg.allomas, elek[i]->veg.country);
					printf("Indul�s: %d:%d\n", elek[i]->indulas.tm_hour, elek[i]->indulas.tm_min);
					printf("�rkez�s: %d:%d\n", elek[i]->erkezes.tm_hour, elek[i]->erkezes.tm_min);
					printf("Id�eltolod�s: GMT %s%d\n", (elek[i]->idoeltolodas > 0 ? "+" : ""), elek[i]->idoeltolodas);
					printf("T�vols�g: %.2f km\n", elek[i]->tavolsag);
					printf("Oszt�lyok: %s\n", elek[i]->osztalyok);

					varakozasi_ido->tm_hour = Graf->elek[j].indulas.tm_hour - elek[i]->erkezes.tm_hour;
					varakozasi_ido->tm_min = Graf->elek[j].indulas.tm_min - elek[i]->erkezes.tm_min;
					if (varakozasi_ido->tm_min < 0)
					{
						varakozasi_ido->tm_hour = varakozasi_ido->tm_hour - 1;
						varakozasi_ido->tm_min = 60 + varakozasi_ido->tm_min;
					}
					printf("\nV�rakoz�si id�: %d:%d\n", varakozasi_ido->tm_hour, varakozasi_ido->tm_min);
					printf("Id� eltol�d�s: GMT %s%d\n", ((elek[i]->idoeltolodas + Graf->elek[j].idoeltolodas) > 0 ? "+" : ""), (elek[i]->idoeltolodas + Graf->elek[j].idoeltolodas));

					printf("\nHonnan: %s (%s)\n", Graf->elek[j].kezdet.allomas, Graf->elek[j].kezdet.country);
					printf("Hov�: %s (%s)\n", Graf->elek[j].veg.allomas, Graf->elek[j].veg.country);
					printf("Indul�s: %d:%d\n", Graf->elek[j].indulas.tm_hour, Graf->elek[j].indulas.tm_min);
					printf("�rkez�s: %d:%d\n", Graf->elek[j].erkezes.tm_hour, Graf->elek[j].erkezes.tm_min);
					printf("Id�eltolod�s: GMT %s%d\n", (Graf->elek[j].idoeltolodas > 0 ? "+" : ""), Graf->elek[j].idoeltolodas);
					printf("T�vols�g: %.2f km\n", Graf->elek[j].tavolsag);
					printf("Oszt�lyok: %s\n", Graf->elek[j].osztalyok);
					felhasznalt_elek--;
				}
			}
		}
	}

	return counter;
}

int FindPath(struct graf *Graf, char *honnan, char *hova, int return_type)
{
	utvonal *ut_vonal[MAX];
	utvonal *buffer, *iter;
	el *kozvetlen[MAX];

	char hova2[MAX];
	char *p;
	int indulo_csucsok_index = 0;
	int kozvetlen_jarat_index = 0;
	int i, j;
	int first = 1;

	for (i = 0; i < MAX; i++)
	{
		if (!(strcmp(kisbetusito(Graf->elek[i].kezdet.allomas), honnan))){

			ut_vonal[indulo_csucsok_index] = (utvonal*)malloc(sizeof(utvonal));

			ut_vonal[indulo_csucsok_index]->allomas = Graf->elek[i].kezdet;
			ut_vonal[indulo_csucsok_index]->el_id = Graf->elek[i].el_id;
			ut_vonal[indulo_csucsok_index]->kov = NULL;
			indulo_csucsok_index++;

		}
	}

	for (i = indulo_csucsok_index; i < MAX; i++)
	{
		ut_vonal[i] = NULL;
	}

	for (i = 0; i <= indulo_csucsok_index; i++)
	{
		for (buffer = ut_vonal[i]; i < indulo_csucsok_index; buffer = ut_vonal[++i])
		{
			for (j = 0; j < MAX; j++)
			{
				if (buffer->el_id == Graf->elek[j].el_id)
				{
					if (!(strcmp(Graf->elek[j].kezdet.allomas, buffer->allomas.allomas))){
						if (!(strcmp(kisbetusito(Graf->elek[j].veg.allomas), hova))){
							iter = (utvonal*)malloc(sizeof(utvonal));

							iter->allomas = Graf->elek[j].veg;
							iter->parent = buffer->allomas.id;
							iter->el_id = -1;				//Nincs tov�bb = -1
							ut_vonal[i]->kov = iter;
							ut_vonal[i]->kov->kov = NULL;
						}
					}
				}
			}
		}
	}


	//Kell m�g ID-ket egyeztetni ut_vonal-ban cs�csokat az �lekhez.
	for (i = 0; i < indulo_csucsok_index; i++)
	{
		buffer = ut_vonal[i];
		for (j = 0; j < MAX; j++)
		{
			if (buffer->kov != NULL && buffer->el_id == Graf->elek[j].el_id)
			{
				if (Graf->elek[j].kezdet.id == buffer->allomas.id)
				{
					if (Graf->elek[j].veg.id == buffer->kov->allomas.id)
					{
						kozvetlen[kozvetlen_jarat_index] = (el*)malloc(sizeof(el));
						*kozvetlen[kozvetlen_jarat_index] = Graf->elek[j];
						kozvetlen_jarat_index++;
					}
				}
			}
		}
	}
	if (return_type == 0)
	{
		if (kozvetlen_jarat_index > 0) printf("\n%s �s %s k�z�tt k�zleked� j�ratok:\n", kozvetlen[0]->kezdet.allomas, kozvetlen[0]->veg.allomas);
		for (i = 0; i < kozvetlen_jarat_index; i++)
		{
			printf("\nHonnan: %s (%s)\n", kozvetlen[i]->kezdet.allomas, kozvetlen[i]->kezdet.country);
			printf("Hov�: %s (%s)\n", kozvetlen[i]->veg.allomas, kozvetlen[i]->veg.country);
			printf("Indul�s: %d:%d\n", kozvetlen[i]->indulas.tm_hour, kozvetlen[i]->indulas.tm_min);
			printf("�rkez�s: %d:%d\n", kozvetlen[i]->erkezes.tm_hour, kozvetlen[i]->erkezes.tm_min);
			printf("Id�eltolod�s: GMT %s%d\n", (kozvetlen[i]->idoeltolodas > 0 ? "+" : ""), kozvetlen[i]->idoeltolodas);
			printf("T�vols�g: %.2f km\n", kozvetlen[i]->tavolsag);
			printf("Jegy�rak:");
			for (p = strsep(kozvetlen[i]->osztalyok, ","); p != NULL; p = strsep(NULL, ","))
			{
				printf("\n%s. oszt�ly: %.2f Ft\n", p, jegyar_kalkulator(kozvetlen[i]->tavolsag, atoi(p)));
			}
		}
	}
	switch (return_type)
	{
	case 0:
		return kozvetlen_jarat_index;
		break;
	case 1:
		return (kozvetlen[0]->indulas.tm_hour * 60) + (kozvetlen[0]->indulas.tm_min);
		break;
	case 2:
		return (kozvetlen[0]->erkezes.tm_hour * 60) + (kozvetlen[0]->erkezes.tm_min);
		break;
	case 3:
		return kozvetlen[0]->idoeltolodas;
		break;
	default:
		break;
	}
	return kozvetlen_jarat_index;
}
//�tvonaltervez�s
void utvonaltervezes(Data *lista){
	char honnan[MAX];						//innen indulna az �gyf�l
	char hova[MAX];
	char erintve[MAX];
	struct tm *varakozasi_ido = (struct tm*)malloc(sizeof(struct tm));

	int i;
	int van_indulo_orszag = 0;				//A bevitel ellen�rz�s�re szolg�l
	int van_erkezo_orszag = 0;				//A bevitel ellen�rz�s�re szolg�l
	int van_erinto_orszag = 0;				//A bevitel ellen�rz�s�re szolg�l

	char seged[MAX];
	int lvl=0;
	Data *iter;
	graf *Graf = malloc((graf*)sizeof(graf));



	FillGraph(Graf, lista);


	do
	{
		printf("Kerem adja meg az indul� �llom�st:\n>");
		getline(honnan, MAX - 1);
		strcpy(honnan, kisbetusito(honnan));
		for (iter = lista; iter->kov != NULL; iter = iter->kov)
		{
			if (!(strcmp(kisbetusito(iter->repter1), honnan)))
			{
				van_indulo_orszag = 1;
			}
		}

		if (van_indulo_orszag == 0)
		{
			printf("\nNincs ilyen indul� �llom�s!\n\n");
		}
	} while (van_indulo_orszag == 0 && strlen(honnan) < 1);




	do
	{
		printf("Kerem adja meg az �rinteni k�v�nt �llom�st (Ha k�zvetlen j�ratot keres, hagyja �resen):\n>");
		getline(erintve, MAX - 1);
		strcpy(erintve, kisbetusito(erintve));
		for (iter = lista; iter->kov != NULL; iter = iter->kov)
		{
			if (strlen(erintve) > 0)
			{
				if (strcmp(kisbetusito(iter->repter1), erintve) == 0)
				{
					van_erinto_orszag = 1;
				}
			}
			else
			{
				van_erinto_orszag = 1;
			}
		}

		if (van_erinto_orszag == 0)
		{
			printf("\nNincs ilyen �llom�s!\n\n");
		}

	} while (van_erinto_orszag == 0);





	do
	{
		printf("\nKerem adja meg az �rkez� �llom�st:\n>");
		getline(hova, MAX - 1);
		strcpy(hova, kisbetusito(hova));
		for (iter = lista; iter->kov != NULL; iter = iter->kov)
		{
			if (!(strcmp(kisbetusito(iter->repter2), hova)))
			{
				van_erkezo_orszag = 1;
			}
		}

		if (van_erkezo_orszag == 0)
		{
			printf("\nNincs ilyen �rkez� �llom�s!\n\n");
		}
	} while (van_erkezo_orszag == 0 && strlen(hova) < 1);

	printf("\nTal�latok:\n");
	if (strlen(erintve) > 0)
	{
		lvl = FindPath(Graf, honnan, erintve, 0);
		if (lvl > 0)
		{
			varakozasi_ido->tm_hour = FindPath(Graf, erintve, hova, 1) / 60;
			varakozasi_ido->tm_hour = varakozasi_ido->tm_hour - FindPath(Graf, honnan, erintve, 2) / 60;
			if (varakozasi_ido->tm_hour < 0)
			{
				varakozasi_ido->tm_hour = 24 + varakozasi_ido->tm_hour;
			}
			varakozasi_ido->tm_min = FindPath(Graf, erintve, hova, 1) % 60 - FindPath(Graf, honnan, erintve, 2) % 60;
			if (varakozasi_ido->tm_min < 0)
			{
				varakozasi_ido->tm_min = 60 - varakozasi_ido->tm_min;
				varakozasi_ido->tm_hour = varakozasi_ido->tm_hour - 1;
			}
			printf("\nV�rakoz�si id�: %d:%d\n", varakozasi_ido->tm_hour, varakozasi_ido->tm_min);
			printf("Id� eltol�d�s: GMT %s%d\n", ((FindPath(Graf, honnan, erintve, 3) + FindPath(Graf, honnan, erintve, 3)) > 0 ? "+" : ""), (FindPath(Graf, honnan, erintve, 3) + FindPath(Graf, honnan, erintve, 3)));
		}
		lvl = lvl+FindPath(Graf, erintve, hova, 0);

	}
	else
	{
		lvl = FindPath(Graf, honnan, hova, 0);
	}

	if (lvl == 0)
	{
		printf("\nSajnos k�zvetlen j�ratot nem tal�ltam, de az al�bbi helyeken �tsz�ll�si lehet�s�g van.\n\n");
		Kozos_pontok(Graf, honnan, hova);
	}

	free(Graf);
}
//F�program
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
			//ido();
			//listakiir(lis);
			utvonaltervezes(lis);
			break;
		case 2:
			printf("OK");
			break;
		case 3:
			repterkereses(lis);
			break;
		case 4:
			orszagkereses(lis);
			break;
		case 5:
			idopontkereses(lis);
			break;
		case 6:
			jaratszamkereses(lis);
			break;
		case 7:
			osztalyok_keresese(lis);
			break;
		case 8:
			exit(0);
			break;
		}
		printf("\n\nNyomj egy <Enter>-t a men�be val� visszat�r�shez...");
		getchar();
		system("cls");
	} while (1 == 1);
}
