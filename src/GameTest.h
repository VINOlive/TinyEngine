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
#include ".\\TinyEngine\T_Engine.h"
#include ".\\TinyEngine\T_Menu.h"
#include ".\\TinyEngine\T_Sprite.h"
#include ".\\TinyEngine\T_AI.h"

typedef struct{
	wstring filename;
	int wframe;
	int hframe;
}FISHINFO;


class GameTest:public T_Engine{

private:
	static FISHINFO fish[7];
	static const int NUM=10;
	static int sequence[20];
	int wnd_width,wnd_height;
	POINT mouse_pt;
	T_Graph*back;
	T_Sprite*player;
	T_Sprite* npc[NUM];
	T_AI* sp_ai;

	void LoadPlayer();
	void LoadNpc(int num);
	void UpdateFrames();
	void UpdatePos();


public:
	GameTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,WORD Icon, WORD SmIcon, int iWidth, int iHeight);
	virtual ~GameTest(void);

	void GameInit();                                       
	void GameLogic();
	void GameEnd();
	void GamePaint(HDC hdc);
	void GameKeyAction(int ActionType=KEY_SYS_NONE);
	void GameMouseAction(int x, int y, int ActionType);
};

