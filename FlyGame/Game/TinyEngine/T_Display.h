//*******************************************************************
// TinyEngine����  
// ����: ������(WanLizhong)
// ����: www.wanlizhong.com 
// ����: 2013-08-02
// ��Ȩ���� 2007-2013 ������
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#pragma once
#include <Windows.h> 

class T_Display 
{
public:
	T_Display();
	~T_Display();

	void SaveMode();
	void ResetMode();
	bool ChangeMode(int width, int height);

private:
	DEVMODE	devmode_saved;
	bool mode_changed;
} ;