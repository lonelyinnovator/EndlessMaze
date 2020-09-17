#include <afx.h>

#if !defined(AFX_GAMEMAZEWND_H__B6D2C600_436A_4C31_8A73_571ECD571EC5__INCLUDED_)
#define AFX_GAMEMAZEWND_H__B6D2C600_436A_4C31_8A73_571ECD571EC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameMazeWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GameMazeWnd frame

#include"resource.h"

class GameMazeWnd : public CFrameWnd
{
	DECLARE_DYNCREATE(GameMazeWnd)
public:
	GameMazeWnd(); 
	CreateMaze(int maze[17][17], int dx, int dy);

// Attributes
public:

// Operations

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GameMazeWnd)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~GameMazeWnd();

	// Generated message map functions
	//{{AFX_MSG(GameMazeWnd)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnAbout();
	afx_msg void OnHelp();
	afx_msg void OnMusic1();
	afx_msg void OnMusic2();
	afx_msg void OnMusic3();
	afx_msg void OnnoMusic();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CDC *mdc;//定义一个设备缓冲区
	CBitmap *bitmap[4];
	CBitmap *obitmap[4];
	CBitmap *mbitmap[4];
	CBitmap  *housefloor[15][15];
	CBitmap *bkgd;
	CBitmap *wall[15][15];
	CBitmap *stair[3];
	CBitmap *bed[2];
	CBitmap *shelf[2][6];
	CBitmap *godness[2];
	CBitmap *chair;
	CBitmap *outfloor[15][15];
	CBitmap *houseroof[15][15];
	CBitmap *outwall[15][15];
	CBitmap *window;
	CBitmap *door[2];
	CBitmap *outlgod[2];
	CBitmap *outrgod[2];
	CBitmap *down;
	CBitmap *grass[15][15];
	CBitmap *leaf[15][15];
	CBitmap *tree[4][4];
	CBitmap *flower[4][4];
	CBitmap *rock[4][4];
	CBitmap *cactus[4][4];
	CBitmap *bean[4][4];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMEMAZEWND_H__B6D2C600_436A_4C31_8A73_571ECD571EC5__INCLUDED_)
