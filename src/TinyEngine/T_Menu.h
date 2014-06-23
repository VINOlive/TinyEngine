//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#pragma once
#include "T_Config.h"
#include "T_Graph.h"
#include "T_Audio.h"

class T_Menu
{
private:
	MENU_INFO menu_info;
	int m_index;
	bool isItemFocused;
	int lastIndex;
	int key_index;
	int MaxMenuItemLen;
	int bkImageAlpha;
	int bkColor;

	AudioDXBuffer* m_MoveSound;
	AudioDXBuffer* m_ClickSound;
	T_Graph gm_menuBkg;
	T_Graph BtnDIB;					
	vector<MENUITEM> gm_menuItems;

	int GetMenuIndex(int x, int y);
	StringAlignment GetAlignment();						
	FontStyle GetFontStyle();

public:
	T_Menu();
	virtual ~T_Menu(void);

	void SetMenuBkg(LPCTSTR img_path, int alphaLevel=255, COLORREF backColor=RGB(0,0,0));	
	void SetBtnBmp(LPCTSTR img_path, int btnWidth, int btnHeight);							
	void SetMenuInfo(MENU_INFO menuInfo);			
	void AddMenuItem(MENUITEM menuItem);
	void SetMoveSound(AudioDXBuffer* ms_buffer);
	void SetClickSound(AudioDXBuffer* mc_buffer);
	void DrawMenu(HDC hdc, int bkgX = 0, int bkgY = 0, BYTE btnTrans=255, bool startState=true);
	int MenuMouseClick(int x, int y);
	void MenuMouseMove(int x, int y);
	int MenuKeyDown(WPARAM key);

	int GetMenuIndex(){ return m_index; }
	void SetMenuIndex(int i){ m_index = i; }
	void DestroyAll();
};
