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

class T_Util
{
public:
	static wchar_t * int_to_wstring(int i);
	static void GetBevelSpeed(IN POINT startPT, IN POINT destPT, IN int Speed, OUT int& xRatio, OUT int& yRatio);
	static void GetRandomNum(int range, int* iArray);
};