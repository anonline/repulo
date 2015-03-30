#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <Windows.h>

int menu()
{
	HANDLE handler = GetStdHandle(STD_OUTPUT_HANDLE);
	WORD regi_szin_attr;
	CONSOLE_SCREEN_BUFFER_INFO console_backed_Info;

	char fomenu[8][1024];
	int i;
	int code = 0;
	int end=0;
	int precheck = 0;

	strcpy(fomenu[0], "Útvonaltervezés");
	strcpy(fomenu[1], "Jegyár kalkulátor");
	strcpy(fomenu[2], "Reptér keresése");
	strcpy(fomenu[3], "Ország keresése");
	strcpy(fomenu[4], "Idõ alapú keresés");
	strcpy(fomenu[5], "Járatszám keresése");
	strcpy(fomenu[6], "Osztályok keresése");
	strcpy(fomenu[7], "Kilépés");

	GetConsoleScreenBufferInfo(handler, &console_backed_Info);
	regi_szin_attr = console_backed_Info.wAttributes;

	while (1==1)
	{
		for (i = 0; i < 8; i++)
		{
			if (i == end)
			{
				SetConsoleTextAttribute(handler, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
			}
			else
			{
				SetConsoleTextAttribute(handler, regi_szin_attr);
			}

			cprintf("%d. %s\n", (i + 1), fomenu[i]);

		}
		code = getch();

		if (code == 13)		/*Enter*/
		{
			return end + 1;
		}
		if (code == 224)
		{
			precheck = 1;
		}
		if (precheck == 1)
		{
			if (code == 72)
			{
				precheck = 0;
				end = (end == 0) ? 7 : (end-1);
			}
			else if (code == 80)
			{
				precheck = 0;
				end = (end == 7) ? 0 : (end+1);
			}
		}

		system("cls");
		
	}
}

