/* ========================================================== 
*	文 件 名：T_Engine.h
*	当前版本：1.0.0
*	创建时间：2014-03-14 
*	修改时间：2014-03-14
*	功能说明：游戏引擎类声明
*	编译环境：VS2008
*
*	班    级：东软实验111班
*	姓    名：方杰
*	学    号：8000611063
*	实验名称：
*	实验序号：实验3
*	实验地点：图书馆406
*	实验时间：2014-03-14 16:10-18:00
 ==========================================================*/
#pragma once
#include "T_Config.h"
#include "T_Display.h"

class T_Engine
{
public:
	//窗口句柄
	static HWND m_hWnd;
	//实例句柄
	static HINSTANCE m_hInstance;
	//本类实例指针
	static T_Engine* pEngine;

protected:
	//GDI+引用
	ULONG_PTR ptrGdiplusToken;
////窗口类名称
	LPCTSTR wndClass;
	//窗口标题
	LPCTSTR wndTitle;	
	//窗口宽高
	int wndWidth, wndHeight;
	//屏幕分辨率宽高
	int scrnWidth, scrnHeight;
	//程序大小图标
	WORD wIcon, wSmIcon;
	//是否全屏的标志位
	BOOL m_bFullScreen;
	//当前窗口尺寸
	RECT m_rcOld;
	//窗口基本风格
	LONG style;
	//窗口扩展风格
	LONG ex_style;
	//T_Display指针
	T_Display* p_disp;
	//游戏帧频间隔(一帧的时间间隔)
	int IntervalTime;
	//屏幕是否睡眠
	BOOL isAsleep;

	//内存缓冲位图
	HBITMAP bufferBitmap;
	//内存缓冲设备
	HDC bufferDC;
	//键盘数组
	bool keys[256];
	//游戏状态
	int GameState;

public:
	T_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,WORD Icon = NULL, WORD SmIcon = NULL,int iWidth = WIN_WIDTH, int iHeight = WIN_HEIGHT);
	virtual ~T_Engine();

	//获取游戏帧频数据
	int GetInterval() { return IntervalTime; }
	//设置游戏帧频
	void SetFrame(int iTime) { IntervalTime = 1000 / iTime; }
	//获取是否为睡眠状态
	BOOL GetSleep() { return isAsleep; }
	//设置睡眠状态
	void SetSleep(BOOL asleep) { isAsleep = asleep; }
	//设置全屏状态
	void SetFullScreen(BOOL isFull){ m_bFullScreen = isFull; }
	//获取某个按键状态
	bool CheckKey(WPARAM wParam){ return keys[wParam]; }


	//附加键行为，如:esc
	void SubKeyAction(WPARAM wParam);
	//游戏窗口初始化
	BOOL GameWinInit();
	//消息处理函数（供 回调函数调用）
	LONG GameEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//开启引擎
	void StartEngine();
	//消息回调函数
	static LONG CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	virtual void GameInit()=0;	//游戏初始化	
	virtual void GameLogic()=0;	//游戏逻辑处理
	virtual void GameEnd()=0;  //游戏结束
	virtual void GamePaint(HDC hdc)=0;  //根据GAME_STATE显示游戏界面
	virtual void GameKeyAction(int ActionType=KEY_SYS_NONE)=0; //处理按键消息
	virtual void GameMouseAction(int x, int y, int ActionType)=0;	 //处理鼠标消息
};
