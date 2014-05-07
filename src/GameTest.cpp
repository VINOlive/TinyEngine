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
	//1.画海滩和海星
	//T_Graph *beach=new T_Graph(L".\\res\\beach.jpg");
	//beach->PaintImage(hdc,0,0,wnd_width,wnd_height);
	//T_Graph *star=new T_Graph(L".\\res\\star_slipper.png");
	//
	//T_Graph::PaintRegion(star->GetBmpHandle(),hdc,100,380,0,0,195,200,0.5);
	//T_Graph::PaintRegion(star->GetBmpHandle(),hdc,500,400,195,0,195,200,1);
	//T_Graph::PaintRegion(star->GetBmpHandle(),hdc,250,420,195*2,0,195,200,1);

	//2.画游动的鱼
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