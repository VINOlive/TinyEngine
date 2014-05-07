/* ========================================================== 
*	文 件 名：T_Config.cpp
*	当前版本：1.0.0
*	创建时间：2014-03-14 
*	修改时间：2014-03-14
*	功能说明：游戏引擎类实现
*	编译环境：VS2008
*
*	班    级：东软实验111班
*	姓    名：方杰
*	学    号：8000611063
*	实验名称：类与对象
*	实验序号：
*	实验地点：图书馆406
*	实验时间：2014-03-14 16:10-18:00
 ==========================================================*/
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
static const LPCTSTR WIN_CLASS	 = TEXT("JayEngine");
static const LPCTSTR WIN_TITLE	 = TEXT("JayEngine Ver 1.0 作者:方杰(C) 20014-");

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