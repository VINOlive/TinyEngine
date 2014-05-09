//*******************************************************************
// TinyEngine引擎  
// 作者: 方杰
// 博客: http://fangjie.sinaapp.com
// 日期: 2014-4-17
// 版权所有 2014-  方杰
// (C) 2014- JayFang All Rights Reserved
//*******************************************************************

#pragma once
#include "T_Config.h"

class T_Layer
{
public:
	T_Layer(){};
	virtual ~T_Layer(void){};

protected:
	int Width, Height;
	int X, Y;
	bool Visible;
	int LayerTypeID;
	int zorder;

public:
	void SetWidth(int width){ Width = width; }
	int GetWidth(){ return Width; }

	void SetHeight(int height){ Height = height; }
	int GetHeight(){ return Height; }
	
	void SetPosition(int x, int y){ X = x; Y = y; }
	int GetX(){ return X; }
	int GetY(){ return Y; }

	void SetVisible(bool visible){ Visible = visible; }
	bool IsVisible(){ return Visible; }
	
	void SetLayerTypeID(int i){ LayerTypeID = i; }
	int GetLayerTypeID(){ return LayerTypeID; }

	int getZorder(){ return zorder; }
	void setZorder(int z){ zorder = z; }

	//dx_speed: 正值向右移动， 负值向左移动
	//dy_speed: 正值向下移动， 负值向上移动
	void Move(int dx_speed, int dy_speed){ X += dx_speed; Y += dy_speed;}	
	virtual string ClassName(){ return "T_Layer"; }


	virtual void Draw(HDC hdc)=0;
};