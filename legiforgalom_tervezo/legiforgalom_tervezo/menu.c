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

	char fomenu[6][1024];
	int i;
	int code = 0;
	int end=0;
	int precheck = 0;

	strcpy(fomenu[0], "�tvonaltervez�s (m�g listakiir())");
	strcpy(fomenu[1], "Jegy�r kalkul�tor");
	strcpy(fomenu[2], "Rept�r keres�se");
	strcpy(fomenu[3], "J�ratsz�m keres�se");
	strcpy(fomenu[4], "Oszt�lyok keres�se");
	strcpy(fomenu[5], "Kil�p�s");

	GetConsoleScreenBufferInfo(handler, &console_backed_Info);
	regi_szin_attr = console_backed_Info.wAttributes;

	while (1==1)
	{
		for (i = 0; i < 6; i++)
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
				end = (end == 0) ? 5 : (end-1);
			}
			else if (code == 80)
			{
				precheck = 0;
				end = (end == 5) ? 0 : (end+1);
			}
		}

		system("cls");
		
	}
}

