//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#pragma once
#include "T_Graph.h"
#include "T_Layer.h"

class T_Map:public T_Layer
{
protected:
	int tile_width;
	int tile_height;
	int map_cols;
	int map_rows;
	int	first_gid;	
	bool updated;
	
	T_Graph* graph;
	HDC dc_buf;
	HBITMAP hbmp_old;
	HBITMAP hbmp_layer;
	vector<vector<int>> layer_data;
	
public:
	T_Map(LAYERINFO layerInfo);
	T_Map(LPCTSTR imgFilepath);
	virtual ~T_Map(void);

	int getTileWidth(){ return tile_width; }
	int getTileHeight(){ return tile_height; }
	int getMapCols(){ return map_cols; }
	int getMapRows(){ return map_rows; }
	void setUpdate(bool updt){ updated = updt; }
	bool getUpdate(){ return updated; }
	virtual string ClassName(){ return "T_Map"; }

	void setTile(int col, int row, int tileIndex);
	int getTile(int col, int row);
	void Redraw(HDC hdc);
	void Draw(HDC hdc);
};