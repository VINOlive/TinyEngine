#include"T_Menu.h"
#include"T_Graph.h"

T_Menu::T_Menu()
{
	m_index=-1;
	isItemFocused=false;
	bkImageAlpha=255;
	bkColor=RGB(0,0,0);
}

T_Menu::~T_Menu(void)
{
	DestroyAll();
}

void T_Menu::SetMenuBkg(LPCTSTR img_path, int alphaLevel/* =255 */, COLORREF backColor/* =RGB */)
{
	if(wcslen(img_path)>0)
	{
		gm_menuBkg.LoadImageFile(img_path);
		bkColor=backColor;
		bkImageAlpha=alphaLevel;
	}
}

void T_Menu::SetBtnBmp(LPCTSTR img_path, int btnWidth, int btnHeight)
{
	if(wcslen(img_path)>0)
	{
		BtnDIB.LoadImageFile(img_path);
		if(&BtnDIB!=NULL)
		{
			menu_info.width=btnHeight;
			menu_info.height=btnHeight;
		}
	}
}

void T_Menu::AddMenuItem(MENUITEM menuItem)
{
	gm_menuItems.push_back(menuItem);
	int len=menuItem.ItemName.length();
	if(MaxMenuItemLen<len)
	{
		MaxMenuItemLen=len;
	}
}

void T_Menu::SetMenuInfo(MENU_INFO menuinfo)
{
	menu_info=menuinfo;
}

void T_Menu::DrawMenu(HDC hdc, int bkgX /* = 0 */, int bkgY /* = 0 */, BYTE btnTrans/* =255 */, bool startState/* =true */)
{
	if(&gm_menuBkg != NULL && startState == true) 
	{
		HBITMAP tBmp = T_Graph::CreateBlankBitmap(WIN_WIDTH, WIN_HEIGHT, bkColor);
		SelectObject(hdc, tBmp);

		gm_menuBkg.PaintImage(hdc, bkgX, bkgY, WIN_WIDTH, WIN_HEIGHT, bkImageAlpha);

		DeleteObject(tBmp);
		tBmp = NULL;
	}
	else
	{
		T_Graph::PaintBlank(hdc, 0, 0, WIN_WIDTH, WIN_HEIGHT, MENU_BKCLR, MENU_ALPHA);

		if(&gm_menuBkg != NULL)
		{
			gm_menuBkg.PaintImage(
				hdc, bkgX, bkgY, gm_menuBkg.GetImageWidth(), gm_menuBkg.GetImageHeight(), bkImageAlpha
				);
		}
	}

	int w = menu_info.width;
	int h = menu_info.height;

	int FontHeight;
	int px = 0;
	int w_px = w/(MaxMenuItemLen+1);	//计算每个字所占的像素
	int h_px = (int)((float)(h/2.5));
	if(w_px > h_px) 
	{
		px = h_px;
	}
	else
	{
		px = w_px;
	}
	FontHeight = (px*72)/96;		//根据每个字的像素计算字号

	if(isItemFocused == FALSE)
	{
		Gdiplus::RectF Rec;
		vector<MENUITEM>::iterator iter;
		for(iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++)
		{
			int x = iter->pos.x;
			int y = iter->pos.y;

			// 绘制按钮图像
			if(&BtnDIB != NULL)
			{
				BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(), hdc, x, y, 0, 0, w, h, 1, 0, btnTrans);
			}
			Rec.X = (float)x;		
			Rec.Y = (float)y;
			Rec.Width = (float)w;	
			Rec.Height = (float)h;

			// 绘制按钮文字
			LPCTSTR item = iter->ItemName.c_str();
			T_Graph::PaintText(hdc, Rec, item, (REAL)FontHeight, menu_info.fontName, 
				menu_info.normalTextColor, GetFontStyle(), GetAlignment());
		}
	}

	if(isItemFocused == TRUE)
	{
		int mIndex = 0;
		Gdiplus::RectF Rec;
		vector<MENUITEM>::iterator iter;
		for(iter = gm_menuItems.begin(); iter != gm_menuItems.end(); iter++)
		{
			int x = iter->pos.x;
			int y = iter->pos.y;

			Rec.X = (float)x;		
			Rec.Y = (float)y;
			Rec.Width = (float)w;	
			Rec.Height = (float)h;

			if(mIndex != m_index)
			{
				if(&BtnDIB != NULL)
				{
					BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(), hdc, x, y, 0, 0, w, h, 1, 0, btnTrans);
				}

				LPCTSTR item = iter->ItemName.c_str();
				T_Graph::PaintText(hdc, Rec, item, (REAL)FontHeight, menu_info.fontName, 
					menu_info.normalTextColor, GetFontStyle(), GetAlignment());
			}

			if(mIndex == m_index)
			{
				if(&BtnDIB != NULL)
				{
					BtnDIB.PaintRegion(BtnDIB.GetBmpHandle(), hdc, x, y, 0, h, w, h, 1, 0, btnTrans);
				}

				LPCTSTR item = iter->ItemName.c_str();
				T_Graph::PaintText(hdc, Rec, item, (REAL)FontHeight, menu_info.fontName, 
					menu_info.focusTextColor, GetFontStyle(), GetAlignment());
			}

			mIndex = mIndex + 1;
		}
	}
}

int T_Menu::GetMenuIndex(int x,int y)
{
	int Index=-1;
	POINT pt;
	pt.x=x;
	pt.y=y;
	int w=menu_info.width;
	int h=menu_info.height;
	RECT rec;
	int iCount=0;
	vector<MENUITEM>::iterator iter;
	for(iter=gm_menuItems.begin();iter!=gm_menuItems.end();iter++)
	{
		rec.left=iter->pos.x;
		rec.top=iter->pos.y;
		rec.right=rec.left+w;
		rec.bottom=rec.top+h;
		if(PtInRect(&rec,pt))
		{
			return iCount;
		}
		iCount++;
	}
	return Index;
}

StringAlignment T_Menu::GetAlignment()
{
	StringAlignment sAlign;
	if(menu_info.align==0)
	{
		sAlign=StringAlignmentNear;
	}
	if(menu_info.align==1)
	{
		sAlign=StringAlignmentCenter;
	}
	if(menu_info.align==2)
	{
		sAlign=StringAlignmentFar;
	}
	return sAlign;

}

FontStyle T_Menu::GetFontStyle()
{
	FontStyle fstyle;
	if(menu_info.isBold==true)
	{
		fstyle=FontStyleBold;
	}
	else
	{
		fstyle=FontStyleRegular;
	}
	return fstyle;
}

int T_Menu::MenuMouseClick(int X,int Y)
{
	m_index=GetMenuIndex(X,Y);
	return m_index;
}

void T_Menu::MenuMouseMove(int x,int y)
{
	lastIndex=m_index;
	m_index=GetMenuIndex(x,y);
	if(m_index>=0)
	{
		isItemFocused=true;
	}
	else
	{
		isItemFocused=false;
	}
}

int T_Menu::MenuKeyDown(WPARAM key)
{
	if(key==VK_UP||key==VK_LEFT)
	{
		m_index=m_index-1;
		if(m_index<0)
		{
			m_index=(int)(gm_menuItems.size()-1);
		}
		isItemFocused=true;
	}
	if(key==VK_DOWN||key==VK_RIGHT)
	{
		m_index=m_index+1;
		if(m_index<0)
		{
			m_index=(int)(gm_menuItems.size()+1);
		}
		isItemFocused=true;
	}
	return m_index;
}
void T_Menu::DestroyAll()
{
	gm_menuItems.clear();
	gm_menuItems.swap(vector<MENUITEM>());
}