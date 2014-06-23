//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#pragma once
#include "T_Config.h"
#include "T_Display.h"
#include "..\\resource.h"

class T_Engine
{
public:
	static HWND m_hWnd;
	static HINSTANCE m_hInstance;
	static T_Engine* pEngine;

protected:
	ULONG_PTR ptrGdiplusToken;

	LPCTSTR wndClass;
	LPCTSTR wndTitle;	
	int wndWidth, wndHeight;
	int scrnWidth, scrnHeight;
	WORD wIcon, wSmIcon;
	BOOL m_bFullScreen;

	RECT m_rcOld;
	LONG style;
	LONG ex_style;
	T_Display* p_disp;

	int IntervalTime;
	BOOL isAsleep;

	HBITMAP bufferBitmap;
	HDC bufferDC;

	bool keys[256];
	int GameState;

public:
	T_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle, 
		     WORD Icon = NULL, WORD SmIcon = NULL, 
		     int iWidth = WIN_WIDTH, int iHeight = WIN_HEIGHT);
	virtual ~T_Engine();

	int GetInterval() { return IntervalTime; }
	void SetFrame(int iTime) { IntervalTime = 1000 / iTime; }
	BOOL GetSleep() { return isAsleep; }
	void SetSleep(BOOL asleep) { isAsleep = asleep; }
	void SetFullScreen(BOOL isFull){ m_bFullScreen = isFull; }
	bool CheckKey(WPARAM wParam){ return keys[wParam]; }
	void SubKeyAction(WPARAM wParam);
	static LONG CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );
	BOOL GameWinInit();
	LONG GameEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void StartEngine();

public:
	virtual void GameInit()=0;		
	virtual void GameLogic()=0;		
	virtual void GameEnd()=0;
	virtual void GamePaint(HDC hdc)=0;
	virtual void GameKeyAction(int ActionType=KEY_SYS_NONE)=0;
	virtual void GameMouseAction(int x, int y, int ActionType)=0;	
};
