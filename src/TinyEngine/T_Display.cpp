//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "T_Display.h"
 
T_Display::T_Display():mode_changed(false)
{
	SaveMode();
}

T_Display::~T_Display()
{
	ResetMode();
}

void T_Display::SaveMode()
{
	HDC	dc = GetDC(0);
	devmode_saved.dmSize = sizeof(devmode_saved);
	devmode_saved.dmDriverExtra = 0;
	devmode_saved.dmPelsWidth = GetDeviceCaps(dc, HORZRES);
	devmode_saved.dmPelsHeight = GetDeviceCaps(dc, VERTRES);
	devmode_saved.dmBitsPerPel = GetDeviceCaps(dc, BITSPIXEL);
	devmode_saved.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;
	if ((GetVersion() & 0x80000000) == 0)
	{		
		devmode_saved.dmFields |= DM_DISPLAYFREQUENCY;
		devmode_saved.dmDisplayFrequency = GetDeviceCaps(dc, VREFRESH);
	}
	ReleaseDC(0, dc);
}

void T_Display::ResetMode()
{
	if (mode_changed) 
	{
		ChangeDisplaySettings(&devmode_saved, 0);
		mode_changed = false;
	}
}

bool T_Display::ChangeMode(int width, int height)
{
	DEVMODE	devmode;
	devmode = devmode_saved;
	devmode.dmPelsWidth = width;
	devmode.dmPelsHeight = height;
	devmode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&devmode, CDS_FULLSCREEN) 
		== DISP_CHANGE_SUCCESSFUL) 
	{
		mode_changed = true;
		return true;
	}
	return false;
}