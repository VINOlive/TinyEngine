/* ========================================================== 
*	文 件 名：T_Engine.cpp
*	当前版本：1.0.0
*	创建时间：2014-03-14 
*	修改时间：2014-03-14
*	功能说明：游戏引擎类实现
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
#include "T_Engine.h"
HWND T_Engine::m_hWnd=NULL;
HINSTANCE T_Engine::m_hInstance=NULL;
T_Engine* T_Engine::pEngine=NULL;

/*引擎类启动过程：
 *1.先调用构造函数
 *2.然后StartEngine
*/
T_Engine::T_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,WORD Icon, WORD SmIcon, 
		 int iWidth, int iHeight)
{
	//GDI+设置
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&ptrGdiplusToken,&gdiplusStartupInput,NULL);

	//基本的参数设置
	pEngine=this;
	m_hInstance=hInstance;
	wndClass=szWindowClass;
	wndTitle=szTitle;
	wndWidth=iWidth;
	wndHeight=iHeight;
	m_bFullScreen=FALSE;
	wIcon=Icon;
	wSmIcon=SmIcon;
	
	p_disp=new T_Display();
	//所有键盘 按键状态为 “未按下”
	for(int i=0;i<256;i++)
		keys[i]=false;

	bufferDC=CreateCompatibleDC(GetDC(m_hWnd));
	bufferBitmap=CreateCompatibleBitmap(GetDC(m_hWnd),wndWidth,wndHeight);//该位图宽高与窗口大小相同
	//将位图与设备关联起来
	SelectObject(bufferDC,bufferBitmap);
}

//引擎类析构函数
T_Engine::~T_Engine()
{
	delete p_disp;
	Gdiplus::GdiplusShutdown(ptrGdiplusToken);
}
//启动引擎
void T_Engine::StartEngine()
{
	MSG msg;
	static int TickElaspsed=0;
	int nowTick;
	//现初始化窗口
	GameWinInit();
	srand((unsigned)time(NULL));
	while (TRUE)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message==WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if(!GetSleep())
			{
				//程序运行到目前的时间
				nowTick=GetTickCount();
				if(nowTick>TickElaspsed)
				{
					//程序运行到现在+帧频时间
					TickElaspsed=nowTick+GetInterval();
					GameLogic();
					GamePaint(bufferDC);
					HDC hDC=GetDC(m_hWnd);
					BitBlt(hDC,0,0,WIN_WIDTH,WIN_HEIGHT,bufferDC,0,0,SRCCOPY);
					ReleaseDC(m_hWnd,hDC);
				}
			}
		}
	}
	pEngine->GameEnd();
}

//主界面窗口绘制，供StartEngine调用
BOOL T_Engine::GameWinInit()
{
	WNDCLASSEX gamewin;
	gamewin.cbSize=sizeof(WNDCLASSEX);
	gamewin.style=CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
	gamewin.lpfnWndProc=WndProc;
	gamewin.cbClsExtra=0;
	gamewin.cbWndExtra=0;
	gamewin.hInstance=m_hInstance;
	gamewin.hIcon=LoadIcon(m_hInstance,MAKEINTRESOURCE(wIcon));
	gamewin.hCursor=LoadCursor(NULL,IDC_ARROW);
	gamewin.hbrBackground=(HBRUSH)(COLOR_WINDOW+1);
	gamewin.lpszMenuName=NULL;
	gamewin.lpszClassName=wndClass;
	gamewin.hIconSm=LoadIcon(m_hInstance,MAKEINTRESOURCE(wSmIcon));
	if(!RegisterClassEx(&gamewin))
		return FALSE;             

	//屏幕分辨率可以通过系统函数得到
	scrnWidth=GetSystemMetrics(SM_CXFULLSCREEN);
	scrnHeight=GetSystemMetrics(SM_CYFULLSCREEN);

	int x=(scrnWidth-wndWidth)/2;
	int y=(scrnHeight-wndHeight)/2;
	//x,y 指示窗口的位置
	m_hWnd=CreateWindow(wndClass,wndTitle,WS_OVERLAPPEDWINDOW&~WS_SIZEBOX,x,y,wndWidth+6,wndHeight+28
		,NULL,NULL,m_hInstance,NULL);
	if(!m_hWnd)
		return FALSE;

	if(m_bFullScreen==TRUE)
	{
		GetWindowRect(m_hWnd,&m_rcOld);
		style=GetWindowLong(m_hWnd,GWL_STYLE);
		ex_style=GetWindowLong(m_hWnd,GWL_EXSTYLE);

		//先保存全屏前的设置
		p_disp->SaveMode();
		HWND hDesktop;
		RECT rc;
		hDesktop=GetDesktopWindow();
		GetWindowRect(hDesktop,&rc);
		p_disp->ChangeMode(wndWidth,wndHeight);
		SetWindowLong(m_hWnd,GWL_EXSTYLE,WS_EX_WINDOWEDGE);
		SetWindowLong(m_hWnd,GWL_STYLE,WS_BORDER);
		SetWindowPos(m_hWnd,HWND_TOP,-1,-1,rc.right,rc.bottom,SWP_SHOWWINDOW);
	}

	ShowWindow(m_hWnd,SW_NORMAL);
	UpdateWindow(m_hWnd);
	return true;
}

//处理附加按键消息处理
void T_Engine::SubKeyAction(WPARAM wParam)
{
	int x,y;
	if(wParam==VK_ESCAPE)//退出
	{

		m_bFullScreen=!m_bFullScreen;
		if(!m_bFullScreen)
		{
			p_disp->ResetMode();
			SetWindowLong(m_hWnd,GWL_STYLE,style);
			SetWindowLong(m_hWnd,GWL_EXSTYLE,ex_style);
			int oldwidth=m_rcOld.right-m_rcOld.left;
			int oldheight=m_rcOld.bottom-m_rcOld.top;
			x=m_rcOld.left;
			y=m_rcOld.top;

			SetWindowPos(m_hWnd,HWND_NOTOPMOST,x,y,oldwidth,oldheight,SWP_SHOWWINDOW);
		}
		else
		{
			ShowCursor(true);
			if(IDOK==MessageBox(NULL,L"你确定要退出吗?",wndTitle,MB_OKCANCEL|MB_ICONQUESTION))
			{
				DestroyWindow(m_hWnd);
			}
		}
	}
}

LONG CALLBACK T_Engine::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return pEngine->GameEvent(hWnd,msg,wParam,lParam);
}
//消息处理，供回调函数调用
LONG T_Engine::GameEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	int x,y;
	HDC hdc;
	switch (msg)
	{
		case WM_SETFOCUS:
			SetSleep(FALSE);
			return 0;
		case WM_KILLFOCUS:
			SetSleep(TRUE);
			return 0;
		case WM_CREATE:
			m_hWnd=hWnd;
			GameInit();
			return 0;
		case WM_LBUTTONDOWN:
			GameMouseAction(LOWORD(lParam),HIWORD(lParam),MOUSE_LCLICK);
			return 0;
		case WM_MOUSEMOVE:
			GameMouseAction(LOWORD(lParam),HIWORD(lParam),MOUSE_MOVE);
			return 0;
		case WM_KEYDOWN:
			keys[wParam]=true;
			GameKeyAction(KEY_DOWN);
			SubKeyAction(wParam);
			return 0;
		case WM_KEYUP:
			keys[wParam]=false;
			GameKeyAction(KEY_UP);
			return 0;	
		case WM_NCLBUTTONDBLCLK:
			if(HTCAPTION==wParam)
				return 0;
		case WM_SYSCOMMAND://window menu 
			if(wParam==SC_MAXIMIZE)//最大化
			{
				m_bFullScreen=!m_bFullScreen;
				if(m_bFullScreen)//从非全屏到全屏
				{
					GetWindowRect(m_hWnd,&m_rcOld);
					style=GetWindowLong(m_hWnd,GWL_STYLE);
					ex_style=GetWindowLong(m_hWnd,GWL_EXSTYLE);
					p_disp->SaveMode();

					HWND hDesktop;
					RECT rc;
					hDesktop=GetDesktopWindow();
					GetWindowRect(hDesktop,&rc);

					p_disp->ChangeMode(wndWidth,wndHeight);

					SetWindowLong(m_hWnd,GWL_EXSTYLE,WS_EX_WINDOWEDGE);
					SetWindowLong(m_hWnd,GWL_STYLE,WS_BORDER);
					SetWindowPos(m_hWnd,HWND_TOP,-1,-1,rc.right,rc.bottom,SWP_SHOWWINDOW);
				}
			}
			else if(wParam==SC_CLOSE)//关闭
			{
				if(IDOK==MessageBox(NULL,L"你确定要退出吗?",wndTitle,MB_OKCANCEL|MB_ICONQUESTION))
				{
					DestroyWindow(m_hWnd);
				}	
			}
			else
			{
				return DefWindowProc(hWnd,WM_SYSCOMMAND,wParam,lParam);
			}
			return 0;
		case WM_CLOSE:
			DestroyWindow(hWnd);//系统函数
			return 0;
		case WM_PAINT:
			PAINTSTRUCT ps;
			hdc=BeginPaint(hWnd,&ps);//系统函数
			GamePaint(hdc);
			EndPaint(hWnd,&ps);//系统函数
			return 0;
		case WM_DESTROY:
			PostQuitMessage(0);//系统函数
			return 0;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

