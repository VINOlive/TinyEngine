/* ========================================================== 
*	文 件 名：GameTest.cpp
*	当前版本：1.0.0
*	创建时间：2014-03-21 
*	修改时间：2014-03-21
*	功能说明：游戏引擎测试类 WinMain
*	编译环境：VS2008
*
*	班    级：东软实验111班
*	姓    名：方杰
*	学    号：8000611063
*	实验名称：
*	实验序号：实验3
*	实验地点：图书馆406
*	实验时间：2014-03-21 16:10-18:00
 ==========================================================*/
#include "T_Engine.h"
#include "T_Menu.h"

class GameTest:public T_Engine{
public:
	int wnd_width,wnd_height;
	T_Menu t_menu;


public:
	GameTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,WORD Icon, WORD SmIcon, int iWidth, int iHeight);
	~GameTest();

	void GameInit();                                       
	void GameLogic();
	void GameEnd();
	void GamePaint(HDC hdc);
	void GameKeyAction(int ActionType=KEY_SYS_NONE);
	void GameMouseAction(int x, int y, int ActionType);
};

