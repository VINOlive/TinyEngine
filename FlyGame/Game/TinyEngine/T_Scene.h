//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#pragma once
#include "T_Map.h"
#include "T_Layer.h"
#include "T_Sprite.h"

typedef struct
{
	int z_order;
	int type_id;
	T_Layer* layer;	
}GAMELAYER;

typedef vector<GAMELAYER> SCENE_LAYERS;

class T_Scene
{
protected:

	SCENE_LAYERS sceneLayers;
	T_Map* pBarrier;
	T_Map* pMask;

	int SceneWidth, SceneHeight;
	int WinWidth, WinHeight;
	int lastSceneX, lastSceneY;
	int SceneX, SceneY;
	bool LayerChanged;

	static bool SortByZorder(const GAMELAYER &l1, const GAMELAYER &l2);
	
public:
	T_Scene();
	virtual ~T_Scene(void);
	
	int getSceneX(){ return SceneX; }
	int getSceneY(){ return SceneY; }
	int getlastSceneX(){ return lastSceneX; }
	int getlastSceneY(){ return lastSceneY; }
	T_Map* getBarrier(){ return pBarrier; }
	T_Map* getMask(){ return pMask; }
	int GetTotalLayers(){ return sceneLayers.size(); }
	SCENE_LAYERS* getSceneLayers(){ return &sceneLayers; }	
	
	void InitScene(int scn_x, int scn_y, int scn_width, int scn_height, int win_width, int win_height);
	void SetScenePos(int x, int y);
	void MoveScene(int speedX, int speedY);
	void ScrollScene(T_Sprite* player);
		
	void Append(GAMELAYER gm_layer);
	void Insert(GAMELAYER gm_layer, int index);
	void Remove(GAMELAYER gm_layer);
	void RemoveAll();
	void SortLayers();
	POINT getRandomFreeCell();

	void GetTxtMapValue(wstring str, wchar_t* keywords, int& value);
	void GetTxtMapValue(wstring str, wchar_t* keywords, wstring& value);
	void parseCsvData(wstring csv_data, LAYERINFO& m_LayerInfo);
	bool LoadTxtMap(const char* txtmap_path);
	
	void Draw(HDC hdc, int x, int y);
};

