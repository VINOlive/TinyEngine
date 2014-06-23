#include "FlyGame.h"

// 游戏中的资源参数
NPCTYPEINFO FlyGame::NpcSprites[NPC_TYPE_NUM+1]=
{
	//imgName					frameWidth	frameHeight	Speed	Score	Ratio
	//------------------------------------------------------------------------
	{L"npc1.png",				50,			40,			4,		10,		0.8f	,1},
	{L"npc2.png",				75,			95,			2,		20,		0.8f	,4},
	{L"npc3.png",				170,		245,		1,		50,		0.8f	,8},
	{L"npc1.png",				50,			40,			8,		10,		0.8f	,1},
	{L"npc2.png",				75,			95,			4,		20,		0.8f	,4},
};

// 构造函数
FlyGame::FlyGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, 
				   WORD icon, WORD sm_icon, int winwidth, int winheight)
				   :T_Engine(h_instance, sz_winclass, sz_title, icon, sm_icon, winwidth, winheight)
{	
	wnd_width = winwidth;
	wnd_height = winheight;
}

// 析构函数
FlyGame::~FlyGame(void)
{
	
}

void FlyGame::GameEnd() {
	GameLevelClear();
}

//游戏开始
void FlyGame::GameInit(){
	level=1;
	count=0;
	time=0;
	award1num=0;
	award2time=0;

	GameState = GAME_START;
	LoadSound(m_hWnd);
	gameMenu = NULL;
	LoadGameMenu(GameState);
	GameLevelRun();
}

//游戏鼠标行为处理（主要为菜单的点击、MOVE事件）
void FlyGame::GameMouseAction(int x, int y, int Action)	
{
	mouse_pt.x = x;
	mouse_pt.y = y;
	if(Action == MOUSE_MOVE)
	{
		if(GameState != GAME_RUN)
		{
			gameMenu->MenuMouseMove(x, y);
		}
	}
	if(Action == MOUSE_LCLICK)
	{
		//游戏开始的菜单
		if(GameState == GAME_START)
		{
			int index = gameMenu->MenuMouseClick(x, y);
			if(index >=0)
			{
				switch(index)
				{
				case 0:
					GameState = GAME_RUN;
					PlayGameSound();
					break;
				case 1:
					GameState = GAME_HELP;
					delete gameMenu;
					gameMenu = NULL;
					LoadGameMenu(GAME_HELP);
					break;
				case 2:
					GameState = GAME_ABOUT;
					delete gameMenu;
					gameMenu = NULL;
					LoadGameMenu(GAME_ABOUT);
					break;
				case 3:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
				}
			}
			return;
		}
		//结束的菜单
		if(GameState == GAME_UPGRADE || GameState == GAME_WIN || GameState == GAME_OVER)
		{
			int index = gameMenu->MenuMouseClick(x, y);
			if(index >=0)
			{
				switch(index)
				{
				case 0://重新开始
					{
						if (GameState==GAME_OVER||GameState==GAME_WIN)
						{
							level=1;
							count=0;
							time=0;
							award1num=0;
							award2time=0;
						}
						GameLevelRun();
						GameState = GAME_RUN;
						PlayGameSound();
						break;
					}
				case 1:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
				}
			}
			return;
		}

		if(GameState == GAME_HELP || GameState == GAME_ABOUT)
		{
			int index = gameMenu->MenuMouseClick(x, y);
			if(index >=0)
			{
				switch(index)
				{
				case 0:
					GameState = GAME_START;
					delete gameMenu;
					gameMenu = NULL;
					LoadGameMenu(GAME_START);
					break;
				case 1:
					SendMessage(m_hWnd, WM_SYSCOMMAND, SC_CLOSE, 0);
				}
			}
		}
	}
}


//游戏窗口重绘
void FlyGame::GamePaint(HDC hdc){
	if( GameState == GAME_START || 
		GameState == GAME_HELP || 
		GameState == GAME_ABOUT)
	{
		gameMenu->DrawMenu(hdc);
	}
	if (GameState==GAME_RUN)
	{
		t_scene->Draw(hdc, 0, 0);	
	}
	if(GameState == GAME_UPGRADE || 
		GameState == GAME_OVER || 
		GameState == GAME_WIN)
	{
		gameMenu->DrawMenu(hdc);
	}
	DisplayInfo(hdc, GameState);
}
//游戏逻辑
void FlyGame::GameLogic(){
	srand(GetTickCount());
	GameKeyAction();
	if (GameState==GAME_RUN)
	{
		UpdateNpcPos();
		UpdateAwardPos();
		UpdateBombPos(&player_bomb_set);
		UpdateAnimation();

		//判断是否过关或者WIN
		if (count>=400*level) 
		{
			level++;
			if (level>=10)
			{
				GameState=GAME_WIN;
				PlayGameSound();
				GameLevelClear();
			}
			else
			{
				GameState=GAME_UPGRADE;
				PlayGameSound();
				GameLevelClear();
			}
		}
		//生成NPC的策略
		time++;
		if (time%30==0)
			LoadNpc(1,0);//生成第一种NPC
		if (time%50==0)
			LoadNpc(1,3);//生成第四种NPC
		if(time%100==0)
			LoadNpc(1,1);//生成第二种NPC
		if(time%200==0)
			LoadNpc(1,4);//生成第五种NPC
		if(time%800==0)
			LoadNpc(1,2);//生成第三种NPC
		if(time==800)
			time=0;
		if(time%300==0)
			LoadAward(rand()%2+1);//随机生成奖励
		if(award2time>0)//更新奖励1的剩余时间（即全灭）
			award2time--;

		//自动发射子弹
		if(!player->IsDead())
		{
			if (award2time>0){
				LoadBomb(player, player_bomb_set, 400,2);//有双子弹的加载双子弹
				bulletSound->Play();
			}
			else{
				LoadBomb(player, player_bomb_set, 400);//默认加载普通子弹
				bulletSound->Play();
			}
		}

	}
}

//游戏按键处理（主要是玩家操作）
void FlyGame::GameKeyAction(int Action)
{
	int SpeedX,SpeedY;
	int nextStepX,nextStepY;
	if(Action == KEY_SYS_NONE)//根据keys数组状态处理按键行为
	{
		if(GameState == GAME_RUN)
		{
			if(CheckKey(VK_LEFT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
			{
				player->SetActive(true);
				SpeedX = -player->GetSpeed();
				SpeedY = 0;
				//计算下一步移动是否超过边界
				nextStepX = player->GetX()-player->GetSpeed();
				if(nextStepX <= 0) 
				{
					SpeedX = 0 - player->GetX();
				}
				player->Move(SpeedX, SpeedY);	// 继续移动
			}
			if(CheckKey(VK_RIGHT) && !CheckKey(VK_DOWN) && !CheckKey(VK_UP))
			{
				player->SetActive(true);
				SpeedX =player->GetSpeed();
				SpeedY = 0;
				//计算下一步移动是否超过边界
				nextStepX = player->GetX()+ player->GetRatioSize().cx + player->GetSpeed();
				if(nextStepX >= wnd_width) 
				{
					SpeedX = wnd_width - player->GetRatioSize().cx - player->GetX();
				}
				player->Move(SpeedX, SpeedY);	// 继续移动
			}
			if(CheckKey(VK_UP) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
			{
				player->SetActive(true);
				SpeedX = 0;
				SpeedY = -player->GetSpeed();
				//计算下一步移动是否超过边界
				nextStepY = player->GetY()-player->GetSpeed();
				if(nextStepY <= 0) 
				{
					SpeedY = 0 - player->GetY();
				}
				player->Move(SpeedX, SpeedY);	// 继续移动
			}
			if(CheckKey(VK_DOWN) && !CheckKey(VK_LEFT) && !CheckKey(VK_RIGHT))
			{
				player->SetActive(true);
				SpeedX = 0;
				SpeedY = player->GetSpeed();
				//计算下一步移动是否超过边界
				nextStepY = player->GetY() + player->GetRatioSize().cy + player->GetSpeed();
				if(nextStepY >= wnd_height) 
				{
					SpeedY = wnd_height - player->GetRatioSize().cy - player->GetY();
				}
				player->Move(SpeedX, SpeedY);	// 继续移动
			}

			if(CheckKey(VK_LEFT) == false && CheckKey(VK_RIGHT)==false &&
				CheckKey(VK_UP)==false && CheckKey(VK_DOWN)==false)
			{
				player->SetActive(false);
			}
		}
	}
	if (Action==KEY_DOWN)
	{
		if(GetAsyncKeyState(VK_SPACE)<0)
		{
			if(award1num>0)
			{
				KillAllNpc();	
				award1num = award1num - 1;
			}
			LoadBomb(player, player_bomb_set, 400,2);
		}
	}
}
//游戏过关升级后下一个资源加载
void FlyGame::GameLevelRun()
{
	award1Img = NULL;
	totalscoreBmp = NULL;
	t_scene = NULL;
	if(t_scene == NULL)
	{
		t_scene = new T_Scene();
	}
	LoadMap();
	LoadPlayer();
	LoadImageRes();
	PlayGameSound();
}
//游戏过关升级后上一个资源释放
void FlyGame::GameLevelClear()
{
	if(gameMenu)
	{
		delete gameMenu;		
		gameMenu = NULL;
	}
	if (t_scene)
	{
		delete t_scene;
		t_scene =NULL;
	}
	if (award1Img)
	{
		delete award1Img;
		award1Img=NULL;
	}
	if (totalscoreBmp)
	{
		delete totalscoreBmp;
		totalscoreBmp=NULL;
	}
	LoadGameMenu(GameState);  //加载过关升级的菜单
	npc_set.clear();	npc_set.swap(vector<T_Sprite*>());
	award_set.clear();	award_set.swap(vector<T_Sprite*>());
	player_bomb_set.clear();	player_bomb_set.swap(vector<T_Sprite*>());
}

// 加载游戏图片资源
void FlyGame::LoadImageRes()
{
	if(award1Img == NULL) award1Img = new T_Graph(L".\\res\\game\\bomb.png");
	if(totalscoreBmp == NULL) totalscoreBmp = new T_Graph(L".\\res\\game\\score.png");
}

// 加载游戏菜单
void FlyGame::LoadGameMenu(int type)
{
	int btnWidth = 150;
	int btnHeight = 60;
	if(gameMenu == NULL) gameMenu = new T_Menu();
	gameMenu->SetMenuIndex(-1);
	if(type == GAME_START)
	{
		wstring menuItems[] = {L"开始游戏  ", L"游戏帮助  ", L"关于游戏  ", L"退出游戏  "};
		gameMenu->SetMenuBkg(L".\\res\\game\\first.png", 255, RGB(0,0,0));
		SetMenuPara(menuItems, 4, btnWidth, btnHeight,0);
	}
	if(type == GAME_OVER)
	{
		wstring menuItems[] = {L"重新开始", L"退出游戏"};
		gameMenu->SetMenuBkg(L".\\res\\game\\over.png", 255, RGB(0,0, 0));
		SetMenuPara(menuItems, 2, btnWidth, btnHeight, 1);
	}
	if(type == GAME_UPGRADE)
	{
		wstring menuItems[] = {L"下一关", L"退出游戏"};
		gameMenu->SetMenuBkg(L".\\res\\game\\1.png", 255, RGB(0,0, 0));
		SetMenuPara(menuItems, 2, btnWidth, btnHeight, 1);
	}
	if(type == GAME_WIN)
	{
		wstring menuItems[] = {L"重新玩", L"退出游戏"};
		gameMenu->SetMenuBkg(L".\\res\\game\\3.png", 255, RGB(0,0, 0));
		SetMenuPara(menuItems, 2, btnWidth, btnHeight, 1);
	}
	if(type == GAME_ABOUT)
	{
		wstring menuItems[] = {L"返回菜单", L"退出游戏"};
		gameMenu->SetMenuBkg(L".\\res\\game\\about.png", 255, RGB(0,0, 0));
		SetMenuPara(menuItems, 2, btnWidth, btnHeight, 2);
	}
	if(type==GAME_HELP)
	{
		wstring menuItems[] = {L"返回菜单", L"退出游戏"};
		gameMenu->SetMenuBkg(L".\\res\\game\\back.png", 255, RGB(0,0, 0));
		SetMenuPara(menuItems, 2, btnWidth, btnHeight, 2);
	}
}


//设置菜单样式
void FlyGame::SetMenuPara(wstring* menuItems, int itemSize, int m_w, int m_h, int posType)
{
	int space = 30;
	int y = 0,x=0;
	for(int i=0; i<itemSize; i++)
	{

		switch(posType)
		{
			case 0:
				x=(wnd_width-m_w)/2;
				y=300+i*m_h;
				break;
			case 1:
				y=300;
				x=50+i*m_w;
				break;
			case 2:
				y=450;
				x=40+i*m_w;
				break;
		}
		MENUITEM mItem;
		mItem.pos.x = x;
		mItem.pos.y = y;
		mItem.ItemName = menuItems[i];
		gameMenu->AddMenuItem(mItem);
	}
	gameMenu->SetMoveSound(mouseOverSound);
	gameMenu->SetClickSound(mouseDownSound);
	MENU_INFO menuInfo;
	menuInfo.align = 1;
	menuInfo.space = space;
	menuInfo.width = 150;
	menuInfo.height = 60;
	menuInfo.fontName = L"黑体";
	menuInfo.isBold = true;
	menuInfo.normalTextColor = Color::White;
	menuInfo.focusTextColor = Color::Red;
	gameMenu->SetMenuInfo(menuInfo);
}

//加载地图背景
void FlyGame::LoadMap()
{
	GAMELAYER bkgLayer;
	int scn_width,scn_height;
	bkgLayer.type_id = LAYER_MAP_BACK;
	bkgLayer.z_order = t_scene->getSceneLayers()->size()+1;
	bkgLayer.layer = new T_Map(L".\\res\\game\\back.png"); 
	t_scene->getSceneLayers()->push_back(bkgLayer);
	scn_width = bkgLayer.layer->GetWidth();
	scn_height =bkgLayer.layer->GetHeight();
	// 视图初始位置以地图作为参照
	int scn_x = (wnd_width-scn_width)/2;
	int scn_y = (wnd_height-scn_height)/2;
	// 将游戏地图初始化为屏幕中央位置
	t_scene->InitScene(scn_x, scn_y, scn_width, scn_height, wnd_width, wnd_height);
	bkgLayer.layer->SetPosition(scn_x, scn_y);
}

//加载玩家角色
void FlyGame::LoadPlayer()
{
	GAMELAYER gameLayer;
	SPRITEINFO fish_Info;

	// 加载玩家角色
	player = new T_Sprite(L".\\res\\game\\player.png", 100, 120);
	fish_Info.Active = true;
	fish_Info.Dead = false;
	fish_Info.Dir = DIR_UP;
	fish_Info.Rotation = TRANS_NONE;
	fish_Info.Ratio =0.8f;
	fish_Info.Level = 1;//游戏关数
	fish_Info.Score = 0;
	fish_Info.Speed = 8;
	fish_Info.Alpha = 200;
	fish_Info.Visible = true;
	player->Initiate(fish_Info);
	player->SetLayerTypeID(LAYER_PLY);
	int x = 175;
	int y = 500;
	player->SetPosition(x, y);

	gameLayer.layer = player;
	gameLayer.type_id = LAYER_PLY;
	gameLayer.z_order = t_scene->getSceneLayers()->size()+1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);

	player->SetStartTime(GetTickCount());
}






//加载NPC
void FlyGame::LoadNpc(int num, int type)
{
	GAMELAYER gameLayer;
	SPRITEINFO fish_Info;
	srand(GetTickCount());
	for(int i=0; i<num;  i++)
	{
		fish_Info.Active = true;
		fish_Info.Dead = false;
		fish_Info.Rotation = TRANS_NONE;
		fish_Info.Alpha = 230;
		fish_Info.Visible = true;
		fish_Info.Level = level;

		fish_Info.Life = NpcSprites[type].Life;
		fish_Info.Ratio = NpcSprites[type].Ratio;
		fish_Info.Speed = NpcSprites[type].Speed+level;
		fish_Info.Score = NpcSprites[type].Score;

		int sp_width = NpcSprites[type].frameWidth;
		int sp_height = NpcSprites[type].frameHeight;
		LPCTSTR img_name = NpcSprites[type].imgName;

		//抽取随机方向(NPC角色在窗口两边生成)
		fish_Info.Dir = DIR_LEFT;
		fish_Info.X =rand()%(wnd_width-sp_width);
		fish_Info.Y =0;

		//在NPC列表中增加新的项目
		wstring path = L".\\res\\game\\";
		path.append(img_name);
		npc_set.push_back(new T_Sprite(path.c_str(),  sp_width, sp_height));

		//初始化刚增加的项目
		T_Sprite *sp = npc_set.back();
		sp->Initiate(fish_Info);
		sp->SetLayerTypeID(LAYER_NPC); 

		gameLayer.layer = sp;
		gameLayer.type_id = LAYER_NPC;
		gameLayer.z_order = t_scene->getSceneLayers()->size()+1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		t_scene->Append(gameLayer);
		sp = NULL;
	}
}
//更新NPC的位置
void FlyGame::UpdateNpcPos()
{
	if(npc_set.size()==0) return;
	T_AI* spAi = new T_AI(8);
	vSpriteSet::iterator p;
	for (p = npc_set.begin(); p != npc_set.end(); p++) 
	{
		if((*p)->IsActive()==true && (*p)->IsVisible()==true)
		{
			int SpeedX=0, SpeedY=0;
			int npc_dir=0;
			npc_dir = (*p)->GetDir();
			int y=(*p)->GetY()+(*p)->GetSpeed();
			(*p)->SetPosition((*p)->GetX(),y);
			(*p)->SetRotation(TRANS_NONE);
			// 与玩家角色的碰撞处理
			if( (*p)->CollideWith(player) && (*p)->IsDead() == false && (*p)->IsVisible())
			{
				player->SetDead(true);
				player->SetActive(false);;
				bkgMusic->Stop();
				gameOverSound->Play(false);
				return ;
			}
		}
	}
	delete spAi;
}


// 发射炮弹处理(sp:发射炮弹的角色; bombSet:炮弹集合; iTime:发射炮弹的间隔时间)
void FlyGame::LoadBomb(T_Sprite* sp, vSpriteSet& bombSet, int iTime,int type)
{
	sp->SetEndTime(GetTickCount());
	if(sp->GetEndTime() - sp->GetStartTime() >= (DWORD)iTime)
	{
		sp->SetStartTime(sp->GetEndTime());
		GAMELAYER gameLayer;
		SPRITEINFO bombInfo;
		int m_dir = sp->GetDir();

		wchar_t path[256];
		if (type==1)
		{
			wsprintf(path, L".\\res\\game\\bullet1.png");
		}
		else if (type==2)
		{
			wsprintf(path, L".\\res\\game\\bullet2.png");
		}
		bombSet.push_back(new T_Sprite(path));
		//初始化刚增加的项目
		T_Sprite *bomb = bombSet.back();
		bombInfo.Active = true;
		bombInfo.Dead = false;
		bombInfo.Dir = m_dir;
		bombInfo.Rotation = TRANS_NONE;
		bombInfo.Ratio =1;
		bombInfo.Level = 0;
		bombInfo.Score = 0;
		bombInfo.Speed = 8;
		bombInfo.Alpha = 255;

		if (type==2)
			bombInfo.X = sp->GetX()+sp->GetRatioSize().cx/2 - 12;
		else if(type==1)
			bombInfo.X = sp->GetX()+sp->GetRatioSize().cx/2 - 2;
		bombInfo.Y = sp->GetY() - 16;


		bombInfo.Visible = true;
		bomb->Initiate(bombInfo);

		gameLayer.layer = bomb;
		if(bombSet == player_bomb_set)
		{
			if (type==2)
			{
				gameLayer.type_id = LAYER_PLY_BOMB2;
				gameLayer.layer->SetLayerTypeID(LAYER_PLY_BOMB2);
			}
			else
			{
				gameLayer.type_id = LAYER_PLY_BOMB;
				gameLayer.layer->SetLayerTypeID(LAYER_PLY_BOMB);
			}
		}
		gameLayer.z_order = t_scene->GetTotalLayers()+1;
		gameLayer.layer->setZorder(gameLayer.z_order);
		t_scene->Append(gameLayer);
	}
}
// 更新炮弹的位置
void FlyGame::UpdateBombPos(vSpriteSet* bombSet)
{
	if(bombSet->size()==0) return;

	vSpriteSet::iterator itp = bombSet->begin();
	for(; itp != bombSet->end();)
	{
		if((*itp)->IsVisible()==true && (*itp)->IsActive()==true)
		{
			int SpeedX=0, SpeedY=0;
			switch((*itp)->GetDir())
			{
			case DIR_LEFT:
				SpeedX = -(*itp)->GetSpeed();
				SpeedY = 0;
				break;

			case DIR_RIGHT:
				SpeedX =(*itp)->GetSpeed();
				SpeedY = 0;
				break;

			case DIR_UP:
				SpeedX = 0;
				SpeedY = -(*itp)->GetSpeed();
				break;

			case DIR_DOWN:
				SpeedX = 0;
				SpeedY = (*itp)->GetSpeed();
				break;
			}
			(*itp)->Move(SpeedX, SpeedY);

			//检测炮弹是否击中目标
			BombCollide((*itp));
			
			if(GameState != GAME_RUN) return;
		}

		if((*itp)->IsVisible() == false ||
			(*itp)->GetY() < 0 || (*itp)->GetX() < 0 ||
			(*itp)->GetY() > wnd_height || (*itp)->GetX() > wnd_width )
		{
			//删除场景中的对象	
			SCENE_LAYERS::iterator p;
			for (p = t_scene->getSceneLayers()->begin();
				p != t_scene->getSceneLayers()->end(); p++) 
			{
				if((*p).layer == (*itp))
				{
					p = t_scene->getSceneLayers()->erase(p);
					break;
				}
			}
			delete (*itp);
			itp = bombSet->erase(itp);
		}

		if(itp == bombSet->end())
		{
			break;
		}

		itp++;
	}
}


// 炮弹碰撞处理
void FlyGame::BombCollide(T_Sprite* bomb)
{
	if(bomb->IsActive()==true && bomb->IsVisible()==true)
	{
		// 如果玩家炮弹碰到了NPC
		if(bomb->GetLayerTypeID() == LAYER_PLY_BOMB)
		{
			for (vSpriteSet::iterator sp = npc_set.begin(); sp != npc_set.end(); sp++) 
			{
				if(bomb->CollideWith((*sp)) && !((*sp)->IsDead()) && 
					((*sp)->IsVisible()) && ((*sp)->IsActive()))
				{
					bomb->SetActive(false);
					bomb->SetVisible(false);
					int curlife=(*sp)->GetLife();
					(*sp)->SetLife(curlife-1);
					if ((*sp)->GetLife()<=0)
					{
						(*sp)->SetActive(false);
						(*sp)->SetDead(true);
						explosionSound->Play(false);
						count+=(*sp)->GetScore();
						break;
					}
				}
			}
		}
		if (bomb->GetLayerTypeID() == LAYER_PLY_BOMB2)
		{
			for (vSpriteSet::iterator sp = npc_set.begin(); sp != npc_set.end(); sp++) 
			{
				if(bomb->CollideWith((*sp)) && !((*sp)->IsDead()) && 
					((*sp)->IsVisible()) && ((*sp)->IsActive()))
				{
					bomb->SetActive(false);
					bomb->SetVisible(false);

					int curlife=(*sp)->GetLife();
					(*sp)->SetLife(curlife-2);
					if ((*sp)->GetLife()<=0)
					{
						(*sp)->SetActive(false);
						(*sp)->SetDead(true);
						explosionSound->Play(false);
						count+=(*sp)->GetScore();
						break;
					}
				}
			}
		}
	}
}

//奖励，type=1-全灭，type=2-双子弹，
void FlyGame::LoadAward(int type)
{
	GAMELAYER gameLayer;
	SPRITEINFO fish_Info;
	srand(GetTickCount());
	fish_Info.Active = true;
	fish_Info.Dead = false;
	fish_Info.Rotation = TRANS_NONE;
	fish_Info.Alpha = 230;
	fish_Info.Visible = true;
	fish_Info.Ratio = 0.8f;
	fish_Info.Speed = 8;
	int sp_width = 65;
	int sp_height = 90;
	//抽取随机方向(NPC角色在窗口两边生成)
	fish_Info.X =rand()%(wnd_width-sp_width);
	fish_Info.Y =0;
	//在NPC列表中增加新的项目
	wstring path;
	if(type==1)
		path.append(L".\\res\\game\\award1.png");
	else
		path.append(L".\\res\\game\\award2.png");
	award_set.push_back(new T_Sprite(path.c_str(),  sp_width, sp_height));

	//初始化刚增加的项目
	T_Sprite *sp = award_set.back();
	sp->Initiate(fish_Info);
	gameLayer.layer = sp;
	if (type==1)
	{
		sp->SetLayerTypeID(LAYER_AWARD1); 
		gameLayer.type_id = LAYER_AWARD1;
	}
	else if (type==2)
	{
		sp->SetLayerTypeID(LAYER_AWARD2); 
		gameLayer.type_id = LAYER_AWARD2;
	}
	gameLayer.z_order = t_scene->getSceneLayers()->size()+1;
	gameLayer.layer->setZorder(gameLayer.z_order);
	t_scene->Append(gameLayer);
	sp = NULL;
}

// 更新奖品的位置
void FlyGame::UpdateAwardPos()
{
	if(award_set.size()==0) return;
	vSpriteSet::iterator p;
	for (p = award_set.begin(); p != award_set.end(); p++) 
	{
		if((*p)->IsActive()==true && (*p)->IsVisible()==true)
		{
			int SpeedX=0, SpeedY=0;
			int npc_dir=0;
			npc_dir = (*p)->GetDir();
			int y=(*p)->GetY()+(*p)->GetSpeed();
			(*p)->SetPosition((*p)->GetX(),y);

			(*p)->SetRotation(TRANS_NONE);

			// 与玩家角色的碰撞处理
			if( (*p)->CollideWith(player) && (*p)->IsDead() == false && (*p)->IsVisible())
			{

				(*p)->SetVisible(false);
				(*p)->SetDead(true);
				(*p)->SetActive(false);;
				if ((*p)->GetLayerTypeID()==LAYER_AWARD1)
				{
					award1num++;
					powerupSound1->Play(false);
				}
				else if ((*p)->GetLayerTypeID()==LAYER_AWARD2)
				{
					award2time=200;
					powerupSound2->Play(false);
				}
				return ;
			}
		}
	}
}
//全灭
void FlyGame::KillAllNpc()
{
	int i=npc_set.size();
	vSpriteSet::iterator sp;
	for (sp = npc_set.begin(); sp != npc_set.end(); sp++) 
	{
		if(!((*sp)->IsDead()) &&((*sp)->IsVisible()) &&((*sp)->IsActive()))
		{
			(*sp)->SetActive(false);
			(*sp)->SetDead(true);
			explosionSound->Play(false);
			count+=(*sp)->GetScore();
		}
	}
}


// 更新角色动画帧序列号
void FlyGame::UpdateAnimation()
{
	vSpriteSet::iterator p;
	if(npc_set.size()>0)
	{
		for (p = npc_set.begin(); p != npc_set.end(); p++) 
		{
			if ((*p)->IsDead()&&(*p)->IsVisible())
			{
				if ((*p)->LoopFrameOnce())
				{
					(*p)->SetVisible(false);
				}
			}
		}
	}
	if(award_set.size()>0)
	{
		for (p = award_set.begin(); p != award_set.end(); p++) 
		{
			(*p)->LoopFrame();
		}
	}
	if(player != NULL)
	{
		if(player->IsVisible()==true && player->IsDead()==true)
		{
			if (player->LoopFrameOnce())
			{
				player->SetVisible(false);
				GameState=GAME_OVER;
				GameLevelClear();
			}
		}
	}
}
// 游戏状态显示
void FlyGame::DisplayInfo(HDC hdc, int game_state)
{
	int FontHeight = 0;//字号
	Gdiplus::RectF rect;
	wstring Content = L"";
	rect.X = 50.00;
	rect.Width = 768.00;

	switch(game_state)
	{
	case GAME_RUN:
		{
			int strLen = 220;
			int iconW = 40;
			int x = 12;
			int y = 16;
			rect.Y = (REAL)y;
			rect.Height = (REAL)30;
			rect.Width = (REAL)200;
			FontHeight = 20;

			totalscoreBmp->PaintImage(
				hdc, x, y-4, totalscoreBmp->GetImageWidth(), 
				totalscoreBmp->GetImageHeight(), 200
				);
			wstring ScoreNum = L"";
			ScoreNum.append(T_Util::int_to_wstring(count));
			rect.X = (REAL)(x+iconW);
			T_Graph::PaintText(hdc, rect, ScoreNum.c_str(), (REAL)FontHeight, L"黑体",
				Color(143, 146, 147, 0),FontStyleBold, StringAlignmentNear);
		
			x = x + strLen;
			award1Img->PaintImage(
				hdc, x, y-4, award1Img->GetImageWidth(), 
				award1Img->GetImageHeight(), 200
				);
			wstring LeveNum = L" X ";
			LeveNum.append(T_Util::int_to_wstring(award1num));
			rect.X = (REAL)(x+iconW);
			T_Graph::PaintText(hdc, rect, LeveNum.c_str(), (REAL)FontHeight, L"黑体",
				Color(143, 146, 147, 0), FontStyleBold, StringAlignmentNear);
		}
		break;

	case GAME_OVER:
		{
			rect.X = 300.00;
			rect.Y = 90.00;
			rect.Width = float(WIN_WIDTH);
			rect.Height = float(WIN_HEIGHT/2);
			FontHeight = 24;
			wstring ScoreNum = L"";
			ScoreNum.append(T_Util::int_to_wstring(count));
			T_Graph::PaintText(hdc, rect, ScoreNum.c_str(), (REAL)FontHeight, L"黑体",
				Color(143, 146, 147, 0), FontStyleBold, StringAlignmentNear);
		}
		break;
	case GAME_UPGRADE:
		{
			rect.X = 10.00;
			rect.Y = 80.00;
			rect.Width = float(WIN_WIDTH);
			rect.Height = float(WIN_HEIGHT/2);
			FontHeight = 32;
			wstring Str = L"第 ";
			Str.append(T_Util::int_to_wstring(level));
			Str.append(L"关");
			T_Graph::PaintText(hdc, rect,Str.c_str(), (REAL)FontHeight, L"黑体");
		}
		break;
	case GAME_WIN:
		{
			rect.X = 20.00;
			rect.Y = 80.00;
			rect.Width = float(WIN_WIDTH);
			rect.Height = float(WIN_HEIGHT/2);
			FontHeight = 36;
			T_Graph::PaintText(hdc, rect, L"恭喜通关！", (REAL)FontHeight, L"黑体");
		}
		break;
	case GAME_HELP:
		rect.X = 60.00;
		rect.Y = 100.00;
		rect.Width = float(WIN_WIDTH-120);
		rect.Height = 50;
		FontHeight = 24;
		T_Graph::PaintText(hdc, rect, L"帮  助", (REAL)FontHeight, L"黑体");

		rect.X = 60.00;
		rect.Y = 200.00;
		rect.Width = float(WIN_WIDTH-120);
		rect.Height = WIN_HEIGHT-450;
		FontHeight = 13;
		Content = L"    玩家使用上、下、左、右方向键来控制角色各个方向的行动";
		Content.append(L"游戏过程中随机出现“双发子弹”");
		Content.append(L"和“全屏炸弹”奖励。其中“双发子弹”会");
		Content.append(L"在持续一段时间后消失，“全屏炸弹”玩家可以按");
		Content.append(L"空格键来使用。");
		T_Graph::PaintText(hdc, rect, Content.c_str(), (REAL)FontHeight, L"黑体", 
			Color::White, FontStyleRegular, StringAlignmentNear);
		break;
	case GAME_ABOUT:
		rect.X = 60.00;
		rect.Y = 200.00;
		rect.Width = float(WIN_WIDTH-100);
		rect.Height = 80;
		FontHeight = 24;
		T_Graph::PaintText(hdc, rect, L"开发团队", (REAL)FontHeight, L"黑体");

		rect.X = 60.00;
		rect.Y = 205.00;
		rect.Width = float(WIN_WIDTH-100);
		rect.Height = WIN_HEIGHT-300;
		FontHeight = 12;
		Content = L"";
		Content.append(L"方  杰（中）：主要负责游戏代码实现\n");
		Content.append(L"郭振全（右）：主要负责游戏策划、素\n              材收集、图片素材处理\n");
		Content.append(L"补凯博（左）：主要负责游戏测试、素\n              材收集、音频素材处理\n\n");
		Content.append(L"版本: 1.0.0\n");
		Content.append(L"Copyright 2014\n");
		Content.append(L"All Rights Reserved\n");
		Content.append(L"保留所有权利\n");
		T_Graph::PaintText(hdc, rect, Content.c_str(), (REAL)FontHeight, L"黑体", 
			Color::White, FontStyleRegular, StringAlignmentNear);
	}	
}

// 加载游戏声音资源
void FlyGame::LoadSound(HWND hwnd)
{
	gameOverSound = new AudioDXBuffer;
	gameWinSound = new AudioDXBuffer;
	bulletSound = new AudioDXBuffer;
	bkgMusic = new AudioDXBuffer;
	mouseOverSound = new AudioDXBuffer;
	mouseDownSound = new AudioDXBuffer;
	explosionSound = new AudioDXBuffer;
	powerupSound1 = new AudioDXBuffer;
	powerupSound2 = new AudioDXBuffer;
	if(!dxSnd.CreateDS(hwnd))  return;
	bulletSound->LoadWave(dxSnd, L".\\res\\sound\\bullet.wav");//子弹声音
	bkgMusic->LoadWave(dxSnd, L".\\res\\sound\\backmusic.wav");//背景声音
	mouseOverSound->LoadWave(dxSnd, L".\\res\\sound\\mouseover.wav");//菜单
	mouseDownSound->LoadWave(dxSnd, L".\\res\\sound\\mousedown.wav");//菜单按下声音
	explosionSound->LoadWave(dxSnd, L".\\res\\sound\\enemy1_down.wav");//爆炸声音1
	gameOverSound->LoadWave(dxSnd, L".\\res\\sound\\game_over.wav");//gameover
	powerupSound1->LoadWave(dxSnd, L".\\res\\sound\\get_bomb.wav");//得到奖励1
	powerupSound2->LoadWave(dxSnd, L".\\res\\sound\\get_double_laser.wav");//得到奖励2
	gameWinSound->LoadWave(dxSnd, L".\\res\\sound\\achievement.wav");//Win
}

// 管理游戏中的各种声音播放
void FlyGame::PlayGameSound()
{
	bkgMusic->Terminate();			// 游戏背景音乐
	gameOverSound->Terminate();		// 游戏结束声音
	gameWinSound->Terminate();		// 游戏胜利声音

	switch(GameState)
	{
		case GAME_RUN:
			bkgMusic->Play(true);
			break;
		case GAME_WIN:
			bkgMusic->Stop();
			gameWinSound->Play();
			break;
		default:
			bkgMusic->Stop();
			break;
	}
}