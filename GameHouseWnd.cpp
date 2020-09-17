// GameHouseWnd.cpp : implementation file
//

//#include "stdafx.h"
#include "game.h"
#include "GameHouseWnd.h"
#include "GameMazeApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GameHouseWnd

IMPLEMENT_DYNCREATE(GameHouseWnd, CFrameWnd)

GameHouseWnd::GameHouseWnd()
{
	if(gameStartFlag == 1)
	{
		Create(NULL,"Fantasy Maze Ver1.0");
		this->SetWindowPos(NULL,0,0,1200,1200,0);
	}
}

GameHouseWnd::~GameHouseWnd()
{
}


BEGIN_MESSAGE_MAP(GameHouseWnd, CFrameWnd)
	//{{AFX_MSG_MAP(GameHouseWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GameHouseWnd message handlers
