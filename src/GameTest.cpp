/* ========================================================== 
*	文 件 名：GameTest.cpp
*	当前版本：1.0.0
*	创建时间：2014-03-21 
*	修改时间：2014-03-21
*	功能说明：游戏引擎测试类 WinMain
*	编译环境：VS2008
*
*	班    级：东软实验111班
*	姓    名：方杰
*	学    号：8000611063
*	实验名称：
*	实验序号：实验3
*	实验地点：图书馆406
*	实验时间：2014-03-21 16:10-18:00
 ==========================================================*/
#include "GameTest.h"

FISHINFO GameTest::fish[7]={
	{L"blue.png",143,84},{L"green.png",143,84},
	{L"orange.png",143,84},{L"pink.png",143,84},
	{L"red.png",143,84},{L"yellow.png",143,84},
	{L"clown.png",78,40}
};
int GameTest::sequence[20]={0,0,1,1,1,1,3,3,3,3,5,5,5,5,7,7,7,7,0,0};

GameTest::GameTest(HINSTANCE hInstance, LPCTSTR szWindowClass, LPCTSTR szTitle,WORD Icon, WORD SmIcon, int iWidth, int iHeight):T_Engine(hInstance,szWindowClass,szTitle,Icon,SmIcon,iWidth,iHeight)
{
	wnd_width=iWidth;
	wnd_height=iHeight;
}
GameTest::~GameTest(){

}

void GameTest::LoadPlayer()
{
	int x=0;
	int y=0;
	player=new T_Sprite(L".\\res\\fish\\purplefish.png",176,111);
	SPRITEINFO spInfo;
	spInfo.Active=true;
	spInfo.Dead=false;
	spInfo.Alpha=255;
	spInfo.Dir=DIR_LEFT;
	spInfo.Level=0;
	spInfo.Rotation=TRANS_NONE;
	spInfo.Ratio=0.8f;
	spInfo.Score=0;
	spInfo.Speed=10;
	spInfo.Visible=true;
	player->Initiate(spInfo);
	x=(wnd_width-player->GetRatioSize().cx)/2;
	y=(wnd_height-player->GetRatioSize().cy)/2;
	player->SetPosition(x,y);
	player->SetSequence(sequence,20);
}

void GameTest::LoadNpc(int num)
{
	int x=0;
	int y=0;
	for(int i=0;i<num;i++)
	{
		int r=rand()%7;
		wstring path=L".\\res\\fish\\";
		path.append(fish[r].filename);
		npc[i]=new T_Sprite(path.c_str(),fish[r].wframe,fish[r].hframe);
		SPRITEINFO spInfo;
		spInfo.Active=true;
		spInfo.Dead=false;
		spInfo.Alpha=255;
		spInfo.Dir=DIR_LEFT;
		spInfo.Level=0;
		spInfo.Rotation=TRANS_NONE;
		spInfo.Ratio=1;
		spInfo.Score=0;
		spInfo.Speed=3;
		spInfo.Visible=true;
		npc[i]->Initiate(spInfo);
		x=rand()%(wnd_width-npc[i]->GetRatioSize().cx);
		y=rand()%(wnd_height-npc[i]->GetRatioSize().cy);
		npc[i]->SetPosition(x,y);
	}
}

void GameTest::UpdateFrames()
{
	player->LoopFrame();
	for(int i=0;i<NUM;i++)
	{
		npc[i]->LoopFrame();
	}
}

void GameTest::UpdatePos()
{
	if(player->IsActive()==true)
	{
		long centerX=player->GetX()+(player->GetRatioSize().cx/2);
		long centerY=player->GetY()+(player->GetRatioSize().cy/2);
		POINT centerPT={centerX,centerY};
		RECT WinRECT={0,0,wnd_width,wnd_height};
		player->MoveTo(mouse_pt,centerPT,WinRECT);
	}
	int npc_dir=0;
	for (int i=0;i<NUM;i++)
	{
		if(npc[i]->IsActive()==true&&npc[i]->IsVisible()==true)
		{
			npc_dir=npc[i]->GetDir();
			switch (npc_dir)
			{
				case DIR_LEFT:
				case DIR_LEFT_DOWN:
				case DIR_LEFT_UP:
					player->SetRotation(TRANS_NONE);
					break;
				case DIR_RIGHT:
				case DIR_RIGHT_DOWN:
				case DIR_RIGHT_UP:
					player->SetRotation(TRANS_HFLIP_NOROT);
					break;
				case DIR_UP:
				case DIR_DOWN:
					break;
			}
		}
		RECT npc_move_bound;
		npc_move_bound.left=-200;
		npc_move_bound.top=0;
		npc_move_bound.right=wnd_width+200;
		npc_move_bound.bottom=wnd_height-npc[i]->GetHeight();
		sp_ai->Wander(npc[i],npc_dir,npc[i]->GetSpeed(),npc_move_bound);
		sp_ai->Evade(npc[i],player);

	}
}


void GameTest::GameInit(){
	srand(GetTickCount());
	back=new T_Graph(L".\\res\\seafloor.jpg");
	sp_ai=new T_AI(8);
	LoadPlayer();
	LoadNpc(NUM);
	GameState=GAME_START;
}                        

void GameTest::GameLogic(){
	UpdateFrames();
	UpdatePos();
}

void GameTest::GameEnd(){
	delete player;
	player=NULL;
	delete back;
	back=NULL;
	delete sp_ai;
	for (int i=0;i<NUM;i++)
	{
		delete npc[i];
		npc[i]=NULL;
	}
}

void GameTest::GameKeyAction(int ActionType){

}
void GameTest::GameMouseAction(int x, int y, int ActionType){
	mouse_pt.x=x;
	mouse_pt.y=y;
	if(ActionType==MOUSE_MOVE)
	{
		int p_dir=player->GetDir(mouse_pt);
		player->SetDir(p_dir);
		switch (p_dir)
		{
			case DIR_LEFT:
			case DIR_LEFT_DOWN:
			case DIR_LEFT_UP:
				player->SetRotation(TRANS_NONE);
				break;
			case DIR_RIGHT:
			case DIR_RIGHT_DOWN:
			case DIR_RIGHT_UP:
				player->SetRotation(TRANS_HFLIP_NOROT);
				break;
			case DIR_UP:
			case DIR_DOWN:
				break;
		}
		player->SetActive(true);
	}
}

void GameTest::GamePaint(HDC hdc)
{
	back->PaintImage(hdc,0,0,wnd_width,wnd_height);
	for (int i=0;i<NUM;i++)
	{
		npc[i]->Draw(hdc);
	}
	player->Draw(hdc);

}

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd )
{
	LPCTSTR WinTitle=L"T_Engine";
	GameTest* test=new GameTest(hInstance,WIN_CLASS,WinTitle,NULL,NULL,800,600);
	T_Engine::pEngine=test;
	test->SetFrame(20);
	test->StartEngine();
	return TRUE;
}