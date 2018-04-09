#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#define FORM_SIZE 3
#define ROW 23                  //运动空间——行数
#define COLUMN 14               //运动空间——列数
#define RANDOM_TRANSLATION 7        //center横坐标随机产生4种
#define ORDINATE 2              //center纵坐标初始值
#define DISPLACEMENT 3          //center横坐标平移
#define TEMP 1
#define WIN 15                  //消去WIN行胜利
#define DIE_LINE 1              //到DIE_LINE行数屎
#define UPDATE_TIME 40          //更新时间
#define SPACE  "  "             
#define SQURE "■"
using namespace std;

COORD far_away = { 0,24 };
int time_down = 0;
int scoure=0;                   //得分

enum color{blue=121,green=114,lake_blue=115,red=124,gray=120,pink=117,yellow=126};      //enum颜色

struct squre{                   //声明方块结构
    int state;
    COORD pos[FORM_SIZE];
    COORD center;
    int form;
    int color;
};

static struct fixed{
    short fix[ROW][COLUMN];     //记录空间中每个坐标的状态，0为无，1为存在squre
    int color[ROW][COLUMN];
};

struct fixed initial_fixed(struct fixed * fixed);                   //初始化fixed方块
inline void gotoxy(COORD pos);                                      //移动光标到pos位置
void print_squre(struct squre * form, char ch[]);                   //打印方块
void print_fixed(struct fixed * fixed);                             //打印fixed的方块
void move_squre(struct squre * form, short xdir, short ydir);       //x轴移动xdir个单位，y轴移动ydir个单位
squre create_squre();                                               //创造方块
static squre form1();                                               //7种类型的方块
static squre form2();                                               
static squre form3();                                               
static squre form4();
static squre form5();
static squre form6();
static squre form7();
squre turn(squre * pform);                                          //方块转动
bool turn_success(struct squre * form, struct fixed * fixed);       //是否能够转动成功
static inline COORD tempXY();                                       
bool is_buttom(struct squre * form, struct fixed * fixed);                  //判断是否到达底部
bool is_wall(struct squre * form, struct fixed * fixed, short dir);         //判断是否碰到墙壁/fixed的方块
struct fixed fix_squre(struct squre * form, struct fixed * fixed);          //fixed方块
static bool check_full(const struct fixed * fixed, const int line);         //判断是否存在满行
static bool is_blank(const struct fixed * fixed, const short line);             //是否是空白行
static bool is_in_line(const short y, const short line[], const short t);       
static void update_fixed(struct fixed * fixed, short line[], short t);          //更新fixed的方块
void eliminate(struct fixed * fixed, struct squre * form);                  //消行
bool is_die(struct fixed * fixed);                                          //判断是否死亡
void add_one(short line, char ch[]);                                        
void scoure_board();                                                        //得分板
void print_scoure();                                                        //打印分数
void print_next(const struct squre * form);
void time_board();                                                          //时间板

HANDLE hSqureConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO bInfo;

int main()
{
    GetConsoleScreenBufferInfo(hSqureConsole, &bInfo);
    COORD size = { 70, 25 };
    SetConsoleScreenBufferSize(hSqureConsole, size);
    SMALL_RECT rc = { 0, 0, 60, 23 };                                   //不能大于缓冲区大小
    SetConsoleWindowInfo(hSqureConsole, true, &rc);
    system("color 73");
    struct fixed fixed;
    initial_fixed(&fixed);
    print_fixed(&fixed);

    scoure_board();

    struct squre form_temp = create_squre();
    time_board();

    int is_out=0;
    while (!is_out)
    {
        //Sleep(11);
        fflush(stdin);
        struct squre form = form_temp;
        form_temp = create_squre();
        print_squre(&form, SQURE);

        print_next(&form_temp);

        bool isout = 0;
        while (!isout)
        {
            char ch;
            if (_kbhit())
            {
                ch = _getch();
                switch (ch)
                {
                case 72:
                    if (!turn_success(&form, &fixed))
                    {
                        print_squre(&form, SPACE);
                        turn(&form);
                        print_squre(&form, SQURE);
                    }
                    break;
                case 75:
                    if (!is_wall(&form, &fixed, -1))
                    {
                        move_squre(&form, -1, 0);
                    }
                    break;
                case 77:
                    if (!is_wall(&form, &fixed, +1))
                    {
                        move_squre(&form, +1, 0);
                    }
                    break;
                case 80:
                    if (!is_buttom(&form, &fixed))
                    {
                        move_squre(&form, 0, +1);
                    }
                    break;
                default :
                    break;
                }
            }
            Sleep(15);
            if (0 == ++time_down % 15)
            {
                if (is_buttom(&form, &fixed))
                {
                    fix_squre(&form, &fixed);
                    eliminate(&fixed, &form);
                    isout = 1;
                    if (scoure >= WIN || is_die(&fixed))
                        is_out = 1;
                }
                else
                    move_squre(&form, 0, +1);
            }
        }
    }
}

bool is_die(struct fixed * fixed)
{
    for (int i = 1; i < COLUMN - 1; i++)
    {
        if (fixed->fix[DIE_LINE][i] == 1)
            return 1;
    }
    return 0;
}

inline void gotoxy(COORD pos)
{
    pos.X *= 2;
    SetConsoleCursorPosition(hSqureConsole, pos);
}

void move_squre(struct squre * form,short xdir,short ydir)
{
    print_squre(form, SPACE);
    form->center.X += xdir;
    form->center.Y += ydir;
    for (int i = 0; i < FORM_SIZE; i++)
    {
        form->pos[i].X += xdir;
        form->pos[i].Y += ydir;
    }
    print_squre(form, SQURE);

}

bool is_buttom(struct squre * form, struct fixed * fixed)
{
    short x, y;
    y = form->center.X;             //**********     COORD的X与Y 和 二维数组的一维二维要分清
    x = form->center.Y;             //**********     这里X赋给y,Y赋给x，体现COORD与数组的一二维的区别
    if (fixed->fix[x+1][y])
        return 1;
    for (int i = 0; i < FORM_SIZE; i++)
    {
        y = form->pos[i].X;         //同理↑↑
        x = form->pos[i].Y;
        if (fixed->fix[x+1][y])
            return 1;
    }
    return 0;
}

bool is_wall(struct squre * form, struct fixed * fixed,short dir)
{
    short x, y;
    x = form->center.X;
    y = form->center.Y;
    if (fixed->fix[y][x + dir])
        return 1;
    for (int i = 0; i < FORM_SIZE; i++)
    {
        x = form->pos[i].X;
        y = form->pos[i].Y;
        if (fixed->fix[y][x + dir])
            return 1;
    }
    return 0;
}

struct fixed fix_squre(struct squre * form, struct fixed * fixed)
{
    COORD pos;
    pos.X = form->center.X;
    pos.Y = form->center.Y;
    fixed->fix[pos.Y][pos.X] = 1;
    fixed->color[pos.Y][pos.X] = form->color;
    for (int i = 0; i < FORM_SIZE; i++)
    {
        pos.X = form->pos[i].X;
        pos.Y = form->pos[i].Y;
        fixed->fix[pos.Y][pos.X] = 1;
        fixed->color[pos.Y][pos.X] = form->color;
    }
    return *fixed;
}

struct fixed  initial_fixed(struct fixed * fixed)
{
    for (int i = 0; i < ROW; i++)
    {
        for (int j = 0; j < COLUMN; j++)
        {
            if (j == 0 || j == COLUMN - 1 || i == ROW - 1)
                fixed->fix[i][j] = 1;
            else
                fixed->fix[i][j] = 0;
        }
    }
    return *fixed;
}

void print_fixed(struct fixed * fixed)
{
    COORD pos;
    pos.X = 1;
    pos.Y = ROW-1;
    gotoxy(pos);
    for (int i = 1; i < COLUMN-1; i++)
        cout << "￣";
}

squre turn(squre * pform)
{
    if (pform->form != 1)
    {
        int tempX;
        for (int i = 0; i < 3; i++)
        {
            pform->pos[i].X -= pform->center.X;
            //pform->pos[i].X /= 2;
            pform->pos[i].Y -= pform->center.Y;
            tempX = pform->pos[i].X;
            pform->pos[i].X = (short)((-pform->pos[i].Y)*(pform->state) + pform->center.X);
            pform->pos[i].Y = (short)(tempX*(pform->state) + pform->center.Y);
        }
        if (pform->form == 2)
        {
            pform->state *= -1;
        }
    }
    return *pform;
}

bool turn_success(struct squre * form, struct fixed * fixed)
{
    struct squre form_temp = *form;
    turn(&form_temp);
    short x, y;
    x = form_temp.center.X;
    y = form_temp.center.Y;
    if (fixed->fix[y][x])
        return 1;
    for (int i = 0; i < FORM_SIZE; i++)
    {
        x = form_temp.pos[i].X;
        y = form_temp.pos[i].Y;
        if (fixed->fix[y][x])
            return 1;
    }
    return 0;
}

void print_squre(struct squre * form,char ch[])
{
    COORD pos;
    pos.X = form->center.X;
    pos.Y = form->center.Y;
    gotoxy(pos);
    SetConsoleTextAttribute(hSqureConsole, form->color);
    fputs(ch, stdout);
    for (int i = 0; i < FORM_SIZE; i++)
    {
        pos.X = form->pos[i].X;
        pos.Y = form->pos[i].Y;
        gotoxy(pos);
        fputs(ch, stdout);
    }
}

squre create_squre()        //产生随机squre类型
{
    srand(time(NULL));
    short random = rand() % 7;
    typedef squre (*random_squre)();
    random_squre psqure[7] = {
        form1, form2, form3, form4, form5, form6, form7
    };
    return psqure[random]();
}

static squre form1()        //类型1：L型
{
    COORD temp = tempXY();
    squre form{
        1,
        { { temp.X - TEMP, temp.Y - 1 }, { temp.X - TEMP, temp.Y }, { temp.X + TEMP, temp.Y } },
        { temp.X, temp.Y },
        4,
        pink
    };

    srand(time(NULL));
    short t = rand() % form.form;
    while (t--)
    {
        turn(&form);
    }
    return form;
}

static squre form2()        //类型2：L型镜像
{
    COORD temp = tempXY();
    squre form{
        1,
        { { temp.X + TEMP, temp.Y - 1 }, { temp.X - TEMP, temp.Y }, { temp.X + TEMP, temp.Y } },
        { temp.X, temp.Y },
        4,
        gray
    };

    srand(time(NULL));
    short t = rand() % form.form;
    while (t--)
    {
        turn(&form);
    }
    return form;
}

static squre form3()        //类型：T型
{
    COORD temp = tempXY();
    squre form{
        1,
        { { temp.X, temp.Y - 1 }, { temp.X - TEMP, temp.Y }, { temp.X + TEMP, temp.Y } },
        { temp.X, temp.Y },
        4,
        yellow
    };

    srand(time(NULL));
    short t = rand() % form.form;
    while (t--)
    {
        turn(&form);
    }
    return form;
}

static squre form4()        //类型4：ㄣ型
{
    COORD temp = tempXY();
    squre form{
        1,
        { { temp.X, temp.Y - 1 }, { temp.X - TEMP, temp.Y }, { temp.X - TEMP, temp.Y + 1 } },
        { temp.X, temp.Y },
        2,
        green
    };

    srand(time(NULL));
    short t = rand() % form.form;
    while (t--)
    {
        turn(&form);
    }
    return form;
}

static squre form5()        //类型5：ㄣ型镜像
{
    COORD temp = tempXY();
    squre form{
        1,
        { { temp.X - TEMP, temp.Y - 1 }, { temp.X - TEMP, temp.Y }, { temp.X, temp.Y + 1 } },
        { temp.X, temp.Y },
        2,
        lake_blue
    };

    srand(time(NULL));
    short t = rand() % form.form;
    while (t--)
    {
        turn(&form);
    }
    return form;
}

static squre form6()        //类型6：|型
{
    COORD temp = tempXY();
    squre form{
        1,
        { { temp.X, temp.Y - 2 }, { temp.X, temp.Y - 1 }, { temp.X, temp.Y + 1 } },
        { temp.X, temp.Y },
        2,
        red
    };

    srand(time(NULL));
    short t = rand() % form.form;
    while (t--)
    {
        turn(&form);
    }
    return form;
}

static squre form7()        //类型7：■型
{
    COORD temp = tempXY();
    squre form{
        1,
        { { temp.X - TEMP, temp.Y - 1 }, { temp.X - TEMP, temp.Y }, { temp.X, temp.Y - 1 } },
        { temp.X, temp.Y },
        1,
        blue
    };
    return form;
}

static inline COORD tempXY()
{
    COORD temp;
    srand(time(NULL));
    temp.X = rand() % RANDOM_TRANSLATION + DISPLACEMENT;
    temp.Y = ORDINATE;
    return temp;
}

static bool check_full(const struct fixed * fixed, const int line)      //检查是否满行 no problem
{
    for (int i = 1; i < COLUMN - 1; i++)
    {
        if (!fixed->fix[line][i])
            return 0;
    }
    return 1;
}

static void update_fixed(struct fixed * fixed, short line[],short t)
{
    SetConsoleTextAttribute(hSqureConsole, 127);
    for (int k = t - 1; k >= 0; k--)
    {
        add_one(line[k],"+1");
        Sleep(20);
    }
    SetConsoleTextAttribute(hSqureConsole, 119);
    short heighest;
    for (int i = line[0] -1 ; i > 0; i--)
    {
        if (is_blank(fixed, i))
        {
            heighest = i;
            break;
        }
    }
    for (int i = 1; i < COLUMN - 1; i++)
    {
        for (int j = line[t - 1] - 1; j > heighest; j--)
        {
            COORD pos;
            if (j >= line[0])
            {
                short layer;
                if (layer = is_in_line(j, line, t))
                {
                    if (fixed->fix[j][i] == 0)
                    {
                        fixed->fix[j + layer][i] = 0;
                    }
                    else
                    {
                        fixed->fix[j][i] = 0;
                        pos.X = i;
                        pos.Y = j;
                        gotoxy(pos);
                        SetConsoleTextAttribute(hSqureConsole, 119);
                        cout << SPACE;
                        fixed->fix[j + layer][i] = 1;
                        fixed->color[j + layer][i] = fixed->color[j][i];
                        fixed->color[j][i] = 119;
                        pos.X = i;
                        pos.Y = j + layer;
                        gotoxy(pos);
                        SetConsoleTextAttribute(hSqureConsole, fixed->color[pos.Y][pos.X]);
                        cout << SQURE;
                        Sleep(UPDATE_TIME);
                    }
                }
            }
            else
            {
                if (fixed->fix[j][i] == 0)
                {
                    fixed->fix[j + t][i] = 0;
                }
                else
                {
                    fixed->fix[j][i] = 0;
                    pos.X = i;
                    pos.Y = j;
                    gotoxy(pos);
                    SetConsoleTextAttribute(hSqureConsole, 119);
                    cout << SPACE;
                    fixed->fix[j + t][i] = 1;
                    fixed->color[j + t][i] = fixed->color[j][i];
                    fixed->color[j][i] = 119;
                    pos.X = i;
                    pos.Y = j + t;
                    gotoxy(pos);
                    SetConsoleTextAttribute(hSqureConsole, fixed->color[pos.Y][pos.X]);
                    cout << SQURE;
                    Sleep(UPDATE_TIME);
                }
            }
        }
    }
    print_scoure();
    for (int k = t - 1; k >= 0; k--)
    {
        add_one(line[k], "  ");
        Sleep(20);
    }
}

static bool is_in_line(const short y, const short line[],const short t)
{
    for (int i = 0; i < t - 1; i++)
    {
        if (y == line[i])
            return 0;
    }
    for (int i = t-1; i >= 0; i--)
    {
        if (y > line[i])
            return i + 1;
    }
}

static bool is_blank(const struct fixed * fixed,const short line)       //no problem
{
    for (int i = 1; i < COLUMN - 1; i++)
    {
        if (fixed->fix[line][i])
            return 0;
    }
    return 1;
}

void eliminate(struct fixed * fixed, struct squre * form)           //no problem
{
    short line[4];
    short t=0;
    for (int i = 0; i < 3; i++)
    {
        if (check_full(fixed, form->pos[i].Y))
        {
            line[t++] = form->pos[i].Y;
        }
    }
    if (check_full(fixed, form->center.Y))
        line[t++] = form->center.Y;
    //********      no problem
    int repeat_time=0;
    for (int i = 0; i < t - 1; i++)
    {
        for (int j = i; j < t - 1; j++)
        {
            if (line[i] == line[j + 1] && line[i] != ROW)
            {
                line[j + 1] = ROW;
                repeat_time++;
            }
        }
    }
    t -= repeat_time;
    //********      no problem

    for (int i = 0; i < t - 1; i++)
    {
        for (int j = i + 1; j < t; j++)
        {
            if (line[i] > line[j])
            {
                int temp;
                temp = line[i];
                line[i] = line[j];
                line[j] = temp;
            }
        }
    }
    COORD pos;
    int etime;
    if (t)
    {
        etime = 840 / (COLUMN * t);
    }
    else
        etime = 60;
    for (int i = 1; i < COLUMN - 1; i++)
    {
        for (int j = 0; j < t; j++)
        {
            pos.X = i;
            pos.Y = line[j];
            gotoxy(pos);
            cout << "□";
            Sleep(etime / 2);
            gotoxy(pos);
            SetConsoleTextAttribute(hSqureConsole, 119);
            cout << SPACE;
        }
        Sleep(etime / 2);
    }
    if (t)
    {
        scoure += t;
        update_fixed(fixed, line, t);
    }
}

void add_one(short line, char ch[])
{
    COORD pos;
    pos.X = COLUMN;
    pos.Y = line;
    gotoxy(pos);
    fputs(ch, stdout);
}

void scoure_board()
{
    COORD pos;
    pos.X = COLUMN + 3;
    pos.Y = 2;
    gotoxy(pos);
    SetConsoleTextAttribute(hSqureConsole, 115);
    cout << "通关条件：消除" << WIN << "行";
    pos.Y += 2;
    gotoxy(pos);
    cout << "得分：" << scoure;
    pos.Y += 2;
    gotoxy(pos);
    cout << "Next squre:";
    pos.Y += ROW / 2;
    gotoxy(pos);
    cout << "操作：";
    pos.Y += 2;
    pos.X += 3;
    gotoxy(pos);
    cout << "↑ ↓ ← →";
}

void print_scoure()
{
    COORD pos;
    pos.X = COLUMN + 6;
    pos.Y = 4;
    gotoxy(pos);
    SetConsoleTextAttribute(hSqureConsole, 115);
    cout << scoure;
}

void print_next(const struct squre * form)
{
    COORD pos;
    pos.X = COLUMN + 6;
    pos.Y = 8;
    for (int i = 0; i < 4; i++)
    {
        gotoxy(pos);
        cout << "          ";
        pos.Y += 1;
    }
    pos.Y = 10;
    pos.X += 2;
    short a[3][2];
    struct squre temp;
    temp = * form;
    for (int i = 0; i < 3; i++)
    {
        a[i][0] = temp.center.X - temp.pos[i].X;
        a[i][1] = temp.center.Y - temp.pos[i].Y;
    }

    SetConsoleTextAttribute(hSqureConsole, temp.color);
    temp.center.X = pos.X;
    temp.center.Y = pos.Y;
    gotoxy(temp.center);
    cout << SQURE;
    for (int i = 0; i < 3; i++)
    {
        temp.pos[i].X = temp.center.X - a[i][0];
        temp.pos[i].Y = temp.center.Y - a[i][1];
        gotoxy(temp.pos[i]);
        cout << SQURE;
    }
    SetConsoleTextAttribute(hSqureConsole, 115);

}

void time_board()
{
    COORD pos;
    pos.X = COLUMN / 2 - 1;
    pos.Y = 3;
    gotoxy(pos);
    cout << "time:";
    Sleep(300);
    for (int i = 3; i > 0; i--)
    {
        pos.X = COLUMN / 2 + 2;
        gotoxy(pos);
        cout << i;
        Sleep(500);
    }
    pos.X = COLUMN / 2 - 1;
    gotoxy(pos);
    cout << "        ";
}