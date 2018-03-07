#include <iostream>
#include <Windows.h>
#include <conio.h>
#include "blind_you.h"
#define SPEED Sleep(20);
using namespace std;

int white();
int black();

int blind()
{
	int no_quit = 1;
	cout << endl << endl << "\n\n\n\n\n\n\n			  	  Press any button\n\n\n\n\n";
	getchar();
	system("cls");
	cout << "\n\n\n\n\n\n\n\n\n			    Blind your eyes!!!";
	while (no_quit)
	{
		if (_kbhit())
			no_quit = 0;
		white();
		SPEED
			black();
		SPEED
	}
	system("cls");
	return 15;
}

int white()
{
	system("color F0");
	return 0;
}

int black()
{
	system("color 0F");
	return 0;
}
