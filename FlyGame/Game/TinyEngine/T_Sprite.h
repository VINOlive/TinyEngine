//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#pragma once
#include "T_Graph.h"
#include "T_Map.h"
#include "T_Layer.h"
#include "T_Util.h"

#define ROUND(x) (int)(x+0.5)

class T_Sprite:public T_Layer
{
protected:	
	T_Graph	spImg;
	T_Graph	spOldImg;
	int		life;
	int		frameCols;
	int		frameRows;
	int		rawFrames;
	int		totalFrames;
	int		forward;
	int		backward;
	bool	loopForward;
	int*	frameSequence;
	float	frameRatio;
	int		frameRotate;
	BYTE	frameAlpha;
	int		dir;
	int		lastDir;
	bool	active;
	bool	dead;
	int		speed;
	int		level;
	int		score;
	DWORD	startTime;
	DWORD	endTime;
	RECT	colideRect;
	int		colideWidth;
	int		colideHeight;
	POINT	mapBlockPT;

public:
	T_Sprite(LPCTSTR imgPath, int frameWidth = 0, int frameHeight = 0);
	virtual ~T_Sprite(void);
	virtual string ClassName(){ return "T_Sprite"; }	

	T_Graph* GetImage(){ return &spImg; }
	void SetImage(T_Graph* pImg){ spImg = *pImg; }
	void ResetImage(){ spImg = spOldImg; }
	float GetRatio(){ return frameRatio; }
	void SetRatio(float rat){ frameRatio = rat; }

	SIZE GetRatioSize()									
	{ 
		SIZE ratioFrameSize;
		ratioFrameSize.cx = ROUND(frameRatio*Width);
		ratioFrameSize.cy = ROUND(frameRatio*Height); 
		return ratioFrameSize;
	}

	int GetRotation(){ return frameRotate; }
	void SetRotation(int rot){ frameRotate = rot; }

	BYTE GetAlpha(){ return frameAlpha; }
	void SetAlpha(BYTE a){ frameAlpha = a; }

	int GetDir(){ return dir; }			
	void SetDir(int d){ dir = d; }
	bool IsActive(){ return active; }
	void SetActive(bool act){ active = act; }

	bool IsDead(){ return dead; }
	void SetDead(bool dd){ dead = dd; }

	int GetLife(){return life;}
	void SetLife(int li){life =li;}

	int GetSpeed(){ return speed; }
	void SetSpeed(int spd){ speed = spd; }

	int GetLevel(){ return level; }
	void SetLevel(int lvl){ level = lvl; }

	int GetScore(){ return score; }
	void SetScore(int scr){ score = scr; }

	void SetEndTime(DWORD time){ endTime = time; }
	DWORD GetEndTime(){ return endTime; }
	void SetStartTime(DWORD time){ startTime = time; }
	DWORD GetStartTime(){ return startTime; }

	void AdjustCollideRect(int px=0, int py=0);	
	RECT* GetCollideRect();	
	bool CollideWith(T_Sprite* target, int distance=0);
	bool CollideWith(IN T_Map* map);	
	POINT GetMapBlockPT(){ return mapBlockPT; }				

	void LoopFrame(bool ahead = true);
	bool LoopFrameOnce(bool ahead = true);				
	int GetRawFrames(){ return rawFrames; }	
	int GetTotalFrames(){ return totalFrames; }			

	int GetFrame(bool ahead = true)						
	{ 
		if(ahead == true)
			return forward;
		else
			return backward;
	}

	void SetFrame(int sequenceIndex, bool ahead = true)	
	{ 
		if(ahead == true)
			forward = sequenceIndex;
		else
			backward = sequenceIndex;
	}

	void SetSequence(int* sequence, int length)			
	{ 
		frameSequence = sequence; 
		totalFrames = length;
	}

	int GetDir(POINT mousePT);							
	bool MoveTo(IN POINT mousePT, IN POINT desPT, IN RECT Boundary);
	bool MoveTo(IN POINT mousePT, IN POINT desPT, IN T_Map* map);	

	void Initiate(SPRITEINFO spInfo);
	void Draw(HDC hdc);	
};

inline void T_Sprite::AdjustCollideRect(int px, int py)
{
	if(px == 0 && py == 0)  
	{
		return;
	}
	else
	{
		RECT tempRec={0, 0, colideWidth, colideHeight};
		InflateRect(&tempRec, px, py);
		colideWidth = tempRec.right-tempRec.left;
		colideHeight = tempRec.bottom - tempRec.top;
	}
} 

inline RECT* T_Sprite::GetCollideRect()
{
	int c_left, c_top;
	if(frameRatio>0) 
	{
		c_left = (GetRatioSize().cx-colideWidth)/2;
		c_top = (GetRatioSize().cy-colideHeight)/2;
	}
	else
	{
		c_left = ((int)GetWidth()-colideWidth)/2;
		c_top = ((int)GetHeight()-colideHeight)/2;
	}

	colideRect.left = (LONG)X+c_left;
	colideRect.right = colideRect.left + colideWidth;
	colideRect.top = (LONG)Y+c_top;
	colideRect.bottom = colideRect.top + colideHeight;
	return &colideRect;
}

inline bool T_Sprite::CollideWith(T_Sprite* target, int distance)
{
	RECT targetRect = *(target->GetCollideRect());
	RECT hitRec;
	hitRec.left =targetRect.left-distance;
	hitRec.top = targetRect.top-distance;
	hitRec.right = targetRect.right+distance;
	hitRec.bottom = targetRect.bottom+distance;
	RECT thisRect = *(this->GetCollideRect());
	int cw = abs(thisRect.right - thisRect.left);
	int tw = abs(hitRec.right - hitRec.left);
	int ch = abs(thisRect.bottom - thisRect.top);
	int th = abs(hitRec.bottom - hitRec.top);

	return thisRect.left <= hitRec.right &&
		   hitRec.left <= thisRect.right &&
		   thisRect.top <= hitRec.bottom &&
		   hitRec.top <= thisRect.bottom;
}