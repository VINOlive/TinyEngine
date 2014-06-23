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

class T_Graph
{
protected:
	HBITMAP hBmp;
	int ImageWidth;
	int ImageHeight;

public:
	T_Graph();
	T_Graph(wstring fileName);
	virtual ~T_Graph();

	HBITMAP GetBmpHandle(){ return hBmp; }
	int GetImageWidth(){ return ImageWidth; }
	int GetImageHeight(){ return ImageHeight; }
	
	bool LoadImageFile(wstring path);				
	void PaintImage(HDC hdc, int x, int y);
	void PaintImage(HDC hdc, int x, int y, int width, int height);
	void PaintImage(HDC hdc, int x, int y, int width, int height, BYTE alpha);
	void Destroy();

	static HBITMAP CreateBlankBitmap(int width, int height, COLORREF color);
	static Bitmap* HBITMAP_To_Bitmap(HBITMAP hbmp, HDC hdc);
	static void PaintRegion(HBITMAP in_hbitmap, HDC destDC, int destX, int destY, 
		                    int srcX, int srcY, int regionWidth, int regionHeight,
		                    float ratio, int rotType=0, BYTE alpha=255);  
	static void PaintFrame(HBITMAP in_hbitmap, HDC destDC, int destX, int destY, 
		                   int FrameCount, int RowFrames, int wFrame, int hFrame, 
		                   float ratio=1, int rotType=0, BYTE alpha=255);
	static void PaintBlank(HBITMAP hbmp, int width, int height, COLORREF crColor);
	static void PaintBlank(HDC hdc, int x, int y, 
		                   int width, int height, Color crColor);
	static void PaintBlank(HDC hdc, int x, int y, int width, int height, 
		                   COLORREF crColor, int alpLevel);
	static void PaintText(HDC hdc, RectF fontRect, LPCTSTR text, 
		                  REAL fontSize, LPCTSTR fontName, 
		                  Color fontColor = Color::White,
						  FontStyle style = FontStyleBold, 
						  StringAlignment align = StringAlignmentCenter);
};
