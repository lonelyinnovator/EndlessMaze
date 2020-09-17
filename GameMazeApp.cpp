// GameMazeApp.cpp: implementation of the GameMazeApp class.
//
//////////////////////////////////////////////////////////////////////

#include "game.h"
#include "GameMazeApp.h"
#include "GameMazeWnd.h"



#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GameMazeApp theApp;

BOOL GameMazeApp::InitInstance()
{
	m_pMainWnd = new GameMazeWnd;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}


GameMazeApp::GameMazeApp()
{

}

GameMazeApp::~GameMazeApp()
{

}
