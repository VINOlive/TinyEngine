//*******************************************************************
// TinyEngine����  
// ����: ����
// ����: http://fangjie.sinaapp.com
// ����: 2014-4-17
// ��Ȩ���� 2014-  ����
// (C) 2014- JayFang All Rights Reserved
//*******************************************************************
#pragma once
#include "T_Config.h"
#include "T_Graph.h"

//
// ���˵���֧�֣�
// ��ֱ���У������ְ�ť��ͼƬ��ť
// ˮƽ���У������ְ�ť��ͼƬ��ť
//
class T_Menu
{
private:
	MENU_INFO menu_info;								// �˵���Ϣ
	
	int m_index;										// ��ǰ����˵�������
	bool isItemFocused;									// ��ǰ�˵��Ƿ�񽹵�
	
	int lastIndex;
	int key_index;
	int MaxMenuItemLen;									// ��˵����ֳ���

	int bkImageAlpha;									// ����ͼƬ͸����
	int bkColor;										// ������ɫ

	//AudioDXBuffer* m_MoveSound;						// �˵�����ƹ�ʱ������
	//AudioDXBuffer* m_ClickSound;						// �˵������ʱ������

	T_Graph gm_menuBkg;									// �˵�����
	T_Graph BtnDIB;										// �˵�ͼƬ					
	vector<MENUITEM> gm_menuItems;						// �˵���Ŀ

	int GetMenuIndex(int x, int y);						// ���ݵ�ǰ����������˵���������
	StringAlignment GetAlignment();						
	FontStyle GetFontStyle();

public:
	T_Menu();
	virtual ~T_Menu(void);

	// ���ò˵�����ͼƬ��͸���ȼ�������ɫ
	void SetMenuBkg(LPCTSTR img_path, int alphaLevel=255, COLORREF backColor=RGB(0,0,0));	
	void SetBtnBmp(LPCTSTR img_path,						// ���ò˵���ťͼƬ
		           int btnWidth,							// ͼƬ��ť��
		           int btnHeight);							// ͼƬ��ť��

	void SetMenuInfo(MENU_INFO menuInfo);			
	void AddMenuItem(MENUITEM menuItem);					// ��Ӳ˵�������Ŀ

	//void SetMoveSound(AudioDXBuffer* ms_buffer);
	//void SetClickSound(AudioDXBuffer* mc_buffer);

	// �˵�����
	// bkgX��bkgY�Ǳ���ͼ��Ҫ���Ƶ����꣬Ĭ��Ϊ0, 0;
	// btnTrans��ť͸����, 255��͸��, Ĭ�ϲ�͸��; 
	// startState�Ƿ���ƿ�ʼ�˵���Ĭ��Ϊ��ʼ�˵�
	void DrawMenu(HDC hdc, int bkgX = 0, int bkgY = 0, BYTE btnTrans=255, bool startState=true);

	int MenuMouseClick(int x, int y);					// �˵�������¼�����
	void MenuMouseMove(int x, int y);					// �˵�����ƶ��¼�����
	int MenuKeyDown(WPARAM key);						// �˵������¼�����

	int GetMenuIndex(){ return m_index; }				// ���ص�ǰ��m_index
	void SetMenuIndex(int i){ m_index = i; }			// ���õ�ǰ��m_index

	void DestroyAll();
};
