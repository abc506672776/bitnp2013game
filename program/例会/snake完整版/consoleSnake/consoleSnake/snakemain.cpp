#include "conio.h"
#include "stdio.h"
#include "windows.h"
#include "vector"
#include "time.h"
using namespace std;

//创建一个struct保存坐标信息
typedef struct iPoint
{
	int x;
	int y;
} Point;

//宏定义上下左右四个按键的键值
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

#define WIDTH 40
#define HEIGHT 25

void CheckKeyPress(char key);
void SetCursorTo(int x, int y);
void DrawSnake();
void DrawEraser();
void DrawStar(int x, int y);
void MoveSnake();
void Init();
void Update();
void Draw();
void GenerateFood();
void DrawFood();
void DrawBorder();

bool			isRun = true;
char			keyPress;
int				cursorX = 0, cursorY = 0;
vector<Point>	snake;
vector<Point>	eraser;
int				dx, dy;
Point			food;

int main()
{
	Init();	//自定义的初始化函数

	while (1)
	{
		if (!isRun)
			break;
		Update();		//刷新
		Draw();			//渲染
		Sleep(100);		//睡两秒，防止程序运行过快。也可用来控制游戏速度
	}
	return 0;
}

//初始化函数
void Init()
{
	DrawBorder();
	//初始化贪吃蛇
	Point tempPoint;
	tempPoint.x = 10;
	tempPoint.y = 10;
	snake.push_back(tempPoint);

	tempPoint.x = 11;
	tempPoint.y = 10;
	snake.push_back(tempPoint);

	tempPoint.x = 12;
	tempPoint.y = 10;
	snake.push_back(tempPoint);

	//初始化方向信息
	dx = 1;
	dy = 0;
	GenerateFood();
	//初始化随机数发生器
	srand(time(0));
}

void Update()
{
	if (kbhit())
	{	
		keyPress = getch();
		CheckKeyPress(keyPress);
	}
	eraser.clear();	//清空eraser的
	MoveSnake();	//移动蛇
}

void Draw()
{
	DrawEraser();	//清除不应显示的图像
	DrawFood();		//画食物
	DrawSnake();	//画蛇
}

//检测按键
void CheckKeyPress(char key)
{
	switch (key)
	{
	case 'q':
		{
			isRun = false;
		}break;
	case ' ':
		{
			DrawStar(cursorX,cursorY);
		}break;
	case UP:
		{
			dx = 0;
			dy = -1;
		}break;
	case DOWN:
		{
			dx = 0;
			dy = 1;
		}break;
	case LEFT:
		{
			dx = -1;
			dy = 0;
		}break;
	case RIGHT:
		{
			dx = 1;
			dy = 0;
		}break;
	}
}

//设置光标的位置
void SetCursorTo(int x, int y)
{
	COORD coord = {x*2,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);
	//将光标的位置保存在全局变量中
	cursorX = x;
	cursorY = y;
}

//画一个星星
void DrawStar(int x, int y)
{
	SetCursorTo(x,y);	//将光标设置在要画的位置
	printf("*");		//打印出一个*
}

//画一个@
void DrawAt(int x, int y)
{
	SetCursorTo(x,y);	//将光标设置在要画的位置
	printf("@");		//打印出一个*
}

//画一个char
void DrawChar(int x, int y, char c)
{
	SetCursorTo(x,y);	//将光标设置在要画的位置
	printf("%c",c);		//打印出一个*
}

//画一个空格符 ： 即为清除(x,y)处的图像
void DrawBlank(int x, int y)
{
	SetCursorTo(x,y);
	printf(" ");
}

//画蛇
void DrawSnake()
{
	int length = snake.size();		//获取蛇的长度
	int i;
	for (i = 0; i < length; i++)	//遍历蛇的每一节点并打印一个星号
	{
		DrawStar(snake[i].x, snake[i].y);
	}
}

//画要清除的部分
//每次更新完后需要删除的图形就保存在Eraser中留待删除
void DrawEraser()
{
	int length = eraser.size();
	int i;
	for (i = 0; i < length; i++)
	{
		DrawBlank(eraser[i].x, eraser[i].y);
	}
};

bool CheckEat()
{
	if (snake.back().x == food.x && snake.back().y == food.y)
		return true;
	else
		return false;
}

bool HitBorder()
{
	if (snake.back().x == 0 || snake.back().x == WIDTH-1 ||
		snake.back().y == 0 || snake.back().y == HEIGHT-1)
		return true;
	else
		return false;
}

//移动蛇
void MoveSnake()
{
	Point tempPoint;	//创建一个临时Point
	tempPoint.x = snake.back().x + dx;	//设置这个x为蛇头部的下一个位置的x
	tempPoint.y = snake.back().y + dy;	//同上设置y
	snake.push_back(tempPoint);			//将这个节点放到蛇vector的最后面，即为头部
	if (HitBorder())
	{
		isRun = false;
	}
	if (CheckEat())
	{
		GenerateFood();
	}
	else
	{
		vector<Point>::iterator first = snake.begin();	//创建一个迭代器，获取vector的第一个元素，也是蛇的尾部
		tempPoint.x = first->x;
		tempPoint.y = first->y;
		eraser.push_back(tempPoint);	//将尾部节点保存在要擦出位置的vector中
		snake.erase(first);				//将蛇的尾部节点删除
	}
}

void GenerateFood()
{
	food.x = 1 + rand() % (WIDTH-2) ;
	food.y = 1 + rand() % (HEIGHT-2);
}

void DrawFood()
{
	DrawAt(food.x, food.y);
}

void DrawBorder()
{
	for (int i = 0; i < WIDTH; i++)
	{
		DrawChar(i, 0, '-');
		DrawChar(i, HEIGHT-1, '-');
	}
	for (int i = 0; i < HEIGHT-1; i++)
	{
		DrawChar(0, i, '|');
		DrawChar(WIDTH-1, i, '|');
	}
}

