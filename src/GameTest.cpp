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
#include "GameTest.h"
GameTest::GameTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,WORD Icon, WORD SmIcon, int iWidth, int iHeight):T_Engine(hInstance,szWindowClass,szTitle,Icon,SmIcon,iWidth,iHeight)
{
	wnd_width=iWidth;
	wnd_height=iHeight;
}
GameTest::~GameTest(){}

void GameTest::GameInit(){}                                       
void GameTest::GameLogic(){}
void GameTest::GameEnd(){}
void GameTest::GameKeyAction(int ActionType){}
void GameTest::GameMouseAction(int x, int y, int ActionType){}
void GameTest::GamePaint(HDC hdc)
{
	HGDIOBJ mbrush,oldbrush;
	HGDIOBJ mpen,oldpen;
	RECT mrect;
	int cell_width=wnd_width/20;
	int cell_height=wnd_height/20;
	for(int r=0;r<20;r++)
		for(int c=0;c<20;c++)
		{
			mpen=CreatePen(PS_SOLID,0,RGB(240,240,240));
			oldpen=SelectObject(hdc,mpen);
			mbrush=CreateSolidBrush(RGB(rand()%255,rand()%255,rand()%255));
			oldbrush=SelectObject(hdc,mbrush);
			mrect.left=c*cell_width;
			mrect.right=mrect.left+cell_width;
			mrect.top=r*cell_height;
			mrect.bottom=mrect.top+cell_height;
			FillRect(hdc,&mrect,(HBRUSH)mbrush);
			Rectangle(hdc,mrect.left,mrect.top,mrect.right,mrect.bottom);
			SelectObject(hdc,oldbrush);
			DeleteObject(mbrush);
			SelectObject(hdc,oldpen);
			DeleteObject(mpen);
		}
}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd )
{
	LPCTSTR WinTitle=L"T_Engine";
	GameTest* test=new GameTest(hInstance,WIN_CLASS,WinTitle,NULL,NULL,800,600);
	T_Engine::pEngine=test;
	test->SetFrame(1);
	test->StartEngine();
	return TRUE;
}