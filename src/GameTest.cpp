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
GameTest::GameTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,WORD Icon, WORD SmIcon, int iWidth, int iHeight):T_Engine(hInstance,szWindowClass,szTitle,Icon,SmIcon,iWidth,iHeight)
{
	wnd_width=iWidth;
	wnd_height=iHeight;
}
GameTest::~GameTest(){}

void GameTest::GameInit(){
	int x=0,y=0;
	int btn_width=0,btn_height=0;
	Color normalclr,focusclr;
	wstring menuItems[]={L"新游戏",L"关于",L"帮助",L"退出游戏"};
	t_menu.SetMenuBkg(L".\\res\\menubkg.jpg");
	btn_width=250;
	btn_height=70;
	normalclr=Color::Red;
	focusclr=Color::White;
	t_menu.SetBtnBmp(L".\\res\\button.png",btn_width,btn_height);

	MENU_INFO menuinfo;
	menuinfo.align=1;
	menuinfo.space=MENU_SPACE;
	menuinfo.width=btn_width;
	menuinfo.height=btn_height;
	menuinfo.fontName=L"黑体";
	menuinfo.isBold=true;
	menuinfo.normalTextColor=normalclr;
	menuinfo.focusTextColor=focusclr;
	t_menu.SetMenuInfo(menuinfo);
	for(int i=0;i<4;i++)
	{
		x=(wnd_width-btn_width)/2;
		y=i*(btn_height+MENU_SPACE)+(wnd_height-4*btn_height-3*MENU_SPACE)/2;
		MENUITEM mItem;
		mItem.pos.x=x;
		mItem.pos.y=y;
		mItem.ItemName=menuItems[i];
		t_menu.AddMenuItem(mItem);
	}
	GameState=GAME_START;
}                                       
void GameTest::GameLogic(){}
void GameTest::GameEnd(){}
void GameTest::GameKeyAction(int ActionType){
	if(GetAsyncKeyState(VK_UP)<0)
		t_menu.MenuKeyDown(VK_UP);
	if(GetAsyncKeyState(VK_DOWN)<0)
		t_menu.MenuKeyDown(VK_DOWN);
	if(GetAsyncKeyState(VK_LEFT)<0)
		t_menu.MenuKeyDown(VK_LEFT);
	if(GetAsyncKeyState(VK_RIGHT)<0)
		t_menu.MenuKeyDown(VK_RIGHT);

	if(GetAsyncKeyState(VK_RETURN)<0)
	{
		if(t_menu.GetMenuIndex()>=0)
		{
			switch(t_menu.GetMenuIndex())
			{
				case 0:
					break;
				case 1:
					break;
				case 2:
					break;
				case 3:SendMessage(m_hWnd,WM_SYSCOMMAND,SC_CLOSE,0);
					break;
			}
		}
	}
}
void GameTest::GameMouseAction(int x, int y, int ActionType){
	if (ActionType ==MOUSE_MOVE&&GameState!=GAME_RUN)
	{
		t_menu.MenuMouseMove(x,y);
	}
	if(ActionType==MOUSE_LCLICK)
	{
		if(GameState==GAME_START)
		{
			int index=t_menu.MenuMouseClick(x,y);
			if(index>0)
			{
				switch(index)
				{
					case 0:
						break;
					case 1:
						break;
					case 2:
						break;
					case 3:SendMessage(m_hWnd,WM_SYSCOMMAND,SC_CLOSE,0);
						break;
				}
			}
		}

	}
}

void GameTest::GamePaint(HDC hdc)
{
	if(GAME_START==GameState)
	{
		t_menu.DrawMenu(hdc);
		RectF textRec;
		textRec.X=0.00;
		textRec.Y=0.00;
		textRec.Width=(float)wnd_width;
		textRec.Height=(float)wnd_height/4;
		T_Graph::PaintText(hdc,textRec,L"游戏菜单",36,L"",Color::White,FontStyleBold,StringAlignmentCenter);

	}
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