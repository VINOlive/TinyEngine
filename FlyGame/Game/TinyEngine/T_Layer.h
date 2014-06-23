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
	
	void Move(int dx_speed, int dy_speed){ X += dx_speed; Y += dy_speed;}

	virtual string ClassName(){ return "T_Layer"; }
	virtual void Draw(HDC hdc)=0;
};