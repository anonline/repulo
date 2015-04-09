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

	for (i = 0; str[i]; i++){
		str[i] = tolower(str[i]);
	}
	return str;
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
	} while (strcmp(kisbetusito(indule), "i") && strcmp(kisbetusito(indule), "e"));

	printf("Kerem adja meg az orsz�g nev�t: "); getline(orszag, MAX - 1);						//Bek�ri az orsz�got

	if (strcmp(indule, "i")==0)																	//Ha i akkor indul�si orsz�got keres �s �r ki...
	{	
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			strcpy(tmp[0], iter->repter1_country);
			strcpy(tmp[1], orszag);

			if (strcmp(kisbetusito(tmp[0]), kisbetusito(tmp[1]))==0){
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
			strcpy(tmp[0], iter->repter2_country);
			strcpy(tmp[1], orszag);

			if (strcmp(kisbetusito(tmp[0]), kisbetusito(tmp[1])) == 0){
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
	} while (strcmp(kisbetusito(indule), "i") && strcmp(kisbetusito(indule), "e"));

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

//Do magic
char * utvonal(Data *lista, char *honnan, char *cel, char * utvonal_tomb, int *lvl, char * utvonal_end)
{
	Data *iter, *iter2;
	int i,j = 0;
	char *tmp[MAX];
	char *tok = ";";
	int  words[MAX];
	char  buff[MAX];
	int ntokens = 0;
	int szint = *lvl;
	char * utvonal_p;

	char  *sep = strtok(utvonal_tomb, ";");

	if (strcmp(kisbetusito(honnan), kisbetusito(cel)) == 0)
	{
		return utvonal_tomb;
	}

	while (sep != NULL)
	{
		words[ntokens++] = atoi(sep);
		sep = strtok(NULL, ";");
	}



	for (i = 0; i < MAX; i++)
	{
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			if (words[ntokens-1] == iter->jaratszam)
			{
				for (iter2 = lista; iter2 != NULL; iter2 = iter2->kov)
				{

					if (array_contain(iter2->jaratszam, words,ntokens) == 0)
					{
						
						if (strcmp(kisbetusito(iter2->repter1), kisbetusito(iter->repter2)) == 0)
						{
							itoa(iter2->jaratszam, tmp, 10);
							for (j = 0; j < ntokens; j++)
							{
								itoa(words[j], utvonal_tomb, 10);
								utvonal_tomb = concat(utvonal_tomb, ";");
							}
							utvonal_tomb = concat(utvonal_tomb, tmp);

							if (strcmp(kisbetusito(iter2->repter2), kisbetusito(cel)) == 0){
								return utvonal_tomb;
								for (utvonal_p = strsep(utvonal_tomb, ";"); utvonal_p != NULL; utvonal_p = strsep(NULL, ";"))
								{
									if (strcmp(utvonal_p, utvonal_tomb) != 0)
									{
										strcpy(utvonal_end, utvonal_tomb);
										return utvonal_tomb;
									}
								}
							}
							else
							{
								if (szint == 0)
								{
									return utvonal_tomb;
									for (utvonal_p = strsep(utvonal_tomb, ";"); utvonal_p != NULL; utvonal_p = strsep(NULL, ";"))
									{
										if (strcmp(utvonal_p, utvonal_tomb) != 0)
										{
											strcpy(utvonal_end, utvonal_tomb);
											return utvonal_tomb;
										}
									}
								}
								szint=szint-1;
								strcpy(buff,utvonal(lista, honnan, cel, utvonal_tomb,&szint, utvonal_end));
								utvonal_tomb = concat(utvonal_tomb, ";");
								utvonal_tomb = concat(utvonal_tomb, buff);

								

								//return utvonal_tomb;
							}
						}
					}
					strcpy(utvonal_end, utvonal_tomb);//
				}
			}
		}
	}
}
/*void utvonal(Data *lista, char *honnan, char *hova, char * utvonal_tomb[])
{
	Data *iter;
	struct tm temptime;

	int i=0;

	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		if (strcmp(kisbetusito(iter->repter1),kisbetusito(honnan))==0)
		{
			itoa(iter->jaratszam, utvonal_tomb[i], 10);
			

			if (strcmp(kisbetusito(iter->repter2), kisbetusito(hova)) == 0)
			{
				return utvonal_tomb[i];
			}
			i++;
			
		}
	}

	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		for (i = 0; i < MAX; i++)
		{
			if (utvonal_tomb[i] == iter->jaratszam)
			{
				utvonal(iter, iter->repter1, hova, utvonal_tomb);
			}
		}
	}
}*/

//�tvonaltervez�s
void utvonaltervezes(Data *lista){
	char honnan[MAX];
	char hova[MAX];
	char seged[MAX];
	int max_atszallasok = 10;
	int *lvl;
	Data *iter, *iter2;
	char utvonal_tomb[MAX][MAX];
	char utvonal_end[MAX][MAX];
	int index = 0;
	int end[MAX];
	struct tm temptime;
	char temp[MAX];
	int i, j = 0;

	printf("Kerem adja meg az indul� orsz�g nev�t: ");
	getline(honnan, MAX - 1);
	printf("Kerem adja meg az �rkez� orsz�g nev�t: ");
	getline(hova, MAX - 1);
	do{
		printf("Kerem adja meg az �tsz�ll�sok maxim�lis sz�m�t (hagyja �resen, ha nincs kik�t�s): ");
		getline(seged, 9);	//Bek�rj�k a sz�mot, addig am�g nem eg�sz
		if (egesze(seged) != 1 || seged == "")
		{
			printf("Hib�s j�ratsz�m ! K�rem adja meg �jra !\n\n");
		}
	} while (egesze(seged) != 1 || seged == "");
	if (egesze(seged) == 1)
	{
		max_atszallasok = atoi(seged);
	}
	lvl = &max_atszallasok;


	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		if (strcmp(kisbetusito(honnan), kisbetusito(iter->repter1)) == 0){
			itoa(iter->jaratszam, utvonal_tomb[index], 10);
			index++;
		}
	}

	for (i = 0; i < index; i++)
	{
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			if (atoi(utvonal_tomb[i]) == iter->jaratszam)
			{
				strcpy(utvonal_end[i], utvonal(lista, iter->repter2, hova, utvonal_tomb[i], lvl, utvonal_end));

			}
		}
	}
	for (j = 0; j < index; j++)
	{
		printf("%s;\n", utvonal_end[j]);
	}

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