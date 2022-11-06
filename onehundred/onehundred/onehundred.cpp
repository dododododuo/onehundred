#include <stdio.h>
#include <graphics.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <string.h>
#include <tchar.h>
#define MAX_BOARD 14
#define WIDTH 640
#define HEIGHT 760
#define INTERVAR 60 //板子间隔
DWORD mcolor[10] = { RED,BLACK,WHITE,GREEN,YELLOW};
struct Board //板子
{
	int x;
	int y;
	int len;
	DWORD color;
}board[MAX_BOARD];
struct Ball
{
	int x;
	int y;
	int r;
	int index; //所在板子的下标
	int score; //得分
}ball;
//初始化数据
void GameInit()
{
	//生成随机数种子
	srand((unsigned)time(NULL));
	for (int i = 0; i < MAX_BOARD; i++)
	{
		if (i == 0)board[i].y = rand() % 100 + 100;
		else board[i].y = board[i - 1].y + INTERVAR;
		board[i].x = rand() % (WIDTH - 150); 
		board[i].len = rand() %100 +50;
		board[i].color = mcolor[rand()%5];
	}
	ball.x = board[0].x;
	ball.y = board[0].y;
	ball.r = 15;
	ball.score = 0;
	ball.index = 0;
}
//绘制元件
void GameDraw()
{
	//双缓冲绘图  防止闪屏
	BeginBatchDraw();
	setbkcolor(RGB(230, 231, 232));
	cleardevice();
	IMAGE background;
	loadimage(&background, "./back.png", WIDTH, HEIGHT, 1);
	putimage(0, 0, &background);
	settextcolor(RED);

	TCHAR s1[102] = { _T("您的得分为：") };   //需要预先分配缓冲区
	outtextxy(15, 15, s1);

	TCHAR s[5];
	_stprintf_s(s, _T("%d"), ball.score);		
	outtextxy(110, 15, s);


	for (int i = 0; i < MAX_BOARD; i++)
	{
		setfillcolor(board[i].color);
		solidrectangle(board[i].x, board[i].y,board[i].x + board[i].len, board[i].y+5);
	}
	setfillcolor(board[ball.index].color);
	solidcircle(ball.x, ball.y, ball.r);
	EndBatchDraw();
}
//木板移动
void BoardMove()
{
	for (int i = 0; i < MAX_BOARD; i++)
	{
		board[i].y -= 2;
		if (board[i].y < 0)
		{
			board[i].x = rand() % (WIDTH - 150);
			board[i].len = rand() % 100 + 50;
			board[i].color = mcolor[rand() % 5];
			board[i].y = MAX_BOARD * INTERVAR;
		}
	}

}
//小球移动
void BallMove()
{
	if (GetAsyncKeyState(VK_LEFT))
	{
		ball.x -= 4;
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		ball.x += 4;
	}
	if (ball.x <= 0)ball.x += WIDTH;
	if (ball.x >= WIDTH)ball.x -= WIDTH;
	for (int i = 0; i < MAX_BOARD; i++)
	{
		if (ball.x >= board[i].x && ball.x <= board[i].x + board[i].len
			&& ball.y + ball.r >= board[i].y && ball.y - 5 + ball.r <= board[i].y + 5)
		{
			ball.index = i;
			ball.y = board[i].y - ball.r ;
			
			break;
		}
		else ball.index = -1;
	}
	if (ball.index == -1)ball.y += 5;
}
//判定游戏结束

void Jude()
{
	if (ball.y <= 0 || ball.y >= HEIGHT)
	{
		char str[100] ;
		sprintf_s(str, "%s%d%s", "您的得分为：", ball.score,"\n是否继续游戏？");
		int isok = MessageBox(GetHWnd(), str, "游戏结束", MB_YESNO);
		if (isok == IDYES)
		{
			GameInit();
		}
		else exit(9);
	}
}


int main()
{
	initgraph(WIDTH, HEIGHT);
	GameInit();
	while (1)
	{
		GameDraw();
		ball.score += 1;
		BoardMove();
		BallMove();
		Sleep(30);
		Jude();
	}
	
	getchar();
	return 0;
}