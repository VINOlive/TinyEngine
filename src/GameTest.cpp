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
#include "T_Graph.h"

GameTest::GameTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,WORD Icon, WORD SmIcon, int iWidth, int iHeight):T_Engine(hInstance,szWindowClass,szTitle,Icon,SmIcon,iWidth,iHeight)
{
	wnd_width=iWidth;
	wnd_height=iHeight;
}
GameTest::~GameTest(){}                                    
void GameTest::GameLogic(){}
void GameTest::GameEnd(){}
void GameTest::GameKeyAction(int ActionType){}
void GameTest::GameMouseAction(int x, int y, int ActionType){}

void GameTest::GameInit(){
	frameCount=0;
}   
void GameTest::GamePaint(HDC hdc)
{
	//1.����̲�ͺ���
	//T_Graph *beach=new T_Graph(L".\\res\\beach.jpg");
	//beach->PaintImage(hdc,0,0,wnd_width,wnd_height);
	//T_Graph *star=new T_Graph(L".\\res\\star_slipper.png");
	//
	//T_Graph::PaintRegion(star->GetBmpHandle(),hdc,100,380,0,0,195,200,0.5);
	//T_Graph::PaintRegion(star->GetBmpHandle(),hdc,500,400,195,0,195,200,1);
	//T_Graph::PaintRegion(star->GetBmpHandle(),hdc,250,420,195*2,0,195,200,1);

	//2.���ζ�����
	T_Graph *sea=new T_Graph(L".\\res\\seafloor.jpg");
	sea->PaintImage(hdc,0,0,wnd_width,wnd_height);

	T_Graph *fish1=new T_Graph(L".\\res\\blue.png");
	T_Graph::PaintRegion(fish1->GetBmpHandle(),hdc,200,280,143*frameCount,0,143,84,1);
	T_Graph *fish2=new T_Graph(L".\\res\\red.png");
	T_Graph::PaintRegion(fish2->GetBmpHandle(),hdc,330,360,143*frameCount,0,143,84,1);
	T_Graph::PaintRegion(fish2->GetBmpHandle(),hdc,350,370,143*frameCount,0,143,84,1.2);
	T_Graph::PaintRegion(fish2->GetBmpHandle(),hdc,340,380,143*frameCount,0,143,84,1.5);
	T_Graph::PaintRegion(fish2->GetBmpHandle(),hdc,400,390,143*frameCount,0,143,84,1);
	T_Graph::PaintRegion(fish2->GetBmpHandle(),hdc,420,390,143*frameCount,0,143,84,1);

	frameCount++;
	if(frameCount>=19)
		frameCount=0;
}


int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd )
{
	LPCTSTR WinTitle=L"T_Engine";
	GameTest* test=new GameTest(hInstance,WIN_CLASS,WinTitle,NULL,NULL,800,600);
	T_Engine::pEngine=test;
	test->SetFrame(10);
	test->StartEngine();
	return TRUE;
}