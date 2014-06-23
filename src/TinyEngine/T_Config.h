//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#ifndef TCONFIG_H
#define TCONFIG_H

#include <windows.h>
#include <typeinfo.h>
#include <time.h>
#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <math.h>
using namespace std;
#include <gdiplus.h>
#pragma comment(lib,"Msimg32.lib")
#pragma comment(lib,"gdiplus.lib")
using namespace Gdiplus;

static const int WIN_WIDTH		 = 1024;
static const int WIN_HEIGHT		 = 768;	
static const int FRAME_SPEED	 = 100;	
static const int MENU_WIDTH		 = 200;
static const int MENU_HEIGHT	 = 60;
static const int MENU_SPACE		 = 10;
static const int MENU_ALPHA		 = 180;
static const COLORREF MENU_BKCLR = RGB(0, 0, 0);
static const int LAYER_MAX		 = 1000;
static const LPCTSTR WIN_CLASS	 = TEXT("TinyEngine");
static const LPCTSTR WIN_TITLE	 = TEXT("TinyEngine Ver 1.0 作者:万立中(C) 2007-2013");

enum GAME_DIR
{
	DIR_LEFT,
	DIR_RIGHT,
	DIR_UP,
	DIR_DOWN,
	DIR_LEFT_UP,
	DIR_LEFT_DOWN,
	DIR_RIGHT_UP,
	DIR_RIGHT_DOWN
};

enum TRANSFER
{
	TRANS_NONE,
	TRANS_NOFLIP_ROT90,
	TRANS_NOFLIP_ROT180,
	TRANS_NOFLIP_ROT270,
	TRANS_VFLIP_NOROT,
	TRANS_VFLIP_ROT90,
	TRANS_VFLIP_ROT180,
	TRANS_VFLIP_ROT270,
	TRANS_HFLIP_NOROT,
	TRANS_HFLIP_ROT90,
	TRANS_HFLIP_ROT180,
	TRANS_HFLIP_ROT270
};

enum GAME_STATE
{
	GAME_START,
	GAME_RUN,
	GAME_PAUSE,
	GAME_UPGRADE,
	GAME_WIN,
	GAME_OVER,
	GAME_HELP,
	GAME_ABOUT,
	GAME_SETTING
};
 
enum KM_ACTION
{
	KEY_SYS_NONE,
	KEY_UP,
	KEY_DOWN,
	MOUSE_MOVE,
	MOUSE_LCLICK,
	MOUSE_RCLICK
};

enum LAYER_TYPE
{
	LAYER_PLY,
	LAYER_NPC,
	LAYER_PLY_BOMB,
	LAYER_NPC_BOMB,
	LAYER_MAP_BACK,
	LAYER_MAP_BARR,
	LAYER_MAP_MASK,
	LAYER_EXPLOSION,
	LAYER_TARGET,
	LAYER_BONUS,
	LAYER_NONE
};

typedef struct
{
	int		X;
	int		Y;
	int		Dir;
	int		Speed;
	int		Level;
	int		Score; 
	bool	Visible;
	bool	Active;
	bool	Dead;
	BYTE	Alpha;
	float	Ratio;
	int		Rotation;
}SPRITEINFO;

typedef struct
{
	int		type_id;
	int		map_cols;
	int		map_rows;
	int		tile_width;
	int		tile_height;
	wstring	tile_path;
	int		first_gid;
	vector<vector<int>> data;	
}LAYERINFO;

typedef struct  
{
	POINT pos;
	wstring ItemName;
}MENUITEM;

typedef struct  
{
	int		width;
	int		height;
	int		space;
	int		align;
	LPCTSTR fontName;
	bool	isBold;
	Color	normalTextColor;
	Color	focusTextColor;
}MENU_INFO;

#endif