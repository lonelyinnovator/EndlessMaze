// GameStartWnd.cpp : implementation file
//

//#include "stdafx.h"
#include "game.h"
#include "GameStartWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GameStartWnd

IMPLEMENT_DYNCREATE(GameStartWnd, CFrameWnd)

GameStartWnd::GameStartWnd()
{
	Create(NULL,"MazeV1.0");
	this->SetWindowPos(NULL,0,0,400,400,0);
	Sleep(200);
	this->CloseWindow();
}

GameStartWnd::~GameStartWnd()
{
}


BEGIN_MESSAGE_MAP(GameStartWnd, CFrameWnd)
	//{{AFX_MSG_MAP(GameStartWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GameStartWnd message handlers
