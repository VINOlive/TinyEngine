//*******************************************************************
// TinyEngine����  
// ����: ����
// ����: http://fangjie.sinaapp.com
// ����: 2014-4-17
// ��Ȩ���� 2014-  ����
// (C) 2014- JayFang All Rights Reserved
//*******************************************************************

#pragma once
#include "T_Config.h"

class T_Graph
{
protected:
	HBITMAP hBmp;		// �Ѽ���ͼ����
	int ImageWidth;		// �Ѽ���ͼ���
	int ImageHeight;	// �Ѽ���ͼ���

public:
	T_Graph();					// T_Graph��Ĭ�Ϲ��캯��
	T_Graph(LPTSTR fileName);	// ʹ��ָ���ļ�����T_Graph����
	virtual ~T_Graph();			// T_Graph�����������

	HBITMAP GetBmpHandle(){ return hBmp; }		// ����Ѽ���ͼ����
	int GetImageWidth(){ return ImageWidth; }	// ����Ѽ���ͼ���
	int GetImageHeight(){ return ImageHeight; }	// ����Ѽ���ͼ���
	
public:
	// ����ͼ��(֧��BMP, GIF, JPEG, PNG, TIFF�ȸ�ʽ)
	bool LoadImageFile(LPCTSTR path);				

	void PaintImage(HDC hdc, int x, int y);
	void PaintImage(HDC hdc, int x, int y, int width, int height);
	void PaintImage(HDC hdc, int x, int y, int width, int height, BYTE alpha);
	
	void Destroy();	// �ͷ���Դ
public:
	static HBITMAP CreateBlankBitmap(int width, int height, COLORREF color);

	// ��HBITMAP���ת��ΪBitmap�����ĺ���
	static Bitmap* HBITMAP_To_Bitmap(HBITMAP hbmp, HDC hdc);

	// ����ԭʼͼ��ľֲ����򣬻���ʱ���Խ������Ų�ָ��͸���Ⱥ�ͼ�����ת��ʽ
	// ����in_hbitmapΪҪ������ԭʼλͼ���
	// ����destDCΪҪ���Ƶ�Ŀ���豸
	// ����destX��destYΪ���Ƶ�Ŀ�ĵ�����λ��
	// �ֲ������ɲ���srcX��srcY��regionWidth��regionHeightָ��
	// ����ratioָ�����ű��ʣ���ֵΪ1����ԭ����С��1Ϊ��С������1Ϊ�Ŵ�
	// ����rotTypeָ����ת���ͣ��ò�����ֵ������TRANSFER����ֵ֮һ��
	// ����alphaΪ����ʱ��͸����
	static void PaintRegion(HBITMAP in_hbitmap, HDC destDC, int destX, int destY, 
		                    int srcX, int srcY, int regionWidth, int regionHeight,
		                    float ratio, int rotType=0, BYTE alpha=255);  

	// ����PaintRegion��װ��һ��ר�����ڻ��ƶ���֡ͼ��ĺ���
	// ����in_hbitmapΪҪ������ԭʼλͼ���
	// ����destDCΪҪ���Ƶ�Ŀ���豸
	// ����destX��destYΪ���Ƶ�Ŀ�ĵ�����λ��
	// ����FrameCountΪ����֡����
	// ����RowFramesΪ����֡ͼͼ��ÿ��֡ͼ������
	// ����wFrame��hFrameΪ֡����
	// ����ratioָ�����ű��ʣ���ֵΪ1����ԭ����С��1Ϊ��С������1Ϊ�Ŵ�
	// ����rotTypeָ����ת���ͣ��ò�����ֵ������TRANSFER����ֵ֮һ��
	// ����alphaΪ����ʱ��͸���� 
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
