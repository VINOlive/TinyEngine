/* ========================================================== 
*	�� �� ����GameTest.cpp
*	��ǰ�汾��1.0.0
*	����ʱ�䣺2014-03-21 
*	�޸�ʱ�䣺2014-03-21
*	����˵������Ϸ��������� WinMain
*	���뻷����VS2008
*
*	��    ��������ʵ��111��
*	��    ��������
*	ѧ    �ţ�8000611063
*	ʵ�����ƣ�
*	ʵ����ţ�ʵ��3
*	ʵ��ص㣺ͼ���406
*	ʵ��ʱ�䣺2014-03-21 16:10-18:00
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

