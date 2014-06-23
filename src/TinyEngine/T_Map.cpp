//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "T_Map.h"

T_Map::T_Map(LAYERINFO layerInfo)
{
	graph = NULL;
	graph = new T_Graph(layerInfo.tile_path.c_str());
	if(graph != NULL)
	{
		layer_data = layerInfo.data;
		SetWidth(layerInfo.map_cols*layerInfo.tile_width);
		SetHeight(layerInfo.map_rows*layerInfo.tile_height);
		first_gid = layerInfo.first_gid;
		tile_width = layerInfo.tile_width;
		tile_height = layerInfo.tile_height;
		Visible = true;
		map_cols = layerInfo.map_cols;
		map_rows = layerInfo.map_rows;
		hbmp_layer = T_Graph::CreateBlankBitmap((int)GetWidth(), (int)GetHeight(), Color::White);
		dc_buf = CreateCompatibleDC(GetDC(0));
		hbmp_old = (HBITMAP)SelectObject(dc_buf, hbmp_layer);
		updated = true;
	}
}

T_Map::T_Map(LPCTSTR imgFilepath)
{
	bool loadOK =false;
	graph = NULL;

	int len = wcslen(imgFilepath);
	if(len<=0)
	{
		throw L"文件路径不能为空！";	
	}
	if(len>0)
	{
		graph = new T_Graph(imgFilepath);
		if(graph != NULL)
		{
			SetWidth(graph->GetImageWidth());
			SetHeight(graph->GetImageHeight());

			tile_width = 0;
			tile_height = 0;
			map_cols = 0;
			map_rows = 0;
			X = 0;
			Y = 0;
			Visible = true;
		}
	}
}

T_Map::~T_Map(void)
{
	vector<vector<int>>::iterator p = layer_data.begin();
	for(; p != layer_data.end();)
	{
		p->clear();
		p = layer_data.erase(p);
	}
	layer_data.clear();
	layer_data.swap(vector<vector<int>>()) ;
	if(graph)
	{
		delete graph;
		graph = NULL;
	}
	SelectObject(dc_buf, hbmp_old);
	DeleteObject(hbmp_layer);
	DeleteObject(hbmp_old);
	DeleteDC(dc_buf);
	hbmp_layer = NULL;
	hbmp_old = NULL;	
}

void T_Map::setTile(int col, int row, int tileIndex)
{
	if ((col < 0) || (col >= map_cols) || (row < 0) || (row >= map_rows)) 
	{
		throw L"矩阵索引值无效！";
	}

	if (tileIndex > 0)
	{
		if (tileIndex >= (map_cols*map_rows))
			throw L"图块索引值无效！";
	}

	layer_data[row][col] = tileIndex;

	updated = true;
}

int T_Map::getTile(int col, int row)
{
	if ((col < 0) || (col >= map_cols) || (row < 0) || (row >= map_rows)) 
	{
		throw L"矩阵索引值无效！";
	}

	return layer_data[row][col];
}

void T_Map::Redraw(HDC hdc)
{
	if(graph->GetImageHeight()>0 && graph->GetImageWidth()>0)
	{
		int width = (int)GetWidth();
		int height = (int)GetHeight();
		SelectObject(dc_buf, hbmp_old);
		DeleteObject(hbmp_layer);
		hbmp_layer = NULL;
		hbmp_layer = T_Graph::CreateBlankBitmap(width, height, Color::White);
		hbmp_old = (HBITMAP)SelectObject(dc_buf, hbmp_layer);
		if (Visible == true) 
		{
			int tileIndex = 0;
			int tX = 0, tY = 0;
			int r = 0, c = 0;
			int img_col = 0, img_row = 0;
			int tileImageWidth = graph->GetImageWidth();
			HDC memDC = CreateCompatibleDC(hdc);
			HBITMAP OldMemBmp = (HBITMAP)SelectObject(memDC, graph->GetBmpHandle());
			for (r = 0, tY = 0; r < map_rows; r++, tY+=tile_height)
			{
				for (c = 0, tX = 0; c < map_cols; c++, tX += tile_width)
				{
					int imgTotalCols = tileImageWidth/tile_width;
					tileIndex = layer_data[r][c];
					if(first_gid == 1)
					{
						img_col = (tileIndex - 1) % imgTotalCols;
						img_row = (tileIndex - 1) / imgTotalCols;
					}
					else
					{
						img_col = tileIndex % imgTotalCols;
						img_row = tileIndex / imgTotalCols;
					}

					BLENDFUNCTION frame_bf;
					frame_bf.BlendOp = AC_SRC_OVER;
					frame_bf.BlendFlags = 0;
					frame_bf.SourceConstantAlpha = 255;
					frame_bf.AlphaFormat = AC_SRC_ALPHA;
					AlphaBlend(dc_buf, tX, tY, tile_width, tile_height, 
						memDC, img_col*tile_width, img_row*tile_height, 
						tile_width, tile_height, frame_bf
					);
				}
			}
			SelectObject(memDC, OldMemBmp);
			DeleteDC(memDC);
			DeleteObject(OldMemBmp);
			updated = false;
		}
	}
}

void T_Map::Draw(HDC hdc)
{
	if(tile_width == 0 && tile_height == 0)
	{
		if(graph->GetImageHeight()>0 && graph->GetImageWidth()>0)
		{
			graph->PaintImage(hdc, (int)X, (int)Y);
		}
	}
	if(tile_width > 0 && tile_height > 0)
	{
		if(updated == true) 
		{
			Redraw(hdc);
		}
		if(updated == false)
		{
			BLENDFUNCTION frame_bf;
			frame_bf.BlendOp = AC_SRC_OVER;
			frame_bf.BlendFlags = 0;
			frame_bf.SourceConstantAlpha = 255;
			frame_bf.AlphaFormat = AC_SRC_ALPHA;
			AlphaBlend(hdc, (int)X, (int)Y, (int)GetWidth(), (int)GetHeight(), 
				dc_buf, 0, 0,(int)GetWidth(), (int)GetHeight(), frame_bf
			);
		}
	}
}