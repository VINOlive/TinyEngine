/* ========================================================== 
*	文 件 名：T_Display.h
*	当前版本：1.0.0
*	创建时间：2014-03-14 
*	修改时间：2014-03-14
*	功能说明：游戏显示类定义
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