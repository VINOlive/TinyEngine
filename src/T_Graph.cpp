#include "T_Graph.h"

T_Graph::T_Graph()// T_Graph类默认构造函数
{
	hBmp=NULL;
}
T_Graph::T_Graph(LPTSTR fileName)	// 使用指定文件创建T_Graph对象
{
	LoadImageFile(fileName);
}

T_Graph::~T_Graph()		// T_Graph类的析构函数
{
	Destroy();
}

	// 加载图像(支持BMP, GIF, JPEG, PNG, TIFF等格式)
bool T_Graph::LoadImageFile(LPCTSTR path)
{
	Bitmap * pbmp=Bitmap::FromFile(path);
	if (!pbmp)
		return FALSE;
	ImageHeight=pbmp->GetHeight();
	ImageWidth=pbmp->GetWidth();

	if(ImageWidth==0||ImageHeight==1)
		return false;
	Status status=pbmp->GetHBITMAP(NULL,&hBmp);
	if(pbmp)
	{
		delete pbmp;
		pbmp=NULL;
	}
	if(status==S_OK)
		return true;
	else
		return false;
}

void T_Graph::PaintImage(HDC hdc, int x, int y)
{
	HDC memDC=CreateCompatibleDC(hdc);
	HBITMAP OldFrameBmp=(HBITMAP)SelectObject(memDC,hBmp);
	BitBlt(hdc,x,y,ImageWidth,ImageHeight,memDC,0,0,SRCCOPY);
	SelectObject(memDC,OldFrameBmp);
	DeleteDC(memDC);
	DeleteObject(OldFrameBmp);
}
void T_Graph::PaintImage(HDC hdc, int x, int y, int width, int height)
{
	HDC memDC=CreateCompatibleDC(hdc);
	HBITMAP OldFrameBmp=(HBITMAP)SelectObject(memDC,hBmp);
	SetStretchBltMode(hdc,COLORONCOLOR);
	StretchBlt(hdc,x,y,width,height,memDC,0,0,ImageWidth,ImageHeight,SRCCOPY);
	SelectObject(memDC,OldFrameBmp);
	DeleteDC(memDC);
	DeleteObject(OldFrameBmp);
}
void T_Graph::PaintImage(HDC hdc, int x, int y, int width, int height, BYTE alpha)
{
	HDC memDC=CreateCompatibleDC(hdc);
	HBITMAP OldFrameBmp=(HBITMAP)SelectObject(memDC,hBmp);


	BLENDFUNCTION dc_bf;
	dc_bf.BlendOp=AC_SRC_OVER;
	dc_bf.BlendFlags=0;
	dc_bf.SourceConstantAlpha=alpha;
	dc_bf.AlphaFormat=AC_SRC_ALPHA;

	SetStretchBltMode(hdc,COLORONCOLOR);
	AlphaBlend(hdc,x,y,width,height,memDC,0,0,ImageWidth,ImageHeight,dc_bf);

	SelectObject(memDC,OldFrameBmp);
	DeleteDC(memDC);
	DeleteObject(OldFrameBmp);
}

void T_Graph::Destroy()	// 释放资源
{
	if(hBmp)
	{
		DeleteObject(hBmp);
		hBmp=NULL;
	}
}

HBITMAP T_Graph::CreateBlankBitmap(int width, int height, COLORREF color)
{
	BYTE * pbits=NULL;
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth=width;
	bmi.bmiHeader.biHeight=height;
	bmi.bmiHeader.biPlanes=1;
	bmi.bmiHeader.biBitCount=32;
	bmi.bmiHeader.biCompression=BI_RGB;
	bmi.bmiHeader.biSizeImage=width*height*4;
	HBITMAP blankbmp=CreateDIBSection(
		NULL,&bmi,DIB_RGB_COLORS,(void**)&pbits,NULL,NULL);
	PaintBlank(blankbmp,width,height,color);
	return blankbmp;
}

	// 把HBITMAP句柄转换为Bitmap类对象的函数
Bitmap* T_Graph::HBITMAP_To_Bitmap(HBITMAP hbmp, HDC hdc)
{
	BITMAP bmp;
	int width, height;
	// 根据位图句柄获取BITMAP结构体对象的数据
	memset((void*)&bmp, 0, sizeof(BITMAP));
	GetObject(hbmp, sizeof(BITMAP), (void*)&bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;
	byte* bmpBytes = (byte*)bmp.bmBits;
	if(width<=0 && height<=0) return NULL;

	// 根据位图宽高新建一个PixelFormat32bppPARGB格式的Bitmap对象
	Bitmap* BmpCpy = new Bitmap(width, height, PixelFormat32bppPARGB);
	// 采用LockBits函数为位图像素数据拷贝准备内存缓冲区
	BitmapData bmpData;
	Rect rect(0, 0, width, height);
	BmpCpy->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppPARGB, &bmpData);
	// 计算出位图每行像素数，并将指针指向首行
	int lineSize = width * 4;
	byte* cpyBytes = (byte*)(bmpData.Scan0);
	// 按照位图的高逐行拷贝像素数据至缓冲区
	for (int y = 0; y < height; y++)
	{
		memcpy((y * lineSize)+cpyBytes, ((height - y -1) * lineSize)+bmpBytes, lineSize);
	}
	// 解锁内存缓冲区
	BmpCpy->UnlockBits(&bmpData);
	return BmpCpy;
}

	// 绘制原始图像的局部区域，绘制时可以进行缩放并指定透明度和图像的旋转方式
	// 参数in_hbitmap为要操作的原始位图句柄
	// 参数destDC为要绘制的目标设备
	// 参数destX、destY为绘制的目的地坐标位置
	// 局部区域由参数srcX、srcY、regionWidth和regionHeight指定
	// 参数ratio指定缩放比率，该值为1保持原样，小于1为缩小，大于1为放大
	// 参数rotType指定旋转类型（该参数的值必须是TRANSFER常量值之一）
	// 参数alpha为绘制时的透明度
void T_Graph::PaintRegion(HBITMAP in_hbitmap, HDC destDC, int destX, int destY, 
		int srcX, int srcY, int regionWidth, int regionHeight,
		float ratio, int rotType, BYTE alpha)
{
	int width = (int)(regionWidth*ratio);
	int height = (int)(regionHeight*ratio);

	BYTE* pBits = NULL;
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32; 
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width * height * 4;

	HDC frameDC = ::CreateCompatibleDC(NULL);
	HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void **)&pBits, NULL, NULL);
	HBITMAP OldFrameBmp = (HBITMAP)SelectObject(frameDC, hbitmap);

	BLENDFUNCTION frame_bf;
	frame_bf.BlendOp = AC_SRC_OVER;
	frame_bf.BlendFlags = 0;
	frame_bf.SourceConstantAlpha = 255;
	frame_bf.AlphaFormat = AC_SRC_ALPHA;

	HDC memDC = CreateCompatibleDC(destDC);
	HBITMAP OldMemBmp = (HBITMAP)SelectObject(memDC,in_hbitmap);

	AlphaBlend(frameDC, 0, 0, (int)(regionWidth*ratio), (int)(regionHeight*ratio), 
		memDC, srcX, srcY, regionWidth, regionHeight, frame_bf);

	Bitmap* nowFrameBmp =HBITMAP_To_Bitmap(hbitmap, destDC);

	//还原：使用原来对象替换内存设备中的位图对象
	SelectObject(frameDC, OldFrameBmp);
	DeleteDC(frameDC);//删除内存设备
	DeleteObject(OldFrameBmp);//删除位图对象
	DeleteObject(hbitmap);//删除位图对象

	//还原：使用原来对象替换内存设备中的位图对象
	SelectObject(memDC, OldMemBmp);
	DeleteDC(memDC);//删除内存设备
	DeleteObject(OldMemBmp);//删除位图对象

	//旋转帧图
	int framew = (int)(regionWidth*ratio);
	int frameh =  (int)(regionHeight*ratio);
	switch(rotType)
	{
	case TRANS_NONE:
		break;
	case TRANS_NOFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipNone);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_NOFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate180FlipNone);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_NOFLIP_ROT270:
		nowFrameBmp->RotateFlip(Rotate270FlipNone);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_VFLIP_NOROT:
		nowFrameBmp->RotateFlip(RotateNoneFlipY);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_VFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipY);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_VFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate180FlipY);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_VFLIP_ROT270:
		nowFrameBmp->RotateFlip(Rotate270FlipY);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_HFLIP_NOROT:
		nowFrameBmp->RotateFlip(RotateNoneFlipX);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_HFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipX);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	case TRANS_HFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate180FlipX);
		framew = (int)(regionWidth*ratio);
		frameh = (int)(regionHeight*ratio);
		break;
	case TRANS_HFLIP_ROT270:
		nowFrameBmp->RotateFlip(Rotate270FlipX);
		framew = (int)(regionHeight*ratio);
		frameh = (int)(regionWidth*ratio);
		break;
	}

	HDC bufDC = CreateCompatibleDC(destDC);
	HBITMAP hbmp = NULL;
	nowFrameBmp->GetHBITMAP( Color( 0, 0, 0, 0 ), &hbmp );
	HBITMAP OldbufBmp = (HBITMAP)SelectObject( bufDC, hbmp);

	BLENDFUNCTION buf_bf;
	buf_bf.BlendOp = AC_SRC_OVER;
	buf_bf.BlendFlags = 0;
	buf_bf.SourceConstantAlpha = alpha;
	buf_bf.AlphaFormat = AC_SRC_ALPHA;
	AlphaBlend(destDC, destX, destY, framew, frameh, 
		bufDC, 0, 0, framew, frameh, buf_bf);

	//还原：使用原来对象替换内存设备中的位图对象
	SelectObject(bufDC, OldbufBmp);
	DeleteDC(bufDC);//删除内存设备
	DeleteObject(OldbufBmp);//删除位图对象
	DeleteObject(hbmp);//删除位图对象
	DeleteObject(nowFrameBmp);//删除位图对象
	delete nowFrameBmp;//删除位图对象
}

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
void T_Graph::PaintFrame(HBITMAP in_hbitmap, HDC destDC, int destX, int destY, 
		int FrameCount, int RowFrames, int wFrame, int hFrame, 
		float ratio, int rotType, BYTE alpha)
{
	int col=FrameCount;
	int row=(FrameCount-col)/RowFrames;
	PaintRegion(in_hbitmap,destDC,destX,destY,col*wFrame,row*hFrame,wFrame,hFrame,ratio,rotType,alpha);
}


void T_Graph::PaintBlank(HBITMAP hbmp, int width, int height, COLORREF crColor)
{
	HDC memdc=::CreateCompatibleDC(NULL);
	HBITMAP OldBmp =(HBITMAP)SelectObject(memdc,hbmp);
	HBRUSH hbrush=CreateSolidBrush(crColor);
	RECT rcBitmap={0,0,width,height};
	FillRect(memdc,&rcBitmap,hbrush);

	SelectObject(memdc,OldBmp);
	DeleteDC(memdc);
	DeleteObject(OldBmp);
	OldBmp=NULL;
	DeleteObject(hbrush);

}
void T_Graph::PaintBlank(HDC hdc, int x, int y, 
		int width, int height, Color crColor)
{

	Gdiplus::Graphics g(hdc);
	Rect rect(x,y,width,height);
	SolidBrush brush(crColor);
	g.FillRectangle(&brush,rect);
	g.ReleaseHDC(hdc);
}
void T_Graph::PaintBlank(HDC hdc, int x, int y, int width, int height, 
		COLORREF crColor, int alpLevel)
{
	HBITMAP hbmp=CreateCompatibleBitmap(hdc,width,height);

	HDC memdc=::CreateCompatibleDC(NULL);
	HBITMAP OldBmp =(HBITMAP)SelectObject(memdc,hbmp);
	HBRUSH hbrush=CreateSolidBrush(crColor);
	RECT rcBitmap={x,y,width,height};
	FillRect(memdc,&rcBitmap,hbrush);

	BLENDFUNCTION frame_bf;
	frame_bf.BlendFlags=0;
	frame_bf.BlendOp=AC_SRC_OVER;
	frame_bf.SourceConstantAlpha=alpLevel;
	frame_bf.AlphaFormat=0;
	AlphaBlend(hdc,x,y,width,height,memdc,0,0,width,height,frame_bf);

	SelectObject(memdc,OldBmp);
	DeleteDC(memdc);
	DeleteObject(OldBmp);
	OldBmp=NULL;
	DeleteObject(hbmp);
	hbmp=NULL;
	DeleteObject(hbrush);
}

void T_Graph::PaintText(HDC hdc ,RectF fontRect,LPCTSTR text,
						REAL fontSize,LPCTSTR fontName,Color fontColor,
						FontStyle style,StringAlignment align )
{
	Graphics graph(hdc);
	FontFamily fontFamily(fontName);
	Font font(&fontFamily,fontSize,style,UnitPoint);
	StringFormat format;
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetAlignment(align);
	SolidBrush solidBrush1(fontColor);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graph.DrawString(text,(INT)wcslen(text),&font,fontRect,&format,&solidBrush1);
	graph.ReleaseHDC(hdc);
}