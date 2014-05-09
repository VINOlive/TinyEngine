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
#include "T_Sprite.h"

struct MoveCoord
{
	int oldX;
	int oldY;
	int moveX;
	int moveY;
	int BoundDir;
};

class T_AI
{
private:
	int dir_style;
	static int EvadeDir[8][5];

public:
	T_AI(int d_style);
	virtual ~T_AI(void);

	void GetHitRect(IN T_Sprite* sp, IN int spSizeTimes, OUT RECT& testRec);
	MoveCoord GetMoveCoord(T_Sprite* npc_sp, int dir, int speed, RECT boundary);
	void Wander(T_Sprite* npc_sp, int npc_dir, int speed, RECT boundary);
	void Evade(T_Sprite* npc_sp, T_Sprite* player);
	void CheckOverlay(T_Sprite* npc_sp, vector<T_Sprite*> vSpriteSet);
};
