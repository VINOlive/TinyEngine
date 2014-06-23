//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "T_AI.h"

int T_AI::EvadeDir[8][5]={
	{DIR_RIGHT, DIR_DOWN, DIR_UP, DIR_RIGHT_UP, DIR_RIGHT_DOWN},
	{DIR_LEFT, DIR_DOWN, DIR_UP, DIR_LEFT_UP, DIR_LEFT_DOWN},	
	{DIR_DOWN, DIR_LEFT, DIR_RIGHT, DIR_LEFT_DOWN, DIR_RIGHT_DOWN},	
	{DIR_UP, DIR_LEFT, DIR_RIGHT, DIR_LEFT_UP, DIR_RIGHT_UP},	
	{DIR_RIGHT_DOWN, DIR_RIGHT_UP, DIR_RIGHT, DIR_UP, DIR_DOWN},
	{DIR_RIGHT_UP, DIR_RIGHT_DOWN, DIR_RIGHT, DIR_UP, DIR_DOWN},
	{DIR_LEFT_DOWN, DIR_LEFT_UP, DIR_LEFT, DIR_UP, DIR_DOWN},
	{DIR_LEFT_UP, DIR_LEFT_DOWN, DIR_LEFT, DIR_UP, DIR_DOWN}		
};

T_AI::T_AI(int d_style)
{
	dir_style = d_style;
}

T_AI::~T_AI(void)
{
}

void T_AI::GetHitRect(IN T_Sprite* sp, IN int spSizeTimes, OUT RECT& testRec)
{
	int sp_width = sp->GetRatioSize().cx;
	int sp_height = sp->GetRatioSize().cy;

	switch(sp->GetDir())
	{
	case DIR_LEFT:
		testRec.left = sp->GetX()-spSizeTimes*sp_width;
		testRec.top = sp->GetY();
		testRec.right = sp->GetX();
		testRec.bottom = sp->GetY()+sp_height;;
		break;

	case DIR_RIGHT:
		testRec.left = sp->GetX()+sp_width;
		testRec.top = sp->GetY();
		testRec.right = sp->GetX()+(spSizeTimes+1)*sp_width;
		testRec.bottom = sp->GetY()+sp_height;;
		break;

	case DIR_UP:
		testRec.left = sp->GetX();
		testRec.top = sp->GetY()-spSizeTimes*sp_height;
		testRec.right = sp->GetX()+sp_width;
		testRec.bottom = sp->GetY();
		break;

	case DIR_DOWN:
		testRec.left = sp->GetX();
		testRec.top = sp->GetY()+sp_height;;
		testRec.right = sp->GetX()+sp_width;
		testRec.bottom = sp->GetY()+(spSizeTimes+1)*sp_height;
		break;

	case DIR_LEFT_UP:
		testRec.left = sp->GetX()-spSizeTimes*sp_width;
		testRec.top = sp->GetY()-spSizeTimes*sp_height;
		testRec.right = sp->GetX();
		testRec.bottom = sp->GetY();
		break;

	case DIR_LEFT_DOWN:
		testRec.left = sp->GetX()-spSizeTimes*sp_width;
		testRec.top = sp->GetY()+sp_height;;
		testRec.right = sp->GetX();
		testRec.bottom = sp->GetY()+(spSizeTimes+1)*sp_height;
		break;

	case DIR_RIGHT_UP:
		testRec.left = sp->GetX()+sp_width;
		testRec.top = sp->GetY()-spSizeTimes*sp_height;
		testRec.right = sp->GetX()+(spSizeTimes+1)*sp_width;
		testRec.bottom = sp->GetY();
		break;

	case DIR_RIGHT_DOWN:
		testRec.left = sp->GetX()+sp_width;
		testRec.top = sp->GetY()+sp_height;;
		testRec.right = sp->GetX()+(spSizeTimes+1)*sp_width;
		testRec.bottom = sp->GetY()+(spSizeTimes+1)*sp_height;
		break;
	}
}

MoveCoord T_AI::GetMoveCoord(T_Sprite* npc_sp, int dir, int speed, RECT boundary)
{
	MoveCoord mRate;
	int nextStepX, nextStepY;
	int SpeedX=0, SpeedY=0;
	mRate.BoundDir = -1;

	int sp_width = npc_sp->GetRatioSize().cx;
	int sp_height = npc_sp->GetRatioSize().cy;
 
	switch(dir)
	{
	case DIR_LEFT:
		SpeedX = -speed;
		SpeedY = 0;
		nextStepX = npc_sp->GetX()-speed;
		if(nextStepX <= boundary.left) 
		{
			SpeedX =  -(npc_sp->GetX()-boundary.left);
			mRate.BoundDir = DIR_LEFT;
		}
		break;

	case DIR_RIGHT:
		SpeedX =speed;
		SpeedY = 0;
		nextStepX = npc_sp->GetX()+ sp_width + speed;
		if(nextStepX >= boundary.right)
		{
			SpeedX = boundary.right - (npc_sp->GetX()+sp_width);
			mRate.BoundDir = DIR_RIGHT;
		}
		break;

	case DIR_UP:
		SpeedX = 0;
		SpeedY = -speed;
		nextStepY = npc_sp->GetY()-speed;
		if(nextStepY <= boundary.top) 
		{
			SpeedY =-(npc_sp->GetY()-boundary.top);
			mRate.BoundDir = DIR_UP;
		}
		break;

	case DIR_DOWN:
		SpeedX = 0;
		SpeedY = speed;
		nextStepY = npc_sp->GetY() + sp_height + speed;
		if(nextStepY >= boundary.bottom) 
		{
			SpeedY = boundary.bottom - (npc_sp->GetY()+sp_height);
			mRate.BoundDir = DIR_DOWN;
		}
		break;
	
	case DIR_LEFT_UP:
		SpeedX = -speed;
		SpeedY = -speed;
		nextStepX = npc_sp->GetX()-speed;
		if(nextStepX <= boundary.left) 
		{
			SpeedX =  -(npc_sp->GetX()-boundary.left);
			SpeedY = -(npc_sp->GetX()-boundary.left);
			mRate.BoundDir = DIR_LEFT;
			break;
		}
		nextStepY = npc_sp->GetY()-speed;
		if(nextStepY <= boundary.top) 
		{
			SpeedY =-(npc_sp->GetY()-boundary.top);
			SpeedX = -(npc_sp->GetY()-boundary.top);
			mRate.BoundDir = DIR_UP;
		}
		break;

	case DIR_LEFT_DOWN:
		SpeedX = -speed;
		SpeedY = speed;
		nextStepX = npc_sp->GetX()-speed;
		if(nextStepX <=boundary.left) 
		{
			SpeedX =  -(npc_sp->GetX()-boundary.left);
			SpeedY =  (npc_sp->GetX()-boundary.left);
			mRate.BoundDir = DIR_LEFT;
			break;
		}
		nextStepY = npc_sp->GetY() + sp_height + speed;
		if(nextStepY >=boundary.bottom) 
		{
			SpeedY = boundary.bottom - (npc_sp->GetY()+sp_height);
			SpeedX = -(boundary.bottom - (npc_sp->GetY()+sp_height));
			mRate.BoundDir = DIR_DOWN;
		}
		break;

	case DIR_RIGHT_UP:
		SpeedX = speed;
		SpeedY = -speed;
		nextStepX = npc_sp->GetX()+ sp_width + speed;
		if(nextStepX >= boundary.right) 
		{
			SpeedX = boundary.right - (npc_sp->GetX()+sp_width);
			SpeedY = -(boundary.right - (npc_sp->GetX()+sp_width));
			mRate.BoundDir = DIR_RIGHT;
			break;
		}
		nextStepY = npc_sp->GetY()-speed;
		if(nextStepY <= boundary.top) 
		{
			SpeedY =-( npc_sp->GetY()-boundary.top);
			SpeedX = npc_sp->GetY()-boundary.top;
			mRate.BoundDir = DIR_UP;
		}
		break;

	case DIR_RIGHT_DOWN:
		SpeedX = speed;
		SpeedY = speed;
		nextStepX = npc_sp->GetX()+ sp_width + speed;
		if(nextStepX >= boundary.right) 
		{
			SpeedX = boundary.right - (npc_sp->GetX()+sp_width);
			SpeedY = boundary.right - (npc_sp->GetX()+sp_width);
			mRate.BoundDir = DIR_RIGHT;
			break;
		}
		nextStepY = npc_sp->GetY() + sp_height + speed;
		if(nextStepY >=boundary.bottom) 
		{
			SpeedY = boundary.bottom - (npc_sp->GetY()+sp_height);
			SpeedX = boundary.bottom - (npc_sp->GetY()+sp_height);
			mRate.BoundDir = DIR_DOWN;
		}
	}

	mRate.moveX = SpeedX;
	mRate.moveY = SpeedY;
	mRate.oldX = npc_sp->GetX();
	mRate.oldY = npc_sp->GetY();
	return mRate;
}

void T_AI::Wander(T_Sprite* npc_sp, int npc_dir, int speed, RECT boundary)
{
	MoveCoord mRate = GetMoveCoord(npc_sp, npc_dir, speed, boundary);
	if(mRate.BoundDir == -1 )
	{
		npc_sp->Move(mRate.moveX, mRate.moveY);
	}
	if(mRate.BoundDir != -1) 
	{
		int r_dir = 0;
		if(dir_style == 8) 
		{
			r_dir = rand()%5;
		}
		else
		{
			r_dir = rand()%3;
		}
		npc_sp->SetPosition(mRate.oldX, mRate.oldY);
		npc_sp->SetDir(EvadeDir[mRate.BoundDir][r_dir]);
	}
}

void T_AI::Wander(T_Sprite* npc_sp, int npc_dir, int speed, T_Map* map)
{
	RECT mapBound;
	mapBound.left = map->GetX();
	mapBound.top = map->GetY();
	mapBound.right = map->GetX()+map->GetWidth();
	mapBound.bottom = map->GetY()+map->GetHeight();
	MoveCoord mRate = GetMoveCoord(npc_sp, npc_dir, speed, mapBound);
	int r_dir = 0;
	if(dir_style == 8) 
	{
		r_dir = rand()%5;
	}
	else
	{
		r_dir = rand()%3;
	}
	if(mRate.BoundDir == -1 )
	{
		npc_sp->Move(mRate.moveX, mRate.moveY);
	}
	if(mRate.BoundDir != -1) 
	{
		npc_sp->SetPosition(mRate.oldX, mRate.oldY);
		npc_sp->SetDir(EvadeDir[mRate.BoundDir][r_dir]);
	}
	if(npc_sp->CollideWith(map)) 
	{
		npc_sp->SetPosition(mRate.oldX, mRate.oldY);
		npc_sp->SetDir(EvadeDir[npc_sp->GetDir()][rand()%3]);
	}
}

void T_AI::Evade(T_Sprite* npc_sp, T_Sprite* player)
{
	RECT lprcDst;
	RECT playerRect={player->GetX(), player->GetY(), 
					 player->GetX()+player->GetRatioSize().cx, 
					 player->GetY()+player->GetRatioSize().cy };
	RECT testRect;
	GetHitRect(npc_sp, 1, testRect);
	if ((IntersectRect(&lprcDst, &playerRect, &testRect))==TRUE)
	{

		int r_dir = 0;
		if(dir_style == 8) 
		{
			r_dir = rand()%5;
		}
		else
		{
			r_dir = rand()%3;
		}
		npc_sp->SetDir(EvadeDir[npc_sp->GetDir()][r_dir]);
	}
}

void T_AI::CheckOverlay(T_Sprite* npc_sp, vector<T_Sprite*> vSpriteSet)
{
	vector<T_Sprite*>::iterator p;
	RECT testRec;
	GetHitRect(npc_sp, 1, testRec);
	int r_dir = 0;
	if(dir_style == 8) 
	{
		r_dir = rand()%5;
	}
	else
	{
		r_dir = rand()%3;
	}
	RECT lprcDst;
	for (p = vSpriteSet.begin(); p != vSpriteSet.end(); p++) 
	{
		if(*p == npc_sp) continue;
		
		RECT pRect={(*p)->GetX(), (*p)->GetY(), 
					(*p)->GetX()+(*p)->GetRatioSize().cx, 
					(*p)->GetY()+(*p)->GetRatioSize().cy };

		if ((IntersectRect(&lprcDst, &pRect, &testRec))==TRUE)
		{
			npc_sp->SetDir(EvadeDir[npc_sp->GetDir()][r_dir]);
		}
	}
}