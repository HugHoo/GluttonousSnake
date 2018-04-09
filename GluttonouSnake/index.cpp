#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <conio.h>
#include "blind_you.h"
#include "snake.h"
#define titleM 7		//标题高度
#define titleN 29		//标题宽度
#define spaceM 10		//标题左空行
#define spaceN 1		//标题上空行
#define option_height 4		//选项个数
#define optionM 35		//选项左空行
#define optionN 11		//选项上空行
#define smileM 33		//笑脸初始坐标x
#define smileN 11		//笑脸初始坐标y
using namespace std;

int print_title();		//打印游戏标题SNAKE
int gotoxy(int, int);		//定位光标坐标
int print_option();		//打印选项
int BGcolor();		//背景及字体颜色
int UI();				//界面UI
int smile(int x, int y);		//小笑脸
int up();			//笑脸向上移动
int down();			//笑脸向下移动
int judge();		//判断玩家操作
int enter();		//进入所选项
int border();		//边框

int smile_x = smileM;		//笑脸坐标x
int smile_y = smileN;		//笑脸坐标y
int quit = 1;			//是否退出游戏，1为不退出，0为退出

int title[titleM][titleN] = { { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
{ 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1 },
{ 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0 },
{ 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1 },
{ 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0 },
{ 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1 }
};

int main()
{
	UI();
	while (quit)
	{
		smile(smile_x, smile_y);
		gotoxy(0, 23);
		judge();
	}
	return 0;
}

int UI()		//界面UI
{
	BGcolor();
	print_title();
	border();
	print_option();
	return 0;
}

int BGcolor()		//颜色
{
	system("color 73");
	return 0;
}

int gotoxy(int x, int y)			//移动光标到指定坐标
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hConsole, pos);
	return 0;
}

int print_title()		//打印标题SNAKE
{
	for (int i = 0; i<titleM; i++)
	{
		gotoxy(spaceM, spaceN + i);
		for (int j = 0; j<titleN; j++)
		{
			if (title[i][j] == 0)
				printf("  ");
			else
				printf("%c%c", 0xa8, 0x80);
		}
	}
	return 0;
}

int border()		//边框
{
	gotoxy(8, 1);
	for (int i = 0; i<60; i++)
	{
		cout << "=";
	}
	for (int i = 0; i<18; i++)
	{
		gotoxy(8, 2 + i);
		cout << "|";
		gotoxy(67, 2 + i);
		cout << "|";
	}
	gotoxy(8, 20);
	for (int i = 0; i<60; i++)
	{
		cout << "=";
	}
	return 0;
}

int print_option()		//打印选项
{
	gotoxy(optionM, optionN);
	cout << "开始游戏";
	gotoxy(optionM, optionN + 2);
	cout << "选项";
	gotoxy(optionM, optionN + 4);
	cout << "Blind you";
	gotoxy(optionM, optionN + 6);
	cout << "退出";
	return 0;
}

int smile(int x, int y)		//小笑脸
{
	gotoxy(x, y);
	printf("\1");
	return 0;
}

int judge()		//判断键盘选择
{
	char m;
	m = _getch();
	switch (m)
	{
	case 'w':up(); break;
	case 's':down(); break;
	case '\r':enter(); break;
	case 'W':up(); break;
	case 'S':down(); break;
	default:break;
	}
	return 0;
}

int up()		//笑脸向上移动
{
	gotoxy(smile_x, smile_y);
	cout << " ";
	smile_y -= 2;
	if (smile_y<smileN)
		smile_y = smileN + 6;
	return smile_y;
}

int down()		//笑脸向下移动
{
	gotoxy(smile_x, smile_y);
	cout << " ";
	smile_y += 2;
	if (smile_y>smileN + 6)
		smile_y = smileN;
	return smile_y;
}

int enter()		//进入所选项
{
	switch (smile_y)
	{
	case smileN:system("cls"); snake(); UI(); break;
	case smileN + 2:cout << 2; break;
	case smileN + 4:system("cls"); smile_y = blind(); UI(); break;
	case smileN + 6:quit = 0; break;
	default:break;
	}
	return 0;
}