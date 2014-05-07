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

class T_Graph
{
protected:
	HBITMAP hBmp;		// 已加载图像句柄
	int ImageWidth;		// 已加载图像宽
	int ImageHeight;	// 已加载图像高

public:
	T_Graph();					// T_Graph类默认构造函数
	T_Graph(LPTSTR fileName);	// 使用指定文件创建T_Graph对象
	virtual ~T_Graph();			// T_Graph类的析构函数

	HBITMAP GetBmpHandle(){ return hBmp; }		// 获得已加载图像句柄
	int GetImageWidth(){ return ImageWidth; }	// 获得已加载图像宽
	int GetImageHeight(){ return ImageHeight; }	// 获得已加载图像高
	
public:
	// 加载图像(支持BMP, GIF, JPEG, PNG, TIFF等格式)
	bool LoadImageFile(LPCTSTR path);				

	void PaintImage(HDC hdc, int x, int y);
	void PaintImage(HDC hdc, int x, int y, int width, int height);
	void PaintImage(HDC hdc, int x, int y, int width, int height, BYTE alpha);
	
	void Destroy();	// 释放资源
public:
	static HBITMAP CreateBlankBitmap(int width, int height, COLORREF color);

	// 把HBITMAP句柄转换为Bitmap类对象的函数
	static Bitmap* HBITMAP_To_Bitmap(HBITMAP hbmp, HDC hdc);

	// 绘制原始图像的局部区域，绘制时可以进行缩放并指定透明度和图像的旋转方式
	// 参数in_hbitmap为要操作的原始位图句柄
	// 参数destDC为要绘制的目标设备
	// 参数destX、destY为绘制的目的地坐标位置
	// 局部区域由参数srcX、srcY、regionWidth和regionHeight指定
	// 参数ratio指定缩放比率，该值为1保持原样，小于1为缩小，大于1为放大
	// 参数rotType指定旋转类型（该参数的值必须是TRANSFER常量值之一）
	// 参数alpha为绘制时的透明度
	static void PaintRegion(HBITMAP in_hbitmap, HDC destDC, int destX, int destY, 
		                    int srcX, int srcY, int regionWidth, int regionHeight,
		                    float ratio, int rotType=0, BYTE alpha=255);  

	// 根据PaintRegion封装的一个专门用于绘制动画帧图像的函数
	// 参数in_hbitmap为要操作的原始位图句柄
	// 参数destDC为要绘制的目标设备
	// 参数destX、destY为绘制的目的地坐标位置
	// 参数FrameCount为动画帧总数
	// 参数RowFrames为保存帧图图像每行帧图的数量
	// 参数wFrame、hFrame为帧宽、高
	// 参数ratio指定缩放比率，该值为1保持原样，小于1为缩小，大于1为放大
	// 参数rotType指定旋转类型（该参数的值必须是TRANSFER常量值之一）
	// 参数alpha为绘制时的透明度 
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
	static void PaintText(HDC hdc, LPTSTR text, RECT fontRect, 
		                  int fontSize, LPCTSTR fontName, 
		                  COLORREF fontColor = RGB(255, 255, 255), 
						  bool bold = true, int align = DT_CENTER);

};
