#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include<stdbool.h>
#define MINE_COUNT 100  //地雷数量
#define ROW 30  //更改棋盘行大小
#define COL 30  //更改棋盘列大小
#define ROWS ROW+2
#define COLS COL+2
#define MINE_FLAG '$'  //地雷标记
#define INIT_FLAG '*'  //初始化标价
enum CHOOSE
{
	EXIT,
	PLAY,
};
extern int flag;
extern int setp;

void init_board(char a[][COLS], int r, int c, char set);  //初始化棋盘
void display_board(char a[][COLS], int r, int c);  //打印棋盘
void set_mine(char mine[][COLS], int r, int c, int count);  //布置雷
void find_mine(char show[][COLS], char mine[][COLS], int r, int c, int input_coords[][2], int input_count);  //排查雷

int get_mine(char show[][COLS], char mine[][COLS], int r, int c);  //得到雷的数量

bool is_mine(char mine[][COLS], int x, int y);  //判断下x，y是否为雷
bool is_find(char show[][COLS], int x, int y);  //判断是否被排查过

void surprise(char mine[][COLS], char show[][COLS], int r, int c, int x, int y);  //展开一片
#define  _CRT_SECURE_NO_WARNINGS 0

#pragma warning (disable:6031)  //禁用C6031警告

void init_board(char a[][COLS], int r, int c, char set)
{
	int i = 1;
	int j = 1;
	for (i = 1; i <= r; i++)
	{
		for (j = 1; j <= c; j++)
		{
			a[i][j] = set;
		}
	}
}

void set_mine(char mine[][COLS], int r, int c, int count)
{
	int x = 0;
	int y = 0;
	while (count)
	{
		x = rand() % r + 1;
		y = rand() % c + 1;
		if (mine[x][y] == ' ')
		{
			mine[x][y] = MINE_FLAG;
			count--;
		}
	}
}
void display_board(char a[][COLS], int r, int c)
{
	int i = 1;
	int j = 1;
	printf("    ");
	for (j = 1; j <= c; j++)
	{
		if (j < 10)
			printf("%d ", j);
		else
		{
			printf("%c ", 'A' + j - 10);
		}
	}
	putchar('\n');
	putchar(' ');
	putchar(' ');

	printf("  ");
	for (j = 2; j <= c + 1; j++)
	{
		printf("— ");
	}
	putchar('\n');

	for (i = 1; i <= r; i++)
	{
		if (i < 10)
			printf(" %d| ", i);
		else
		{
			printf(" %c| ", 'A' + i - 10);
		}
		for (j = 1; j <= c; j++)
		{
			if (a[i][j] == MINE_FLAG)
				printf("\033[1;31m%c \033[0m", MINE_FLAG);
			else if (a[i][j] >= '1' && a[i][j] <= '8')
			{
				//为数字添加颜色	
				printf("\033[1;%dm%c \033[0m", 30 + a[i][j] - '0', a[i][j]);
			}
			else
				printf("%c ", a[i][j]);
		}
		printf("\n");
	}
}

void find_mine(char show[][COLS], char mine[][COLS], int r, int c, int input_coords[][2], int input_count)  //看看能不能再优化
{
	int x = 0;
	int y = 0;
	char ch = 0;
	printf("请输入你想要排查雷的位置，并选择是否对该点进行标记(要标记请输入Y，其它默认不标记)\n");  //对同一个点多次标记会取消标记
	//打印之前输入的坐标
	printf("之前输入的坐标：\n");

	for (int i = 0; i < input_count; i++)
	{
		printf("(%d, %d)  ", input_coords[i][0], input_coords[i][1]);
	}

	while (1)
	{
		printf("\n");

		scanf_s("%d %d %c", &x, &y, &ch);
		//将输入的坐标添加到input_coords数组中
		input_coords[input_count][0] = x;
		input_coords[input_count][1] = y;

		if (x >= 1 && x <= r && y >= 1 && y <= c)  //坐标合法
		{
			if (!is_find(show, x, y))  //未被排查过
			{
				if (show[x][y] == 'P' && 'Y' == ch)
				{
					show[x][y] = INIT_FLAG;
					printf("取消标记成功\n");
					system("cls");
					display_board(show, r, c);
					break;
				}
				if ('Y' == ch)  //判断一下是否需要标记
				{
					show[x][y] = 'P';
					printf("标记成功\n");
					system("cls");
					display_board(show, r, c);
					break;
				}
				else
				{
					if (is_mine(mine, x, y))  //该位置是地雷
					{
						printf("你被雷炸死了!!!\n\a\a\a");
						printf("你一共排了%d次\n", input_count);
						show[x][y] = MINE_FLAG;  //将炸死的回访展示出来
						printf("请按任意键查看所有地雷位置");
						getchar();
						getchar();
						flag = 0;
						system("cls");
						display_board(mine, r, c);
						getchar();
						break;
					}
					else  //不是雷
					{
						surprise(mine, show, r, c, x, y);  //进一步判断该位置周围雷的数量，若为0，进行进一步展开
						system("cls");
						display_board(show, r, c);
						break;
					}
				}
			}
			else
			{
				printf("坐标已排查过,请重新输入:>\n");
			}

		}
		else
			printf("坐标非法,请重新输入:>\n");
	}
}

int get_mine(char show[][COLS], char mine[][COLS], int x, int y)
{
	int count = 0;
	if (mine[x - 1][y - 1] == MINE_FLAG)
		count++;
	if (mine[x - 1][y] == MINE_FLAG)
		count++;
	if (mine[x - 1][y + 1] == MINE_FLAG)
		count++;
	if (mine[x][y - 1] == MINE_FLAG)
		count++;
	if (mine[x][y + 1] == MINE_FLAG)
		count++;
	if (mine[x + 1][y - 1] == MINE_FLAG)
		count++;
	if (mine[x + 1][y] == MINE_FLAG)
		count++;
	if (mine[x + 1][y + 1] == MINE_FLAG)
		count++;
	return count;
}

bool is_mine(char mine[][COLS], int x, int y)
{
	if (mine[x][y] == ' ')
		return false;
	else
		return true;
}

bool is_find(char show[][COLS], int x, int y)
{
	if (show[x][y] == INIT_FLAG || show[x][y] == 'P')
		return false;
	else
		return true;
}

void surprise(char mine[][COLS], char show[][COLS], int r, int c, int x, int y)
{
	if (!(x >= 1 && x <= r && y >= 1 && y <= c))  //防止越界
		return;
	if (is_mine(mine, x, y) || is_find(show, x, y))  //能展开的条件：1、没被排查过；2、排查位置不是雷
		return;
	setp--;
	int count = get_mine(show, mine, x, y);  //3、排查位置周围没有雷
	show[x][y] = count + '0';
	if (count == 0)
	{
		surprise(mine, show, r, c, x - 1, y - 1);
		surprise(mine, show, r, c, x - 1, y);
		surprise(mine, show, r, c, x - 1, y + 1);
		surprise(mine, show, r, c, x, y - 1);
		surprise(mine, show, r, c, x, y + 1);
		surprise(mine, show, r, c, x + 1, y - 1);
		surprise(mine, show, r, c, x + 1, y);
		surprise(mine, show, r, c, x + 1, y + 1);
	}
}

#define  _CRT_SECURE_NO_WARNINGS 0

#pragma warning (disable:6031)

int setp = ROW * COL - MINE_COUNT;  //记录排雷最多步数，当setp等于0是表示安全位置已经排完，挑战成功
int flag = 1;  //表示未被地雷炸死而结束游戏，地雷炸死而结束游戏的flag=0，挑战失败

void print_line(int length) {
	for (int i = 0; i < length; i++) {
		printf("*");
	}
	printf("\n");
}

void print_title(const char* title) {
	int length = strlen(title) + 4;
	print_line(length);
	printf("* %s *\n", title);
	print_line(length);
}

void game_over(bool success) {
	system("cls");
	if (success) {
		printf("恭喜你，所有雷都被你扫完了！\n");
	}
	else {
		printf("很遗憾，挑战失败！\n");
	}
	printf("\n按任意键继续...");
	getchar();
}

void game()
{
	char mine[ROWS][COLS] = { 0 };
	char show[ROWS][COLS] = { 0 };
	int input_coords[ROW * COL][2] = { 0 };   //存储输入的坐标

	init_board(mine, ROW, COL, ' ');  //初始化
	init_board(show, ROW, COL, INIT_FLAG);  //初始化

	set_mine(mine, ROW, COL, MINE_COUNT);  //布置雷
	display_board(show, ROW, COL);  //展示棋盘（给玩家看）

	setp = ROW * COL - MINE_COUNT;
	flag = 1;  //重置一下，开始新的游戏

	int input_count = 0;
	while (setp && flag)
	{
		find_mine(show, mine, ROW, COL, input_coords, input_count);   // 将input_coords和input_count传递给find_mine()
		input_count++;
	}
	if (!setp) {
		game_over(true);
	}
	else if (!flag) {
		game_over(false);
	}
	printf("死亡回放:\n");
	display_board(show, ROW, COL);  //展示棋盘（给玩家看）

	printf("  ");
	for (int j = 0; j <= COL; j++)
	{
		printf("--");
	}
	getchar();
	printf("请重新开始游戏");
	getchar();
}

void menu()
{
	system("cls");
	print_title("扫雷小游戏");
	
	printf("游戏介绍:\n");
	printf("1. 两次对同一个点标记,可取消标记.\n");
	printf("2. 最后一次输入位置若是地雷,则该位置将会被标为\033[1;31m%c\033[0m以此告诉玩家死亡原因.\n", MINE_FLAG);
	printf("3. 若在坐标后面输入Y则表示对该点进行标记，其它字母默认不标记.\n");
	printf("4. A-U表示10-30.\n");
	printf("5. 坐标先列后行.");

	printf("\n");
	printf("\n操作菜单:\n");
	printf("*******************************\n");
	printf("* 1. 开始游戏 (play)          *\n");
	printf("* 0. 退出游戏 (exit)          *\n");
	printf("*******************************\n");

	printf("\n请输入您的选择 (0 或 1): ");
}

int main() {
	srand((unsigned int)time(NULL)); //生成整型随机数
	int input = 0;

	do {
		menu();
		scanf_s("%d", &input);
		switch (input) {
		case PLAY:
			system("cls");
			game();
			break;
		case EXIT:
			system("cls");
			printf("退出游戏\n");
			break;
		default:
			system("cls");
			printf("输入有误，重新输入:>\n");
			break;
		}
	} while (input);

	return 0;
}
