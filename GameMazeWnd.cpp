// GameMazeWnd.cpp : implementation file
//

//#include "stdafx.h"
#include "game.h"
#include "GameMazeWnd.h"
#include "GameMazeApp.h"
#include<Windows.h>
#include<time.h>
#include<math.h>


#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GameMazeWnd
#define WALL  0
#define ROUTE 1
#define  L 17

int paintFlag = 0;  //0 默认， 1 newGame, 2 Quit
int preFlag = 0;
int gameStartFlag = 0;         //0 主菜单 ， 1 house, 2 out, 3 maze , 4 设置, 5 宝藏
int paintFlag1 = 0;
int mazeFlag = 0;
int musicFlag = 1;

int housemap[15][15] = {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
									2,0,0,5,5,5,5,5,5,0,0,0,0,0,2,
									2,4,0,5,5,5,5,5,5,0,0,0,0,0,2,
									2,4,0,0,0,0,0,0,0,0,0,0,0,0,2,
									2,0,0,0,7,0,0,0,0,0,0,0,0,0,2,
									2,0,1,0,0,0,0,0,0,0,0,0,0,0,2,
									2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
									2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
									2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
									2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
									2,0,0,0,0,0,0,0,0,0,0,6,0,0,2,
									2,0,0,0,0,0,0,0,0,0,0,6,0,0,2,
									2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
									2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,
									2,2,2,2,2,2,3,3,3,2,2,2,2,2,2}; //0表示地板， 1表示人物, 2 表示墙, 3表示楼梯, 4代表床, 5表示书架, 6表示女神像, 7表示椅子

int outmap[15][15] = {0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,
								0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,
								0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,
								0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,
								0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,
								0,0,0,3,3,3,3,3,3,3,3,3,0,0,0,
								0,0,0,3,3,4,3,3,3,3,3,3,0,0,0,
								0,0,0,3,3,3,3,3,3,5,3,3,0,0,0,
								0,0,0,3,3,3,3,3,3,5,3,3,0,0,0,
								0,0,0,6,0,0,0,0,0,1,0,7,0,0,0,
								0,0,0,6,0,0,0,0,0,0,0,7,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,8,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
								0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};  //0 表示地板，1表示人物， 2表示房顶，3表示墙，4表示窗，5表示门，6表示左女神像，7表示右女神像，8表示迷宫入口

int maze[17][17];

int x = 2, y = 5;
int ox = 9, oy = 9;
int mx = 1, my = 2;
int finx = 15,finy = 14;

int px, py;

int direct = 1;
int odirect = 1;
int mdirect = 1;


char fileName[20];

static int Rank = 0;

IMPLEMENT_DYNCREATE(GameMazeWnd, CFrameWnd)


GameMazeWnd::CreateMaze(int maze[17][17], int dx, int dy)
{
	int i,j,k;
	maze[dx][dy] = ROUTE;
	//确保四个方向随机
	int direction[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
	for (i = 0; i < 4; i++)
	{
		int r = rand() % 4;
		int temp = direction[0][0];
		direction[0][0] = direction[r][0];
		direction[r][0] = temp;
		
		temp = direction[0][1];
		direction[0][1] = direction[r][1];
		direction[r][1] = temp;
	}
	
	//向四个方向开挖
	for ( i = 0; i < 4; i++)
	{
		int ax = dx;
		int ay = dy;
		
		//控制挖的距离，由Rank来调整大小
		int range = 1 + (Rank == 0 ? 0 : rand() % Rank);
		while (range>0) {
			ax += direction[i][0];
			ay += direction[i][1];
			
			//排除掉回头路
			if (maze[ax][ay] == ROUTE) {
				break;
			}
			
			//判断是否挖穿路径
			int count = 0;
			for ( j = ax - 1; j < ax + 2; j++) {
				for ( k =ay - 1; k < ay + 2; k++) {
					//abs(j - dx) + abs(k - dy) == 1 确保只判断九宫格的四个特定位置
					if (abs(j - ax) + abs(k - ay) == 1 && maze[j][k] == ROUTE) {
						count++;
					}
				}
			}
			
			if (count > 1) {
				break;
			}
			
			//确保不会挖穿时，前进
			--range;
			maze[ax][ay] = ROUTE;
		}
		
		//没有挖穿危险，以此为节点递归
		if (range <= 0) {
			CreateMaze(maze,ax,ay);
		}
	}
}


GameMazeWnd::GameMazeWnd()
{
	Create(NULL,"Endless Maze Ver1.0");
	this->SetWindowPos(NULL,0,0,1200,1200,0);
	mciSendString("play Theme1.mp3",0,0,0);
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	this->SetMenu(&menu);
	mdc  = new CDC;
	CClientDC dc(this);
	mdc->CreateCompatibleDC(&dc);
	bkgd = new CBitmap;
	bkgd->m_hObject = LoadImage(NULL,"bk.bmp",IMAGE_BITMAP,1200,1200,LR_LOADFROMFILE);
	chair= new CBitmap;
	chair->m_hObject = LoadImage(NULL,"chair.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
	window= new CBitmap;
	window->m_hObject = LoadImage(NULL,"window.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
	down= new CBitmap;
	down->m_hObject = LoadImage(NULL,"down.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
	int i, j;
	for(i = 0;i < 15;i++)
	{
		for(j = 0; j< 15;j++)
		{
			housefloor[i][j] = new CBitmap;
			housefloor[i][j]->m_hObject = LoadImage(NULL,"housefloor.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);	
			wall[i][j] = new CBitmap;
			wall[i][j]->m_hObject = LoadImage(NULL,"wall.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
			outfloor[i][j] = new CBitmap;
			outfloor[i][j]->m_hObject = LoadImage(NULL,"outfloor.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
			houseroof[i][j] = new CBitmap;
			houseroof[i][j]->m_hObject = LoadImage(NULL,"houseroof.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
			outwall[i][j] = new CBitmap;
			outwall[i][j]->m_hObject = LoadImage(NULL,"outwall.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
			grass[i][j] = new CBitmap;
			grass[i][j]->m_hObject = LoadImage(NULL,"grass.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
			leaf[i][j] = new CBitmap;
			leaf[i][j]->m_hObject = LoadImage(NULL,"leaf.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		}
	}
	for(i = 0;i < 4; i++)
	{
		bitmap[i] = new CBitmap;
		sprintf(fileName, "%d.bmp",i+1);
		bitmap[i]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		obitmap[i] = new CBitmap;
		sprintf(fileName, "o%d.bmp",i+1);
		obitmap[i]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		mbitmap[i] = new CBitmap;
		sprintf(fileName, "m%d.bmp",i+1);
		mbitmap[i]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		for(j = 0;j< 4;j++)
		{
			tree[i][j] = new CBitmap;
			tree[i][j]->m_hObject = LoadImage(NULL,"tree.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
			flower[i][j] = new CBitmap;
			flower[i][j]->m_hObject = LoadImage(NULL,"flower.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
			rock[i][j] = new CBitmap;
			rock[i][j]->m_hObject = LoadImage(NULL,"rock.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
			cactus[i][j] = new CBitmap;
			cactus[i][j]->m_hObject = LoadImage(NULL,"cactus.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
			bean[i][j] = new CBitmap;
			bean[i][j]->m_hObject = LoadImage(NULL,"bean.bmp",IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		}
	}
	for(i = 0;i < 3;i++)
	{
		stair[i] = new CBitmap;
		sprintf(fileName,"stair%d.bmp",i+1);
		stair[i]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
	}
	for(i = 0;i < 2;i++)
	{
		bed[i] = new CBitmap;
		sprintf(fileName,"bed%d.bmp",i+1);
		bed[i]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		godness[i] = new CBitmap;
		sprintf(fileName,"godness%d.bmp",i+1);
		godness[i]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		door[i] = new CBitmap;
		sprintf(fileName,"door%d.bmp",i+1);
		door[i]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		outlgod[i] = new CBitmap;
		sprintf(fileName,"outlgod%d.bmp",i+1);
		outlgod[i]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		outrgod[i] = new CBitmap;
		sprintf(fileName,"outrgod%d.bmp",i+1);
		outrgod[i]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
	}
	for(i = 0;i < 2;i++)
	{
		for(j = 0;j < 6;j++)
		{
			shelf[i][j] = new CBitmap;
			sprintf(fileName,"shelf%d.%d.bmp",i+1,j+1);
			shelf[i][j]->m_hObject = LoadImage(NULL,fileName,IMAGE_BITMAP,60,60,LR_LOADFROMFILE);
		}
	}
}	

GameMazeWnd::~GameMazeWnd()
{

}


BEGIN_MESSAGE_MAP(GameMazeWnd, CFrameWnd)
	//{{AFX_MSG_MAP(GameMazeWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_About, OnAbout)
	ON_COMMAND(ID_HELP, OnHelp)
	ON_COMMAND(ID_Music1, OnMusic1)
	ON_COMMAND(ID_Music2, OnMusic2)
	ON_COMMAND(ID_Music3, OnMusic3)
	ON_COMMAND(ID_noMusic, OnnoMusic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GameMazeWnd message handlers

void GameMazeWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	int i, j;
	if(gameStartFlag == 4)
	{
		HBRUSH NullBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		CBrush* pNullBrush = CBrush::FromHandle(NullBrush);
		CBrush* pOldBrush = dc.SelectObject(pNullBrush);
		CPen pen;
		pen.CreatePen(PS_DASH ,5,RGB(0,0,0));
		dc.SelectObject(&pen);
		//dc.RoundRect(350,365,550,560,50,50);
		dc.Rectangle(350,365,550,560);
		CFont font;
		font.CreateFont( 36, 0,  0, 0, FW_BOLD,   FALSE,   FALSE,     0,     ANSI_CHARSET,  OUT_DEFAULT_PRECIS,  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH   |   FF_SWISS,   _T("consolas"));
		dc.SelectObject(&font);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(400,410, _T("continue"));
		dc.TextOut(410,505,_T("Quit"));
		if(paintFlag1 == 1)
		{
			HBRUSH WhiteBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
			CBrush* pWhiteBrush = CBrush::FromHandle(WhiteBrush);
			CBrush* pOldBrush = dc.SelectObject(pWhiteBrush);
			dc.RoundRect(360,395,540,450,20,20);
			dc.SelectObject(&font);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(0, 0, 0));
			dc.TextOut(400,410, _T("continue"));
		}
		if(paintFlag1 == 2)
		{
			HBRUSH WhiteBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
			CBrush* pWhiteBrush = CBrush::FromHandle(WhiteBrush);
			CBrush* pOldBrush = dc.SelectObject(pWhiteBrush);
			dc.RoundRect(360,490,540,545,20,20);
			dc.SelectObject(&font);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(0, 0, 0));
			dc.TextOut(410,505,_T("Quit"));
		}
	}  
	else if(gameStartFlag == 3)
	{
		if(mazeFlag == 0)
		{
			mazeFlag = 1;
			srand((unsigned)time(NULL));
			for(i=0;i<17;i++)
			{
				for(j = 0;j<17;j++)
				{
					maze[i][j] = 0;
				}
			}
			for ( i = 0; i < L; i++)
			{
				maze[i][0] = ROUTE;
				maze[0][i] = ROUTE;
				maze[i][L - 1] = ROUTE;
				maze[L - 1][i] = ROUTE;
			}
			CreateMaze(maze, 2, 2);
			maze[2][1] = ROUTE;
			
			//由于算法随机性，出口有一定概率不在（L-3,L-2）处，此时需要寻找出口
			for (i = L - 3; i >= 0; i--)
			{
				if (maze[i][L - 3] == ROUTE)
				{
					maze[i][L - 2] = ROUTE;
					break;
				}
			}
			for(i=15;i>=1;i--)
			{
				if(maze[i][15] == 1)
				{
					finx = 15;
					finy = i;
					break;
				}
			}
		maze[2][1] = 2;   //2是人
		}
		for(i = 1;i <5;i++)
		{
			for(j = 1;j < 5; j++)
			{
				if(maze[i][j] == 0 || maze[i][j] == 3)
				{
					maze[i][j] = 3;       //3 是树
					mdc->SelectObject(tree[i-1][j-1]);
					dc.BitBlt((j-1)*60,(i-1)*60,60,60,mdc,0,0,SRCCOPY);
				}
			}
		}
		for(i = 1; i < 5; i++)
		{
			for(j = 12; j < 16; j++)
			{
				if(maze[i][j] == 0 || maze[i][j] == 4)
				{
					maze[i][j] = 4;       //4 是花
					mdc->SelectObject(flower[i-1][j-12]);
					dc.BitBlt((j-1)*60,(i-1)*60,60,60,mdc,0,0,SRCCOPY);
				}
			}
		}
		for(i = 12; i < 16; i++)
		{
			for(j = 1; j < 5; j++)
			{
				if(maze[i][j] == 0 || maze[i][j] == 5)
				{
					maze[i][j] = 5;       //5是石头
					mdc->SelectObject(rock[i-12][j-1]);
					dc.BitBlt((j-1)*60,(i-1)*60,60,60,mdc,0,0,SRCCOPY);
				}
			}
		}
		for(i = 12; i < 16; i++)
		{
			for(j = 12 ;j < 16;j++)
			{
				if(maze[i][j] == 0 || maze[i][j] == 6)
				{
					maze[i][j] = 6;      //6是仙人掌
					mdc->SelectObject(cactus[i-12][j-12]);
					dc.BitBlt((j-1)*60,(i-1)*60,60,60,mdc,0,0,SRCCOPY);
				}
			}
		}
		for(i = 6;i <10;i++)
		{
			for(j = 6;j < 10; j++)
			{
				if(maze[i][j] == 0 || maze[i][j] == 7)
				{
					maze[i][j] = 7;       //7 是豆芽
					mdc->SelectObject(bean[i-6][j-6]);
					dc.BitBlt((j-1)*60,(i-1)*60,60,60,mdc,0,0,SRCCOPY);
				}
			}
		}
		for(i = 1;i < 16;i++)
		{
			for(j = 1;j < 16 ;j++)
			{
				if(maze[i][j] == 2)
				{
					mx = j;
					my = i;
					mdc->SelectObject(mbitmap[mdirect-1]);
					dc.BitBlt((j-1)*60,(i-1)*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(maze[i][j] == 0)
				{
					mdc->SelectObject(leaf[i-1][j-1]);
					dc.BitBlt((j-1)*60,(i-1)*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(maze[i][j] == 1)
				{
					mdc->SelectObject(grass[i-1][j-1]);
					dc.BitBlt((j-1)*60,(i-1)*60,60,60,mdc,0,0,SRCCOPY);
				}
			}
		}
	}
	if(gameStartFlag == 2)
	{
		for(i = 0;i < 15;i++)
		{
			for(j = 0;j < 15; j++)
			{
				if(outmap[i][j] == 1)
				{
					ox = j;
					oy = i;
					mdc->SelectObject(obitmap[odirect-1]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(outmap[i][j] == 0)
				{
					mdc->SelectObject(outfloor[i][j]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(outmap[i][j] == 2)
				{
					mdc->SelectObject(houseroof[i][j]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(outmap[i][j] == 3)
				{
					mdc->SelectObject(outwall[i][j]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(outmap[i][j] == 4)
				{
					mdc->SelectObject(window);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(outmap[i][j] == 5)
				{
					mdc->SelectObject(door[i-7]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(outmap[i][j] == 6)
				{
					mdc->SelectObject(outlgod[i-9]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(outmap[i][j] == 7)
				{
					mdc->SelectObject(outrgod[i-9]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(outmap[i][j] == 8)
				{
					mdc->SelectObject(down);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
			}
		}
	}
	else if(gameStartFlag == 1)
	{
		for(i = 0;i < 15;i++)
		{
			for(j = 0; j< 15;j++)
			{
				if(housemap[i][j] == 1)
				{
					x = j;
					y = i;
					mdc->SelectObject(bitmap[direct-1]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(housemap[i][j] == 0)
				{
					mdc->SelectObject(housefloor[i][j]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(housemap[i][j] == 2)
				{
					mdc->SelectObject(wall[i][j]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(housemap[i][j] == 3)
				{
					mdc->SelectObject(stair[j-6]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(housemap[i][j] == 4)
				{
					mdc->SelectObject(bed[i-2]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(housemap[i][j] == 5)
				{
					mdc->SelectObject(shelf[i-1][j-3]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(housemap[i][j] == 6)
				{
					mdc->SelectObject(godness[i-10]);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
				else if(housemap[i][j] == 7)
				{
					mdc->SelectObject(chair);
					dc.BitBlt(j*60,i*60,60,60,mdc,0,0,SRCCOPY);
				}
			}
		}
	}
	else if(gameStartFlag == 0)
	{
		mdc->SelectObject(bkgd);
		dc.BitBlt(0,0,1200,1200,mdc,0,0,SRCCOPY);
		HBRUSH NullBrush = (HBRUSH)::GetStockObject(NULL_BRUSH);
		CBrush* pNullBrush = CBrush::FromHandle(NullBrush);
		CBrush* pOldBrush = dc.SelectObject(pNullBrush);
		CPen pen;
		pen.CreatePen(PS_DASH ,5,RGB(0,0,0));
		dc.SelectObject(&pen);
		dc.RoundRect(465,485,735,745,30,30);
		CFont font1, font2;
		font1.CreateFont( 96,                                    //   字体的高度   
			0,                                          //   字体的宽度  
			0,                                          //  nEscapement 
			0,                                          //  nOrientation   
			FW_BOLD,                                  //   nWeight   
			FALSE,                                      //   bItalic   
			FALSE,                                      //   bUnderline   
			0,                                                   //   cStrikeOut   
			ANSI_CHARSET,                             //   nCharSet   
			OUT_DEFAULT_PRECIS,                 //   nOutPrecision   
			CLIP_DEFAULT_PRECIS,               //   nClipPrecision   
			DEFAULT_QUALITY,                       //   nQuality   
			DEFAULT_PITCH   |   FF_SWISS,     //   nPitchAndFamily     
			_T("Palace Script MT"));
		font2.CreateFont( 36, 0,  0, 0, FW_BOLD,   FALSE,   FALSE,     0,     ANSI_CHARSET,  OUT_DEFAULT_PRECIS,  CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH   |   FF_SWISS,   _T("consolas"));
		dc.SelectObject(&font1);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOut(425,200,_T("Endless Maze"));
		dc.SelectObject(&font2);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(RGB(0, 0, 0));
		dc.TextOut(530,545, _T("NewGame"));
		dc.TextOut(550,675,_T("Quit"));
		if(paintFlag == 1)
		{
			HBRUSH WhiteBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
			CBrush* pWhiteBrush = CBrush::FromHandle(WhiteBrush);
			CBrush* pOldBrush = dc.SelectObject(pWhiteBrush);
			dc.RoundRect(480,525,720,600,30,30);
			dc.SelectObject(&font2);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(0, 0, 0));
			dc.TextOut(530,545, _T("NewGame"));
		}
		if(paintFlag == 2)
		{
			HBRUSH WhiteBrush = (HBRUSH)::GetStockObject(WHITE_BRUSH);
			CBrush* pWhiteBrush = CBrush::FromHandle(WhiteBrush);
			CBrush* pOldBrush = dc.SelectObject(pWhiteBrush);
			dc.RoundRect(480,650,720,725,30,30);
			dc.SelectObject(&font2);
			dc.SetBkMode(TRANSPARENT);
			dc.SetTextColor(RGB(0, 0, 0));
			dc.TextOut(550,675,_T("Quit"));
		}
	}

}









void GameMazeWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnLButtonDown(nFlags, point);
	if(gameStartFlag == 4)
	{
		if(nFlags == MK_LBUTTON)
		{
			if(point.x > 360 && point.x < 540 && point.y > 395 && point.y < 450 )
			{
				gameStartFlag = preFlag;
				Invalidate();
			}
			else if(point.x > 360 && point.x < 540 && point.y >490 && point.y < 545)
			{
				AfxGetMainWnd()->SendMessage(WM_CLOSE);
			}
		}
	}
	if(gameStartFlag == 0)
	{
		if(nFlags == MK_LBUTTON)
		{
			if(point.x > 480 && point.x < 720 && point.y > 525 && point.y < 600 )
			{
				mciSendString("close Theme1.mp3",0,0,0);
				this->SetWindowPos(NULL,0,0,920,940,0);
				mciSendString("play Theme2.mp3 repeat",0,0,0);
				gameStartFlag = 1;
				Invalidate();
			}
			else if(point.x > 480 && point.x < 720 && point.y >650 && point.y < 725)
			{
				AfxGetMainWnd()->SendMessage(WM_CLOSE);
			}
		}
	}
}

void GameMazeWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnMouseMove(nFlags, point);
	if(gameStartFlag == 4)
	{
		if(point.x > 360 && point.x < 540 && point.y > 395 && point.y < 450 )
		{
			if(paintFlag1 == 0)
			{
				paintFlag1 = 1;
				InvalidateRect(CRect(360,395,540,450),true);
			}
		}
		else if(paintFlag1 == 1)
		{
			paintFlag1 = 0;
			InvalidateRect(CRect(360,395,540,450),true);
		}
		if(point.x > 360 && point.x < 540 && point.y >490 && point.y < 545)
		{
			if(paintFlag1 == 0)
			{
				paintFlag1 = 2;
				InvalidateRect(CRect(360,490,540,545),true);
			}
		}
		else if(paintFlag1 ==2)
		{
			paintFlag1 = 0;
			InvalidateRect(CRect(360,490,540,545),true);
		}
	}
	else if(gameStartFlag == 0)
	{
		if(point.x > 480 && point.x < 720 && point.y > 525 && point.y < 600 )
		{
			if(paintFlag == 0)
			{
				paintFlag = 1;
				InvalidateRect(CRect(480,525,720,600),true);
			}
		}
		else if(paintFlag == 1)
		{
			paintFlag = 0;
			InvalidateRect(CRect(480,525,720,600),true);
		}
		if(point.x > 480 && point.x < 720 && point.y >650 && point.y < 725)
		{
			if(paintFlag == 0)
			{
				paintFlag = 2;
				InvalidateRect(CRect(480,650,720,725),true);
			}
		}
		else if(paintFlag ==2)
		{
			paintFlag = 0;
			InvalidateRect(CRect(480,650,720,725),true);
		}
	}
}

void GameMazeWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	if(gameStartFlag == 3)
	{
		switch(nChar)
		{
		case 'w':
		case 'W':
			mdirect = 2;
			InvalidateRect(CRect((mx-1) * 60,(my-1) * 60,(mx) * 60,(my) * 60),true);
			if(maze[my-1][mx] == 1  && my > 1)
			{
				maze[my][mx] =1 ;
				maze[my-1][mx] = 2;
				InvalidateRect(CRect((mx-1) * 60,(my-2) * 60,(mx) * 60,(my) * 60),true);
			}
			break;
		case 's':
		case 'S':
			mdirect = 1;
			InvalidateRect(CRect((mx-1) * 60,(my-1) * 60,(mx) * 60,(my) * 60),true);
			if(maze[my+1][mx] == 1 && my < 15)
			{
				maze[my][mx] =1 ;
				maze[my+1][mx] = 2;
				InvalidateRect(CRect((mx-1) * 60,(my-1) * 60,(mx) * 60,(my+1) * 60),true);
			}
			break;
		case 'a':
		case 'A':
			mdirect = 3;
			InvalidateRect(CRect((mx-1) * 60,(my-1) * 60,(mx) * 60,(my) * 60),true);
			if(maze[my][mx-1] == 1 && mx > 1)
			{
				maze[my][mx] =1 ;
				maze[my][mx-1] = 2;
				InvalidateRect(CRect((mx-2) * 60,(my-1) * 60,(mx) * 60,(my) * 60),true);
			}
			break;
		case 'd':
		case 'D':
			mdirect = 4;
			InvalidateRect(CRect((mx-1) * 60,(my-1) * 60,(mx) * 60,(my) * 60),true);
			if(maze[my][mx+1] == 1 && mx < 15)
			{
				maze[my][mx] =1 ;
				maze[my][mx+1] = 2;
				InvalidateRect(CRect((mx-1) * 60,(my-1) * 60,(mx+1) * 60,(my) * 60),true);
			}
			break;
		case VK_RETURN:
			if( mx== finx && my == finy && mdirect == 4)
			{
				//mciSendString("close Theme2.mp3",0,0,0);
				this->SetWindowPos(NULL,0,0,920,940,0);
				//mciSendString("play Theme2.mp3",0,0,0);
				gameStartFlag = 3;
				mazeFlag = 0;
				Invalidate();
			}
		}
	}
	if(gameStartFlag == 2)
	{
		switch(nChar)
		{
		case 'w':
		case 'W':
			odirect = 2;
			InvalidateRect(CRect(ox * 60,oy * 60,(ox+1) * 60,(oy+1) * 60),true);
			if(outmap[oy-1][ox] == 0  && oy > 0)
			{
				outmap[oy][ox] =0 ;
				outmap[oy-1][ox] = 1;
				InvalidateRect(CRect(ox * 60,(oy-1) * 60,(ox+1) * 60,(oy+1) * 60),true);
			}
			break;
		case 's':
		case 'S':
			odirect = 1;
			InvalidateRect(CRect(ox * 60,oy * 60,(ox+1) * 60,(oy+1) * 60),true);
			if(outmap[oy+1][ox] == 0 && oy < 14)
			{
				outmap[oy][ox] =0 ;
				outmap[oy+1][ox] = 1;
				InvalidateRect(CRect(ox * 60,oy * 60,(ox+1) * 60,(oy+2) * 60),true);
			}
			break;
		case 'a':
		case 'A':
			odirect = 3;
			InvalidateRect(CRect(ox * 60,oy * 60,(ox+1) * 60,(oy+1) * 60),true);
			if(outmap[oy][ox-1] == 0 && ox > 0)
			{
				outmap[oy][ox] =0 ;
				outmap[oy][ox-1] = 1;
				InvalidateRect(CRect((ox-1) * 60,oy * 60,(ox+1) * 60,(oy+1) * 60),true);
			}
			break;
		case 'd':
		case 'D':
			odirect = 4;
			InvalidateRect(CRect(ox * 60,oy * 60,(ox+1) * 60,(oy+1) * 60),true);
			if(outmap[oy][ox+1] == 0 && ox < 14)
			{
				outmap[oy][ox] =0 ;
				outmap[oy][ox+1] = 1;
				InvalidateRect(CRect(ox * 60,oy * 60,(ox+2) * 60,(oy+1) * 60),true);
			}
			break;
		case VK_RETURN:
			if( ox== 7 && oy == 12 && odirect == 4)
			{
				mciSendString("close Theme3.mp3",0,0,0);
				this->SetWindowPos(NULL,0,0,920,940,0);
				mciSendString("play battle1.mp3 repeat",0,0,0);
				gameStartFlag = 3;
				mazeFlag = 0;
				Invalidate();
			}
			else if( ox== 9 && oy == 9 && odirect == 2)
			{
				mciSendString("close Theme3.mp3",0,0,0);
				this->SetWindowPos(NULL,0,0,920,940,0);
				mciSendString("play Theme2.mp3 repeat",0,0,0);
				gameStartFlag = 1;
				Invalidate();
			}
		}
	}
	else if(gameStartFlag == 1)
	{
		switch(nChar)
		{
		case 'w':
		case 'W':
			direct = 2;
			InvalidateRect(CRect(x * 60,y * 60,(x+1) * 60,(y+1) * 60),true);
			if(housemap[y-1][x] == 0  && y > 0)
			{
				housemap[y][x] =0 ;
				housemap[y-1][x] = 1;
				InvalidateRect(CRect(x * 60,(y-1) * 60,(x+1) * 60,(y+1) * 60),true);
			}
			break;
		case 's':
		case 'S':
			direct = 1;
			InvalidateRect(CRect(x * 60,y * 60,(x+1) * 60,(y+1) * 60),true);
			if(housemap[y+1][x] == 0 && y < 14)
			{
				housemap[y][x] =0 ;
				housemap[y+1][x] = 1;
				InvalidateRect(CRect(x * 60,y * 60,(x+1) * 60,(y+2) * 60),true);
			}
			break;
		case 'a':
		case 'A':
			direct = 4;
			InvalidateRect(CRect(x * 60,y * 60,(x+1) * 60,(y+1) * 60),true);
			if(housemap[y][x-1] == 0 && x > 0)
			{
				housemap[y][x] =0 ;
				housemap[y][x-1] = 1;
				InvalidateRect(CRect((x-1) * 60,y * 60,(x+1) * 60,(y+1) * 60),true);
			}
			break;
		case 'd':
		case 'D':
			direct = 3;
			InvalidateRect(CRect(x * 60,y * 60,(x+1) * 60,(y+1) * 60),true);
			if(housemap[y][x+1] == 0 && x < 14)
			{
				housemap[y][x] =0 ;
				housemap[y][x+1] = 1;
				InvalidateRect(CRect(x * 60,y * 60,(x+2) * 60,(y+1) * 60),true);
			}
			break;
		case VK_RETURN:
			if( x== 7 && y == 13 && direct == 1)
			{
				mciSendString("close Theme2.mp3",0,0,0);
				this->SetWindowPos(NULL,0,0,920,940,0);
				mciSendString("play Theme3.mp3 repeat",0,0,0);
				gameStartFlag = 2;
				Invalidate();
			}
		}
	}
}

void GameMazeWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFrameWnd::OnRButtonDown(nFlags, point);
	if(gameStartFlag  != 0&& gameStartFlag != 4 )
	{
		if(nFlags == MK_RBUTTON)
		{
			preFlag = gameStartFlag;
			gameStartFlag = 4;
			InvalidateRect(CRect(350,365,550,560),true);
		}
	}
}

void GameMazeWnd::OnAbout() 
{
	MessageBox("伊莉雅独自居住在荒漠边缘的小房子里。然而，某天醒来后，她发现房子外多了两座奇怪的石像，还有一条不知去往何处的地下通道。她会选择进入吗？嗯，这是个问题..." , "关于",MB_OK);
}

void GameMazeWnd::OnHelp() 
{
	MessageBox("人物移动：w向上，s向下，a向左，d向右。\n回车进入事件，右键进入菜单。","帮助",MB_OK);
}

void GameMazeWnd::OnMusic1() 
{
	sprintf(fileName,"stop Theme%d.mp3",musicFlag);
	mciSendString(fileName,0,0,0);
	mciSendString("play Theme1.mp3",0,0,0);
	musicFlag = 1;
}

void GameMazeWnd::OnMusic2() 
{
	sprintf(fileName,"stop Theme%d.mp3",musicFlag);
	mciSendString(fileName,0,0,0);
	mciSendString("play Theme2.mp3",0,0,0);
	musicFlag = 2;
}

void GameMazeWnd::OnMusic3() 
{
	sprintf(fileName,"stop Theme%d.mp3",musicFlag);
	mciSendString(fileName,0,0,0);
	mciSendString("play Theme3.mp3",0,0,0);
	musicFlag = 3;
}

void GameMazeWnd::OnnoMusic() 
{
	sprintf(fileName,"stop Theme%d.mp3",musicFlag);
	mciSendString(fileName,0,0,0);
}
