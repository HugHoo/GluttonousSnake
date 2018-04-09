#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <time.h>
#define MARGIN_X 27		//空间上边框与控制台上边框距离
#define MARGIN_Y 4		//空间左边框与控制台左边框距离
#define SPACE_X 10		//空间宽度
#define SPACE_Y 10		//空间高度
#define INITIAL_LENGTH 3	//蛇身初始长度
#define NUM WIN+3		//蛇身最长长度
#define FOOD_NUM WIN	//最多消化的食物为
#define WIN 20				//胜利条件：吃WIN个食物
#define SPEED Sleep(300);		//蛇运动速度
#define ESC 27			//退出ESC定义
using namespace std;

int snake_border();				//边界
int snake_gotoxy(int, int);		//移动光标坐标
int snake_initial_position();		//蛇坐标初始化
int snake_judge();		//判断玩家键盘输入
int snake_move();		//蛇移动
int snake_update(int m, int n);		//蛇更新
bool die();					//判断蛇是否死亡
int snake_food();			//随机产生食物
bool is_repeat();			//食物坐标是否与蛇坐标重叠，若重叠，则重新产生食物
int sanke_ex_food();			//消化的食物向后推
int snake_ex_food_destroy();		//消化的食物向前推
int dig_snake();			//正在消化的蛇节荧光
bool grow();			//成长，拉出一节		
int win();				//胜利
int lose();				//屎了
int score_board();		//得分板
int score_board_2();	//得分板数据
int snake_pause();		//空格暂停
int count_second();		//数秒
int snake();

HANDLE hSnakeConsole = GetStdHandle(STD_OUTPUT_HANDLE);		//获取控制台输出句柄

int snake_data[NUM][2];		//蛇节坐标			//**************之前的一维写成了INITIAL_NUM，导致后边蛇变长后，数组越界，小心********
int length = INITIAL_LENGTH;		//蛇身初始化
char ch = 'd', ch_temp = 'd';		//初始化方向
int die_or_not = 1;				//1为没死，0为死
int food[2] = { 0 };				//食物坐标初始化
int ex_food[FOOD_NUM][2] = { 0 };		//消化的食物坐标初始化
int dig = -1;						//胃里消化的食物
int score = 0;				//得分，也就是吃多少
int esc = 0;					//判断是否退出
int tempX, tempY;
int pause = 0;

int snake()
{
	system("color 73");
	snake_border();
	score_board();
	count_second();
	snake_initial_position();
	snake_food();
	while (snake_judge())
	{
		score_board_2();
		snake_move();
	}
	if (esc == 1)
	{
		system("cls");
		return 0;
	}
	if (score == WIN)
		win();
	else
		lose();
	snake_gotoxy(0, 23);
	system("cls");
	return 0;
}

int snake_gotoxy(int x, int y)			//光标坐标移动
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	SetConsoleCursorPosition(hSnakeConsole, pos);
	return 0;
}

int snake_border()			//边框
{
	snake_gotoxy(MARGIN_X, MARGIN_Y);
	for (int i = 0; i<SPACE_X + 2; i++)
		printf("□");
	snake_gotoxy(MARGIN_X, MARGIN_Y + SPACE_Y + 1);
	for (int i = 0; i<SPACE_X + 2; i++)
		printf("□");
	for (int i = 0; i<SPACE_Y; i++)
	{
		snake_gotoxy(MARGIN_X, MARGIN_Y + 1 + i);
		printf("□");
		snake_gotoxy(MARGIN_X + 2 * SPACE_X + 2, MARGIN_Y + 1 + i);
		printf("□");
	}
	return 0;
}

int snake_initial_position()			//蛇身坐标初始化
{
	for (int i = 0; i<INITIAL_LENGTH; i++)
	{
		snake_data[i][0] = MARGIN_X + INITIAL_LENGTH * 2 - 2 * i;
		snake_data[i][1] = MARGIN_Y + 1;
	}
	for (int i = 0; i<length; i++)
	{
		snake_gotoxy(snake_data[i][0], snake_data[i][1]);
		printf("■");
	}
	return 0;
}

int snake_judge()			//判断玩家操作
{
	if (_kbhit())
	{
		ch_temp = ch;
		ch = _getch();
	}
	snake_pause();
	switch (ch)
	{
	case 'w':if (ch_temp != 's')snake_update(0, -1); else snake_update(0, 1); break;
	case 's':if (ch_temp != 'w')snake_update(0, 1); else snake_update(0, -1); break;
	case 'a':if (ch_temp != 'd')snake_update(-2, 0); else snake_update(2, 0); break;
	case 'd':if (ch_temp != 'a')snake_update(2, 0); else snake_update(-2, 0); break;
	case ESC:esc = 1; break;
	default:break;
	}
	if (!die())
		return 0;
	return 1;
}

int snake_pause()
{
	int out = 1;
	if (ch == ' ')
	{
		snake_gotoxy(34, 17);
		cout << "==========";
		snake_gotoxy(34, 19);
		cout << "==========";
		snake_gotoxy(34, 18);
		cout << "|";
		snake_gotoxy(43, 18);
		cout << "|";
		if (pause)
		{
			snake_gotoxy(36, 18);
			cout << " 暂停 ";
		}
		else
		{
			snake_gotoxy(36, 18);
			cout << "你屎了";
			Sleep(1700);
			snake_gotoxy(36, 18);
			cout << " 呵呵 ";
			Sleep(500);
			snake_gotoxy(36, 18);
			cout << "开玩笑";
			Sleep(500);
			snake_gotoxy(36, 18);
			cout << " 暂停 ";
			pause++;
		}
		while (out)
		{
			ch = _getch();
			if (ch == ' ')
			{
				out = 0;
				switch (ch_temp)
				{
				case 'w':ch = 's'; break;
				case 's':ch = 'w'; break;
				case 'a':ch = 'd'; break;
				case 'd':ch = 'a'; break;
				}
				snake_gotoxy(34, 17);
				cout << "          ";
				snake_gotoxy(34, 19);
				cout << "          ";
				snake_gotoxy(34, 18);
				cout << "          ";
			}
			else if (ch == ESC)
			{
				out = 0;
				esc = 1;
			}
		}
	}
	return 0;
}

bool die()			//是否撞墙或者吃到自己
{
	if (snake_data[0][0] <= MARGIN_X + 1 || snake_data[0][0]>MARGIN_X + 1 + 2 * SPACE_X)
		return 0;
	if (snake_data[0][1] <= MARGIN_Y || snake_data[0][1]>MARGIN_Y + SPACE_Y)
		return 0;
	for (int i = 1; i<length; i++)
	{
		if (snake_data[0][0] == snake_data[i][0] && snake_data[0][1] == snake_data[i][1])
			return 0;
	}
	if (esc == 1)
		return 0;
	if (score == WIN)
		return 0;
	return 1;
}

int snake_update(int m, int n)			//蛇身坐标更新
{
	if (grow())		//********错误点(解决)*****
	{
		length++;
		snake_ex_food_destroy();
	}
	else
	{
		tempX = snake_data[length - 1][0];
		tempY = snake_data[length - 1][1];
	}
	for (int i = length - 1; i>0; i--)
	{
		snake_data[i][0] = snake_data[i - 1][0];
		snake_data[i][1] = snake_data[i - 1][1];
	}
	snake_data[0][0] += m;
	snake_data[0][1] += n;
	return 0;
}

bool grow()
{
	for (int i = 0; i<dig + 1; i++)
	{
		if (snake_data[length - 1][0] == ex_food[i][0] && snake_data[length - 1][1] == ex_food[i][1])
			return 1;
	}
	return 0;
}

int snake_move()			//蛇移动
{
	snake_gotoxy(tempX, tempY);
	printf("  ");
	dig_snake();
	snake_gotoxy(snake_data[0][0], snake_data[0][1]);
	printf("■");
	if (snake_data[0][0] == food[0] && snake_data[0][1] == food[1])
	{
		dig++;
		score++;
		sanke_ex_food();
		snake_food();
	}
	snake_gotoxy(0, 23);
	SPEED
		return 0;
}

int dig_snake()		//		判断是否是正在消化的蛇节
{
	for (int i = 0; i<dig + 1; i++)
	{
		snake_gotoxy(ex_food[i][0], ex_food[i][1]);
		SetConsoleTextAttribute(hSnakeConsole, 123);
		printf("■");
		SetConsoleTextAttribute(hSnakeConsole, 115);
	}
	return 0;
}

int sanke_ex_food()		//食物数据向后推
{
	for (int i = dig; i>0; i--)
	{
		ex_food[i][0] = ex_food[i - 1][0];
		ex_food[i][1] = ex_food[i - 1][1];
	}
	ex_food[0][0] = snake_data[0][0];
	ex_food[0][1] = snake_data[0][1];
	return 0;
}

int snake_ex_food_destroy()			//食物数据向前推，也就是销毁第一个数据
{
	for (int i = 0; i<dig + 1; i++)
	{
		ex_food[i][0] = ex_food[i + 1][0];
		ex_food[i][1] = ex_food[i + 1][1];
	}
	dig--;
	return 0;
}

int snake_food()			//随机产生食物
{
	srand(time(NULL));
	do
	{
		food[0] = (rand() % SPACE_X) * 2 + MARGIN_X + 2;
		food[1] = rand() % SPACE_Y + MARGIN_Y + 1;
	} while (is_repeat());			//*********之前是is_repeat，没有加上“()”,- -||************
	snake_gotoxy(food[0], food[1]);
	printf("◎");
	return 0;
}

bool is_repeat()		//0表示没有重复
{
	for (int i = 0; i<length; i++)
	{
		if (food[0] == snake_data[i][0] && food[1] == snake_data[i][1])
			return 1;
	}
	return 0;
}

int win()
{
	snake_gotoxy(34, 7);
	cout << "==========";
	snake_gotoxy(34, 9);
	cout << "==========";
	snake_gotoxy(34, 8);
	cout << "|";
	snake_gotoxy(43, 8);
	cout << "|";
	snake_gotoxy(36, 8);
	cout << " 胜利!";
	snake_gotoxy(0, 23);
	Sleep(2000);
	return 0;
}

int lose()
{
	snake_gotoxy(34, 7);
	cout << "==========";
	snake_gotoxy(34, 9);
	cout << "==========";
	snake_gotoxy(34, 8);
	cout << "|";
	snake_gotoxy(43, 8);
	cout << "|";
	snake_gotoxy(36, 8);
	cout << "你屎了";
	snake_gotoxy(0, 23);
	Sleep(2000);
	return 0;
}

int score_board()
{
	snake_gotoxy(58, 4);
	cout << "当前蛇长度：";
	snake_gotoxy(58, 6);
	cout << "距离胜利还剩：";
	snake_gotoxy(58, 8);
	cout << "按";
	SetConsoleTextAttribute(hSnakeConsole, 123);
	cout << "ESC";
	SetConsoleTextAttribute(hSnakeConsole, 115);
	cout << "退出";
	snake_gotoxy(58, 10);
	cout << "按";
	SetConsoleTextAttribute(hSnakeConsole, 123);
	cout << "空格";
	SetConsoleTextAttribute(hSnakeConsole, 115);
	cout << "暂停";
	snake_gotoxy(58, 12);
	cout << "上下左右分";
	snake_gotoxy(58, 13);
	cout << "别为w,s,a,d";
	snake_gotoxy(58, 15);
	cout << "若蛇未移动，请关闭";
	snake_gotoxy(58, 16);
	SetConsoleTextAttribute(hSnakeConsole, 123);
	cout << "大写";
	SetConsoleTextAttribute(hSnakeConsole, 115);
	cout << "或";
	SetConsoleTextAttribute(hSnakeConsole, 123);
	cout << "输入法";
	SetConsoleTextAttribute(hSnakeConsole, 115);
	return 0;
}

int score_board_2()
{
	snake_gotoxy(70, 4);
	cout << length;
	snake_gotoxy(72, 6);
	cout << WIN - score << " ";
	return 0;
}

int count_second()
{
	snake_gotoxy(35, 6);
	printf("■");
	printf("■");
	printf("■");
	printf("■");
	snake_gotoxy(41, 7);
	printf("■");
	snake_gotoxy(41, 8);
	printf("■");
	snake_gotoxy(35, 9);
	printf("■");
	printf("■");
	printf("■");
	printf("■");
	snake_gotoxy(41, 10);
	printf("■");
	snake_gotoxy(41, 11);
	printf("■");
	snake_gotoxy(35, 12);
	printf("■");
	printf("■");
	printf("■");
	printf("■");
	Sleep(1000);
	snake_gotoxy(35, 10);
	printf("■");
	snake_gotoxy(35, 11);
	printf("■");
	snake_gotoxy(41, 10);
	printf("  ");
	snake_gotoxy(41, 11);
	printf("  ");
	Sleep(1000);
	for (int i = 0; i<7; i++)
	{
		snake_gotoxy(35, 6 + i);
		printf("          ");
	}
	for (int i = 0; i<7; i++)
	{
		snake_gotoxy(38, 6 + i);
		printf("■");
	}
	Sleep(1000);
	for (int i = 0; i<7; i++)
	{
		snake_gotoxy(35, 6 + i);
		printf("          ");
	}
	Sleep(300);
	return 0;
}