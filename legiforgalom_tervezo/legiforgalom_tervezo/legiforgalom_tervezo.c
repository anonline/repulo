#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define HSZ sizeof(int)/sizeof(short)*5  /* 5 v 10 jegyu max az int meretetol fuggoen */
#define MAX 128
#define ADATOK "repulo.csv"

//Struktúra
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

//Ékezetes betûk
void set_charset(){									//Ékezetes karakterek hálleluja
	system("chcp 1250");
	system("cls");
}

//Egésszám ellenõrzés
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

//Szöveg összefûzés
char* concat(char *s1, char *s2)
{
	char *result = malloc(strlen(s1) + strlen(s2) + 1);//+1 for the zero-terminator
	//in real code you would check for errors in malloc here
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

//Szövegbekérés
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

//Rendszeridõ lekérdezése
struct tm ido()
{
	time_t most_ennyi_ido_van = time(NULL);
	struct tm helyi_ido = *localtime(&most_ennyi_ido_van);
	return helyi_ido;
}

//Egy stringet (char[]-t) kisbetûssé alakít
char * kisbetusito(char str[])
{
	int i;

	for (i = 0; str[i]; i++){
		str[i] = tolower(str[i]);
	}
	return str;
}

//Megnézi egy int tömbben, hogy tartalmazza-e az aktuális elemet.
int array_contain(int val, int *arr, int size){
	int i;
	for (i = 0; i < size; i++) {
		if (arr[i] == val)
			return 1;
	}
	return 0;
}

//Megnézi egy char tömbben, hogy tartalmazza-e az aktuális elemet.
int char_array_int_contain(int val, char *arr, int size){
	int i;
	for (i = 0; i < size; i++) {
		if (atoi(arr) == val)
			return 1;
	}
	return 0;
}

//Billentyû buffer ürítés
void flush_in(FILE *in)
{
	int ch;

	do
		ch = fgetc(in);
	while (ch != EOF && ch != '\n');

	clearerr(in);
}

//Fájlellenõrzés
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

//Lita létrehozása
Data *listaletrehoz(){
	Data *l = NULL;
	int i,j,ora,perc,mp;
	int idotmp[2];
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

//Lista kiírás
void listakiir(Data *lista){

	Data *iter;

	for (iter = lista; iter != NULL; iter = iter->kov)
	{
		printf("Járatszám: [%d]\n", iter->jaratszam);
		printf("Honnan: [%s]\n", iter->repter1);
		printf("Hová: [%s]\n", iter->repter2);
		printf("Indulási idõ: [%d:%d]\n", iter->indulas.tm_hour, iter->indulas.tm_min);
		printf("Érkezési idõ: [%d:%d]\n", iter->erkezes.tm_hour, iter->erkezes.tm_min);
		printf("Idõeltolodás: [GMT %s%d]\n", (iter->idoeltolodas>0?"+":""),iter->idoeltolodas);
		printf("Indulási ország: [%s]\n", iter->repter1_country);
		printf("Érekzési ország: [%s]\n", iter->repter2_country);
		printf("Távolság: [%.2f km]\n", iter->tavolsag);
		printf("Osztályok: [%s]\n\n", iter->osztalyok);
	}
	printf("\n");
}

//Járatszámkeresés
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

//Reptérkeresés
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

//Osztálykeresés
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

//Országkeresés
void orszagkereses(Data *lista){
	
	char indule[MAX];													//Indulási vagy érkezési ország eldöntése
	char orszag[MAX];													//Maga az ország neve
	char tmp[2][MAX];													//kisbetusito fv használatához segédváltozo
	Data *iter;


	printf("Jelmagyarázat:\nI : Indulási ország\nE : Érkezési ország\n\n");
	
	do{																							//Bekér amíg nem i, vagy e
		printf("Kerem adja meg a keresett pozíciót: "); getline(indule, MAX - 1);
	} while (strcmp(kisbetusito(indule), "i") && strcmp(kisbetusito(indule), "e"));

	printf("Kerem adja meg az ország nevét: "); getline(orszag, MAX - 1);						//Bekéri az országot

	if (strcmp(indule, "i")==0)																	//Ha i akkor indulási országot keres és ír ki...
	{	
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			strcpy(tmp[0], iter->repter1_country);
			strcpy(tmp[1], orszag);

			if (strcmp(kisbetusito(tmp[0]), kisbetusito(tmp[1]))==0){
				printf("Járatszám: [%d]\n", iter->jaratszam);
				printf("Honnan: [%s]\n", iter->repter1);
				printf("Hová: [%s]\n", iter->repter2);
				printf("Indulási ország: [%s]\n", iter->repter1_country);
				printf("Érekzési ország: [%s]\n", iter->repter2_country);
				printf("Távolság: [%.2f km]\n", iter->tavolsag);
				printf("Osztályok: [%s]\n\n", iter->osztalyok);
			}
		}

	}
	else																						//Ha nem i akkor erkezesi országot keres és ír ki...
	{
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			strcpy(tmp[0], iter->repter2_country);
			strcpy(tmp[1], orszag);

			if (strcmp(kisbetusito(tmp[0]), kisbetusito(tmp[1])) == 0){
				printf("Járatszám: [%d]\n", iter->jaratszam);
				printf("Honnan: [%s]\n", iter->repter1);
				printf("Hová: [%s]\n", iter->repter2);
				printf("Indulási ország: [%s]\n", iter->repter1_country);
				printf("Érekzési ország: [%s]\n", iter->repter2_country);
				printf("Távolság: [%.2f km]\n", iter->tavolsag);
				printf("Osztályok: [%s]\n\n", iter->osztalyok);
			}
		}	
	}


}

//Idõpontkeresés
void idopontkereses(Data *lista)
{
	int i, userhour, usermin, ok = 0;
	char indule[MAX];
	char idopont[MAX];
	Data *iter;

	printf("Jelmagyarázat:\nI : Indulási idõ\nE : Érkezési idõ\n\n");

	do{																							//Bekér amíg nem i, vagy e
		printf("Kerem adja meg a keresett pozíciót: "); getline(indule, MAX - 1);
	} while (strcmp(kisbetusito(indule), "i") && strcmp(kisbetusito(indule), "e"));

	do{
		fflush(stdin);
		ok = 0;
		printf("Kerem adja meg az idõpontot: "); getline(idopont, MAX - 1);

	} while (sscanf_s(idopont, "%d:%d", &userhour, &usermin) != 2);

	if (strcmp(indule, "i") == 0)																	//Ha i akkor indulási országot keres és ír ki...
	{
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			if (iter->indulas.tm_hour >= userhour && iter->indulas.tm_min >= usermin){
				printf("Járatszám: [%d]\n", iter->jaratszam);
				printf("Honnan: [%s]\n", iter->repter1);
				printf("Hová: [%s]\n", iter->repter2);
				printf("Indulási idõ: [%d:%d]\n", iter->indulas.tm_hour, iter->indulas.tm_min);
				printf("Érkezési idõ: [%d:%d]\n", iter->erkezes.tm_hour, iter->erkezes.tm_min);
				printf("Idõeltolodás: [GMT %s%d]\n", (iter->idoeltolodas>0 ? "+" : ""), iter->idoeltolodas);
				printf("Indulási ország: [%s]\n", iter->repter1_country);
				printf("Érekzési ország: [%s]\n", iter->repter2_country);
				printf("Távolság: [%.2f km]\n", iter->tavolsag);
				printf("Osztályok: [%s]\n\n", iter->osztalyok);
			}
		}

	}
	else																						//Ha nem i akkor erkezesi országot keres és ír ki...
	{
		for (iter = lista; iter != NULL; iter = iter->kov)
		{
			if (iter->erkezes.tm_hour >= userhour && iter->erkezes.tm_min >= usermin){
				printf("Járatszám: [%d]\n", iter->jaratszam);
				printf("Honnan: [%s]\n", iter->repter1);
				printf("Hová: [%s]\n", iter->repter2);
				printf("Indulási idõ: [%d:%d]\n", iter->indulas.tm_hour, iter->indulas.tm_min);
				printf("Érkezési idõ: [%d:%d]\n", iter->erkezes.tm_hour, iter->erkezes.tm_min);
				printf("Idõeltolodás: [GMT %s%d]\n", (iter->idoeltolodas>0 ? "+" : ""), iter->idoeltolodas);
				printf("Indulási ország: [%s]\n", iter->repter1_country);
				printf("Érekzési ország: [%s]\n", iter->repter2_country);
				printf("Távolság: [%.2f km]\n", iter->tavolsag);
				printf("Osztályok: [%s]\n\n", iter->osztalyok);
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

//Útvonaltervezés
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

	printf("Kerem adja meg az induló ország nevét: ");
	getline(honnan, MAX - 1);
	printf("Kerem adja meg az érkezõ ország nevét: ");
	getline(hova, MAX - 1);
	do{
		printf("Kerem adja meg az átszállások maximális számát (hagyja üresen, ha nincs kikötés): ");
		getline(seged, 9);	//Bekérjük a számot, addig amíg nem egész
		if (egesze(seged) != 1 || seged == "")
		{
			printf("Hibás járatszám ! Kérem adja meg újra !\n\n");
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

//Fõprogram
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
		printf("\n\nNyomj egy <Enter>-t a menübe való visszatéréshez...");
		getchar();
		system("cls");
	} while (1 == 1);
}