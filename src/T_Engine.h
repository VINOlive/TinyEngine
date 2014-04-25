/* ========================================================== 
*	�� �� ����T_Engine.h
*	��ǰ�汾��1.0.0
*	����ʱ�䣺2014-03-14 
*	�޸�ʱ�䣺2014-03-14
*	����˵������Ϸ����������
*	���뻷����VS2008
*
*	��    ��������ʵ��111��
*	��    ��������
*	ѧ    �ţ�8000611063
*	ʵ�����ƣ�
*	ʵ����ţ�ʵ��3
*	ʵ��ص㣺ͼ���406
*	ʵ��ʱ�䣺2014-03-14 16:10-18:00
 ==========================================================*/
#pragma once
#include "T_Config.h"
#include "T_Display.h"

class T_Engine
{
public:
	//���ھ��
	static HWND m_hWnd;
	//ʵ�����
	static HINSTANCE m_hInstance;
	//����ʵ��ָ��
	static T_Engine* pEngine;

protected:
	//GDI+����
	ULONG_PTR ptrGdiplusToken;
////����������
	LPCTSTR wndClass;
	//���ڱ���
	LPCTSTR wndTitle;	
	//���ڿ��
	int wndWidth, wndHeight;
	//��Ļ�ֱ��ʿ��
	int scrnWidth, scrnHeight;
	//�����Сͼ��
	WORD wIcon, wSmIcon;
	//�Ƿ�ȫ���ı�־λ
	BOOL m_bFullScreen;
	//��ǰ���ڳߴ�
	RECT m_rcOld;
	//���ڻ������
	LONG style;
	//������չ���
	LONG ex_style;
	//T_Displayָ��
	T_Display* p_disp;
	//��Ϸ֡Ƶ���(һ֡��ʱ����)
	int IntervalTime;
	//��Ļ�Ƿ�˯��
	BOOL isAsleep;

	//�ڴ滺��λͼ
	HBITMAP bufferBitmap;
	//�ڴ滺���豸
	HDC bufferDC;
	//��������
	bool keys[256];
	//��Ϸ״̬
	int GameState;

public:
	T_Engine(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,WORD Icon = NULL, WORD SmIcon = NULL,int iWidth = WIN_WIDTH, int iHeight = WIN_HEIGHT);
	virtual ~T_Engine();

	//��ȡ��Ϸ֡Ƶ����
	int GetInterval() { return IntervalTime; }
	//������Ϸ֡Ƶ
	void SetFrame(int iTime) { IntervalTime = 1000 / iTime; }
	//��ȡ�Ƿ�Ϊ˯��״̬
	BOOL GetSleep() { return isAsleep; }
	//����˯��״̬
	void SetSleep(BOOL asleep) { isAsleep = asleep; }
	//����ȫ��״̬
	void SetFullScreen(BOOL isFull){ m_bFullScreen = isFull; }
	//��ȡĳ������״̬
	bool CheckKey(WPARAM wParam){ return keys[wParam]; }


	//���Ӽ���Ϊ����:esc
	void SubKeyAction(WPARAM wParam);
	//��Ϸ���ڳ�ʼ��
	BOOL GameWinInit();
	//��Ϣ���������� �ص��������ã�
	LONG GameEvent(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	//��������
	void StartEngine();
	//��Ϣ�ص�����
	static LONG CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

	virtual void GameInit()=0;	//��Ϸ��ʼ��	
	virtual void GameLogic()=0;	//��Ϸ�߼�����
	virtual void GameEnd()=0;  //��Ϸ����
	virtual void GamePaint(HDC hdc)=0;  //����GAME_STATE��ʾ��Ϸ����
	virtual void GameKeyAction(int ActionType=KEY_SYS_NONE)=0; //��������Ϣ
	virtual void GameMouseAction(int x, int y, int ActionType)=0;	 //���������Ϣ
};
