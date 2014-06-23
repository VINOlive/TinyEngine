//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
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