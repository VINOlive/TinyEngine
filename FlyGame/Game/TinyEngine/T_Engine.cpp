#include "T_Engine.h"

HWND T_Engine::m_hWnd = NULL;
HINSTANCE T_Engine::m_hInstance = NULL;
T_Engine* T_Engine::pEngine = NULL;

T_Engine::T_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle, 
				   WORD Icon, WORD SmIcon, int iWidth, int iHeight)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&ptrGdiplusToken, &gdiplusStartupInput, NULL);
	pEngine = this;
	m_hInstance = hInstance;
	wndClass = szWindowClass;
	wndTitle = szTitle;
	wndWidth = iWidth;
	wndHeight = iHeight;
	m_bFullScreen = FALSE;
	wIcon = Icon;
	wSmIcon = SmIcon;
	p_disp = new T_Display();	
	for(int i=0; i<256; i++)
	{
		keys[i] = false;
	}
	bufferDC = CreateCompatibleDC(GetDC(m_hWnd));
	bufferBitmap = CreateCompatibleBitmap(GetDC(m_hWnd), wndWidth, wndHeight);
	SelectObject(bufferDC, bufferBitmap);
}

T_Engine::~T_Engine()
{
	delete p_disp;
	Gdiplus::GdiplusShutdown(ptrGdiplusToken);
}

LONG CALLBACK T_Engine::WndProc(HWND hWnd, UINT msg, 
								WPARAM wParam, LPARAM lParam)
{
	return pEngine->GameEvent(hWnd, msg, wParam, lParam);
}

BOOL T_Engine::GameWinInit()
{
	WNDCLASSEX gamewin;
	gamewin.cbSize			= sizeof(WNDCLASSEX); 
	gamewin.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	gamewin.lpfnWndProc		= WndProc;
	gamewin.cbClsExtra		= 0;
	gamewin.cbWndExtra		= 0;
	gamewin.hInstance		= m_hInstance;
	//gamewin.hIcon			= LoadIcon(m_hInstance, MAKEINTRESOURCE(wIcon));
	gamewin.hIcon			= LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
	gamewin.hCursor			= LoadCursor(NULL, IDC_ARROW);
	gamewin.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	gamewin.lpszMenuName	= NULL;
	gamewin.lpszClassName	= wndClass;
	//gamewin.hIconSm		= LoadIcon(m_hInstance, MAKEINTRESOURCE(wSmIcon));
	gamewin.hIconSm			= LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_ICON1));
	if(!RegisterClassEx(&gamewin)) return FALSE;
	scrnWidth=GetSystemMetrics(SM_CXFULLSCREEN);    
	scrnHeight=GetSystemMetrics(SM_CYFULLSCREEN);
	int x = (scrnWidth-wndWidth)/2;
	int y = (scrnHeight-wndHeight)/2;
	m_hWnd = CreateWindow(	wndClass, wndTitle, 
							WS_OVERLAPPEDWINDOW & ~WS_SIZEBOX,
							x, y, wndWidth+6, wndHeight+28,
							NULL, NULL, m_hInstance, NULL  );
	if (!m_hWnd) return FALSE;
	if(m_bFullScreen == TRUE)
	{
		GetWindowRect(m_hWnd, &m_rcOld);
		style = GetWindowLong(m_hWnd,GWL_STYLE); 
		ex_style = GetWindowLong(m_hWnd, GWL_EXSTYLE); 
		p_disp->SaveMode();
		HWND hDesktop;   
		RECT rc;  
		hDesktop = GetDesktopWindow();  
		GetWindowRect(hDesktop, &rc);
		p_disp->ChangeMode(wndWidth, wndHeight);
		SetWindowLong(m_hWnd, GWL_EXSTYLE, WS_EX_WINDOWEDGE);
		SetWindowLong(m_hWnd, GWL_STYLE, WS_BORDER);
		SetWindowPos(m_hWnd, HWND_TOP, -1, -1, 
			         rc.right, rc.bottom, SWP_SHOWWINDOW);
	}
	ShowWindow(m_hWnd, SW_NORMAL);
	UpdateWindow(m_hWnd);
	return TRUE;
}

void T_Engine::SubKeyAction(WPARAM wParam)
{
	int x, y;
	if(wParam == VK_ESCAPE)
	{
		m_bFullScreen=!m_bFullScreen;
		if(!m_bFullScreen)
		{
			p_disp->ResetMode();
			SetWindowLong(m_hWnd, GWL_STYLE, style);
			SetWindowLong(m_hWnd, GWL_EXSTYLE, ex_style);
			int oldWidth=m_rcOld.right-m_rcOld.left;
			int oldHeight=m_rcOld.bottom-m_rcOld.top;
			x = m_rcOld.left;
			y = m_rcOld.top;
			SetWindowPos(m_hWnd, HWND_NOTOPMOST, x, y, 
				         oldWidth, oldHeight, SWP_SHOWWINDOW);
		}
		else
		{
			ShowCursor(true);
			if(IDOK==MessageBox(NULL,L"你确定要退出吗？", wndTitle, 
				MB_OKCANCEL|MB_ICONQUESTION))
			{
				DestroyWindow(m_hWnd);
			}
		}
	}
}

LONG T_Engine::GameEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
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
		m_hWnd = hWnd;
		GameInit();
		return 0;
	case WM_LBUTTONDOWN:
		GameMouseAction(LOWORD(lParam), HIWORD(lParam), MOUSE_LCLICK);
		return 0;
	case WM_MOUSEMOVE:
		GameMouseAction(LOWORD(lParam), HIWORD(lParam), MOUSE_MOVE);
		return 0;
	case WM_KEYDOWN:
		keys[wParam] = true;
		GameKeyAction(KEY_DOWN);
		SubKeyAction(wParam);
		return 0;
	case WM_KEYUP:
		keys[wParam] = false;
		GameKeyAction(KEY_UP);
		return 0;
	case WM_NCLBUTTONDBLCLK: 
		if(HTCAPTION==wParam)   
		{   
			return 0;   
		}  
	case WM_SYSCOMMAND:
		if(wParam == SC_MAXIMIZE)
		{
			m_bFullScreen=!m_bFullScreen;
			if(m_bFullScreen)
			{
				GetWindowRect(hWnd, &m_rcOld);
				style = GetWindowLong(hWnd,GWL_STYLE); 
				ex_style = GetWindowLong(hWnd, GWL_EXSTYLE); 
				p_disp->SaveMode();
				//全屏幕显示
				HWND hDesktop;   
				RECT rc;  
				hDesktop = GetDesktopWindow();
				GetWindowRect(hDesktop, &rc);
				p_disp->ChangeMode(wndWidth, wndHeight);
				SetWindowLong(hWnd, GWL_EXSTYLE, WS_EX_WINDOWEDGE);
				SetWindowLong(hWnd, GWL_STYLE, WS_BORDER);
				SetWindowPos(hWnd, HWND_TOP, -1, -1, rc.right, rc.bottom, SWP_SHOWWINDOW);
			}
		}
		else if(wParam == SC_CLOSE)
		{
			if(IDOK==MessageBox(NULL,L"你确定要退出吗？", 
				wndTitle, MB_OKCANCEL|MB_ICONQUESTION))
			{
				DestroyWindow(hWnd);
			}
		}
		else
		{
			return DefWindowProc(hWnd, WM_SYSCOMMAND, wParam,lParam); 
		}
		return 0;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return 0;
	case WM_PAINT:
		PAINTSTRUCT ps;
		hdc=BeginPaint(hWnd,&ps);
		GamePaint(hdc);
		EndPaint(hWnd,&ps);	
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);	
}

void T_Engine::StartEngine()
{
	MSG msg;
	static int TickElapsed = 0;
	int nowTick;
	GameWinInit();
	srand( (unsigned)time( NULL ) );
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT) break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (!GetSleep())
			{
				nowTick = GetTickCount();
				if (nowTick > TickElapsed)
				{
					TickElapsed = nowTick + GetInterval();
					GameLogic();
					GamePaint(bufferDC);
					HDC hDC = GetDC(m_hWnd);
					BitBlt(hDC, 0, 0, WIN_WIDTH, WIN_HEIGHT, bufferDC, 0, 0, SRCCOPY);
					ReleaseDC(m_hWnd, hDC);
				}
			}
		}
	}
	pEngine->GameEnd();
}