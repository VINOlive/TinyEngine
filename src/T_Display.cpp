/* ========================================================== 
*	文 件 名：T_Display.cpp
*	当前版本：1.0.0
*	创建时间：2014-03-14 
*	修改时间：2014-03-14
*	功能说明：游戏显示类实现
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
	HDC dc=GetDC(0);
	devmode_saved.dmSize=sizeof(devmode_saved);
	devmode_saved.dmDriverExtra=0;
	devmode_saved.dmPelsHeight=GetDeviceCaps(dc,VERTRES);
	devmode_saved.dmPelsWidth=GetDeviceCaps(dc,HORZRES);
	devmode_saved.dmBitsPerPel=GetDeviceCaps(dc,BITSPIXEL);
	devmode_saved.dmFields=DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL;
	if((GetVersion()&0x8000000)==0)
	{
		devmode_saved.dmFields|=DM_DISPLAYFREQUENCY;
		devmode_saved.dmDisplayFrequency=GetDeviceCaps(dc,VERTRES);
	}
	ReleaseDC(0,dc);
}

void T_Display::ResetMode()
{
	if(mode_changed)
	{
		ChangeDisplaySettings(&devmode_saved,0);
		mode_changed=false;
	}
}

bool T_Display::ChangeMode(int width, int height)
{
	DEVMODE devmode;
	devmode=devmode_saved;
	devmode.dmPelsHeight=height;
	devmode.dmPelsWidth=width;
	devmode.dmFields=DM_PELSWIDTH|DM_PELSHEIGHT;
	if(ChangeDisplaySettings(&devmode,CDS_FULLSCREEN)==DISP_CHANGE_SUCCESSFUL)
	{
		mode_changed=true;
		return true;
	}
	return false;
}