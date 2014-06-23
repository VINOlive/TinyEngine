//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "T_Scene.h"

T_Scene::T_Scene()
{
	lastSceneX = 0;
	lastSceneY = 0;
	SceneY = 0;
	SceneX = 0;
	SceneWidth = 0;
	SceneHeight = 0;
	WinWidth = 0;
	WinHeight = 0;
	pBarrier = NULL;
	pMask= NULL;
	LayerChanged = false;
}

T_Scene::~T_Scene(void)
{
	RemoveAll();
}

void T_Scene::InitScene(int scn_x, int scn_y, 
						int scn_width, int scn_height, 
						int win_width, int win_height)
{
	SceneX = scn_x;
	SceneY = scn_y;
	lastSceneX = scn_x;
	lastSceneY = scn_y;
	SceneWidth = scn_width;
	SceneHeight = scn_height;
	WinWidth = win_width;
	WinHeight = win_height;
}

void T_Scene::SetScenePos(int x, int y)
{
	lastSceneX = SceneX;
	lastSceneY = SceneY;
	SceneX = x;
	SceneY = y;
}

void T_Scene::MoveScene(int speedX, int speedY)
{	
	lastSceneX = SceneX;
	lastSceneY = SceneY;
	int nextStepX, nextStepY; 
	nextStepX = SceneX + speedX;
	nextStepY = SceneY + speedY;
	if(nextStepX >= 0) 
		speedX = 0 - SceneX;
	if(nextStepX <= (WinWidth-SceneWidth)) 
		speedX = (WinWidth-SceneWidth) - SceneX;

	if(nextStepY >= 0) 
		speedY = 0 - SceneY;
	if(nextStepY <= (WinHeight-SceneHeight)) 
		speedY = (WinHeight-SceneHeight) - SceneY;
	SceneX += speedX;
	SceneY += speedY;
}

void T_Scene::ScrollScene(T_Sprite* player)
{
	bool scn_scrolling = false;
	int speedX=0, speedY=0;
	switch(player->GetDir())
	{
	case DIR_LEFT:
		speedX = player->GetSpeed();
		speedY = 0;
		if (player->GetX() > (WinWidth/5) ) 
		{
			scn_scrolling = false;
		}
		else
		{
			scn_scrolling = true;
		}
		break;
	case DIR_RIGHT:
		speedX = -player->GetSpeed();
		speedY = 0;
		if (player->GetX() < (WinWidth*4/5)-player->GetRatioSize().cx) 
		{
			scn_scrolling = false;
		}
		else
		{
			scn_scrolling = true;
		}
		break;
	case DIR_UP:
		speedX = 0;
		speedY = player->GetSpeed();
		if (player->GetY() > (WinHeight/5)) 
		{
			scn_scrolling = false;
		}
		else
		{
			scn_scrolling = true;
		}
		break;
	case DIR_DOWN:
		speedX = 0;
		speedY =  -player->GetSpeed();
		if (player->GetY() < (WinHeight*4/5)-player->GetRatioSize().cy) 
		{
			scn_scrolling = false;
		}
		else
		{
			scn_scrolling = true;
		}
		break;
	case DIR_LEFT_UP:
		if (player->GetX() > (WinWidth/5)  &&  player->GetY() > (WinHeight/5)) 
		{
			scn_scrolling = false;
		}
		if (player->GetX() <= (WinWidth/5) &&  player->GetY() <= (WinHeight/5)) 
		{
			POINT spPT = {player->GetX(), player->GetY()};
			POINT scenePT = {SceneX, SceneY};
			T_Util::GetBevelSpeed(spPT, scenePT, player->GetSpeed(), speedX, speedY);
			speedX = -speedX;
			speedY = -speedY;
			scn_scrolling = true;
		}
		break;
	case DIR_LEFT_DOWN:
		if ((player->GetX() > (WinWidth/5)) &&  
			(player->GetY() < ((WinHeight*4/5)-player->GetRatioSize().cy)))
		{
			scn_scrolling = false;
		}
		if ((player->GetX() <= (WinWidth/5)) &&  
			(player->GetY() >= ((WinHeight*4/5)-player->GetRatioSize().cy)))
		{
			POINT spPT = {player->GetX(), player->GetY()+player->GetRatioSize().cy};
			POINT scenePT = {SceneX, SceneY+SceneHeight};
			T_Util::GetBevelSpeed(spPT, scenePT, player->GetSpeed(), speedX, speedY);
			speedX = -speedX;
			speedY = -speedY;
			scn_scrolling = true;
		}
		break;
	case DIR_RIGHT_UP:
		if ((player->GetX() < (WinWidth*4/5)-player->GetRatioSize().cx) &&  
			(player->GetY() > (WinHeight/5)))
		{
			scn_scrolling = false;
		}
		if ((player->GetX() >= (WinWidth*4/5)-player->GetRatioSize().cx) &&  
			(player->GetY() <= (WinHeight/5)))
		{
			POINT spPT = {player->GetX()+player->GetRatioSize().cx, player->GetY()};
			POINT scenePT = {SceneX+SceneWidth, SceneY};
			T_Util::GetBevelSpeed(spPT, scenePT, player->GetSpeed(), speedX, speedY);
			speedX = -speedX;
			speedY = -speedY;
			scn_scrolling = true;
		}
		break;
	case DIR_RIGHT_DOWN:
		if ((player->GetX() < (WinWidth*4/5)-player->GetRatioSize().cx) &&  
			(player->GetY() < (WinHeight*4/5)-player->GetRatioSize().cy))
		{
			scn_scrolling = false;
		}
		if ((player->GetX() >= (WinWidth*4/5)-player->GetRatioSize().cx) &&  
			(player->GetY() >= (WinHeight*4/5)-player->GetRatioSize().cy))
		{
			POINT spPT = {player->GetX()+player->GetRatioSize().cx, 
				          player->GetY()+player->GetRatioSize().cy};
			POINT scenePT = {SceneX+SceneWidth, SceneY+SceneHeight};
			T_Util::GetBevelSpeed(spPT, scenePT, player->GetSpeed(), speedX, speedY);
			speedX = -speedX;
			speedY = -speedY;
			scn_scrolling = true;
		}
		break;
	}
	if (scn_scrolling == true) 
	{
		MoveScene((int)speedX, (int)speedY);
	}
}

void T_Scene::Append(GAMELAYER gm_layer)
{
	sceneLayers.push_back(gm_layer);
	LayerChanged = true;
}

void T_Scene::Insert(GAMELAYER gm_layer, int index)
{
	int p_index = 0;
	SCENE_LAYERS::iterator p;
	for (p = sceneLayers.begin(); p != sceneLayers.end(); p++) 
	{
		if(p_index == index)
		{
			sceneLayers.insert(p, gm_layer);
			break;
		}
		p_index++;
	}
}

void T_Scene::Remove(GAMELAYER gm_layer)
{
	SCENE_LAYERS::iterator p;
	for (p = sceneLayers.begin(); p != sceneLayers.end(); p++) 
	{
		if((*p).layer == gm_layer.layer)
		{
			sceneLayers.erase(p);
			break;
		}
	}
}

void T_Scene::RemoveAll()
{	
	SCENE_LAYERS::iterator vp = sceneLayers.begin();
	for (; vp != sceneLayers.end();) 
	{
		if(vp->layer)
		{
			delete vp->layer;
			vp->layer = NULL;
			vp = sceneLayers.erase(vp);
		}
	}
	sceneLayers.clear();
	sceneLayers.swap(vector<GAMELAYER>()) ;
}

bool T_Scene::SortByZorder(const GAMELAYER &l1, const GAMELAYER &l2)
{
	return l1.z_order < l2.z_order;
}

void T_Scene::SortLayers()
{
	sort(sceneLayers.begin(),sceneLayers.end(),SortByZorder);  
	LayerChanged = false;
}

POINT T_Scene::getRandomFreeCell()
{
	POINT cellPos = {0, 0};
	int row = 0, col = 0;
	do 
	{
		row = rand()%(pBarrier->getMapRows()-1);
		col = rand()%(pBarrier->getMapCols()-1);
		if(row==0) row = 1;
		if(col==0) col = 1;

		bool isFree = true;

		for(int r=row-1; r<=row+1; r++)
		{
			for(int c=col-1; c<=col+1; c++)
			{
				if(pBarrier->getTile(c, r)!=0 ||
					pMask->getTile(c, r)!=0)
				{
					isFree = false;
					break;
				}
			}
			if(isFree == false)
			{
				break;
			}
		}

		if(isFree == true)
		{
			break;
		}
	}while(1);
	cellPos.x = (col-1)*pBarrier->getTileWidth();
	cellPos.y = (row-1)*pBarrier->getTileHeight();
	cellPos.x = SceneX + cellPos.x;
	cellPos.y = SceneY + cellPos.y;
	return cellPos;
}

void T_Scene::GetTxtMapValue(wstring str, wchar_t* keywords, int& value)
{
	int pos;
	pos = str.find(keywords);
	if(pos !=-1)
	{
		pos=str.find(L"=");
		value = _wtoi(str.substr(pos+1, wcslen(str.c_str())).c_str());
	}
}

void T_Scene::GetTxtMapValue(wstring str, wchar_t* keywords, wstring& value)
{
	int pos;
	pos = str.find(keywords);
	if(pos !=-1)
	{
		pos=str.find(L"=");
		value = str.substr(pos+1, wcslen(str.c_str())).c_str();
		pos=value.find(L">");
		if(pos !=-1)
		{
			value = value.substr(0, pos);
		}
	}
}

void T_Scene::parseCsvData(wstring csv_data, LAYERINFO& m_LayerInfo)
{
	wstring data = csv_data;

	vector<int> layerDataRow( m_LayerInfo.map_cols );
	int m_LayerRow = 0;
	int m_LayerCol = 0;
	for (int i = 0; i < m_LayerInfo.map_rows; i++)
	{
		m_LayerInfo.data.push_back( layerDataRow );
	}
	int pos =0;
	while(pos !=-1 && m_LayerRow<m_LayerInfo.map_rows)
	{
		pos= data.find(',');
		int gid = _wtoi(data.substr(0,pos).c_str());
		data = data.substr(pos+1, data.size());

		m_LayerInfo.data[m_LayerRow][m_LayerCol] = gid;

		m_LayerCol++;
		if(m_LayerCol == m_LayerInfo.map_cols)
		{
			m_LayerCol = 0;
			m_LayerRow++;
		}
	}
}

bool T_Scene::LoadTxtMap(const char* txtmap_path)
{
	string str;
	wifstream infile(txtmap_path,ios::in);
	if(!infile)
	{
		return false;
	}
	wchar_t* l_str = new wchar_t[1024];
	wstring line_str;
	wstring mapdata= L"";
	bool startReadMapData=false;
	int layerCount=0;
	wstring barrierName = L"";
	wstring maskName = L"";
	wstring bkgName = L"";
	wstring currentlayerName = L"";
	LAYERINFO layerInfo;
	GAMELAYER mapLayer;

	while(!infile.eof())
	{
		infile.getline(l_str, 1024);
		line_str = wstring(l_str);
		GetTxtMapValue(line_str, L"map_cols", layerInfo.map_cols);
		GetTxtMapValue(line_str, L"map_rows", layerInfo.map_rows);
		GetTxtMapValue(line_str, L"tile_width", layerInfo.tile_width);
		GetTxtMapValue(line_str, L"tile_height", layerInfo.tile_height);
		GetTxtMapValue(line_str, L"tile_path", layerInfo.tile_path);
		GetTxtMapValue(line_str, L"tile_grid_id", layerInfo.first_gid);
		GetTxtMapValue(line_str, L"back_layer_name", bkgName);
		GetTxtMapValue(line_str, L"barrier_layer_name", barrierName);
		GetTxtMapValue(line_str, L"mask_layer_name", maskName);
		if(line_str.find(L"<layer")!=-1)
		{
			GetTxtMapValue(line_str, L"name", currentlayerName);
			int pos = line_str.find(L">");
			if(pos>0 && pos < (int)(line_str.length()-1))
			{
				line_str = line_str.substr(pos+1, line_str.length());
				mapdata = mapdata+line_str;
			}
			startReadMapData = true;
			continue;
		}
		if(startReadMapData == true)
		{
			if(line_str.find(L"</layer>") == -1)
			{
				mapdata = mapdata+line_str;
			}
			if(line_str.find(L"</layer>") != -1)
			{
				startReadMapData = false;
				parseCsvData(mapdata, layerInfo);
				mapLayer.layer = new T_Map(layerInfo);
				if( currentlayerName == bkgName && 
					currentlayerName != L"" && bkgName!= L"")
				{
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BACK);
					mapLayer.type_id = LAYER_MAP_BACK;
					layerInfo.type_id = LAYER_MAP_BACK;
				}
				if(currentlayerName == barrierName && 
				   currentlayerName != L"" && barrierName!= L"")
				{
					pBarrier = (T_Map*)mapLayer.layer;
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_BARR);
					mapLayer.type_id = LAYER_MAP_BARR;
					layerInfo.type_id = LAYER_MAP_BARR;
					
				}
				if(currentlayerName == maskName && 
				   currentlayerName != L"" && maskName!= L"")
				{
					mapLayer.z_order = LAYER_MAX;
					mapLayer.layer->setZorder(LAYER_MAX);
					pMask = (T_Map*)mapLayer.layer;
					mapLayer.layer->SetLayerTypeID(LAYER_MAP_MASK);
					mapLayer.type_id = LAYER_MAP_MASK;
					layerInfo.type_id = LAYER_MAP_MASK;
				}
				else
				{
					mapLayer.z_order = layerCount;
					mapLayer.layer->setZorder(layerCount);
				}
				sceneLayers.push_back(mapLayer);
				mapdata = L"";
				layerInfo.data.clear();
				layerInfo.type_id = -1;
				layerCount = layerCount + 1;
			}
		}
	}
	delete[] l_str;
	return true;
}

void T_Scene::Draw(HDC hdc, int x, int y)
{	
	int offsetX =0;
	int offsetY =0;
	offsetX = (abs(lastSceneX) - abs(SceneX));
	offsetY = (abs(lastSceneY) - abs(SceneY));
	if(LayerChanged == true)
	{
		SortLayers();
	}
	int pX, pY;
	SCENE_LAYERS::iterator p;
	for (p = sceneLayers.begin(); p != sceneLayers.end(); p++) 
	{
		pX = (*p).layer->GetX();
		pX += offsetX;
		pY = (*p).layer->GetY();
		pY += offsetY;
		(*p).layer->SetPosition(pX, pY);
		if((*p).layer->IsVisible() == true)
		{
			(*p).layer->Draw(hdc);
		}
	}
	lastSceneX = SceneX;
	lastSceneY = SceneY;
}