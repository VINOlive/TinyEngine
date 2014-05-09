/* ========================================================== 
*	�� �� ����T_Display.cpp
*	��ǰ�汾��1.0.0
*	����ʱ�䣺2014-03-14 
*	�޸�ʱ�䣺2014-03-14
*	����˵������Ϸ��ʾ��ʵ��
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