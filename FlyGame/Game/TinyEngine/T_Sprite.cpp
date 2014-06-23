//*******************************************************************
// TinyEngine引擎  
// 作者: 万立中(WanLizhong)
// 博客: www.wanlizhong.com 
// 日期: 2013-08-02
// 版权所有 2007-2013 万立中
// (C) 2007-2013 WanLizhong All Rights Reserved
//*******************************************************************

#include "T_Sprite.h"

T_Sprite::T_Sprite(LPCTSTR imgPath, int frameWidth, int frameHeight)
{
	if(wcslen(imgPath)>0)
	{
		spImg.LoadImageFile(imgPath);
		spOldImg = spImg;
	}
	else
	{
		throw L"图像路径为空!";
	}
	if(frameWidth==0 && frameHeight==0)
	{
		SetWidth(spImg.GetImageWidth());
		SetHeight(spImg.GetImageHeight());

		totalFrames = rawFrames = 0;
	}
	else
	{
		SetWidth(frameWidth);
		SetHeight(frameHeight);
		frameCols = spImg.GetImageWidth()/frameWidth;
		frameRows = spImg.GetImageHeight()/frameHeight;
		totalFrames = frameCols*frameRows;
		rawFrames = frameCols*frameRows;
		forward = 0;
		backward = totalFrames-1;
	}
	frameSequence = NULL;
	loopForward = true;
	mapBlockPT.x = -1;
	mapBlockPT.y = -1;
}

T_Sprite::~T_Sprite(void)
{
}

void T_Sprite::Initiate(SPRITEINFO spInfo)
{								
	SetPosition(spInfo.X, spInfo.Y);
	Visible = spInfo.Visible;
	dir = spInfo.Dir;
	active = spInfo.Active;
	speed = spInfo.Speed;
	dead = spInfo.Dead;
	level = spInfo.Level;
	score = spInfo.Score; 
	frameRatio = spInfo.Ratio;
	frameRotate = spInfo.Rotation;
	frameAlpha = spInfo.Alpha;
	life=spInfo.Life;
	if(frameRatio>0) 
	{
		colideWidth = GetRatioSize().cx;
		colideHeight = GetRatioSize().cy;
	}
	else
	{
		colideWidth = (int)GetWidth();
		colideHeight =  (int)GetHeight();
	}
}

void T_Sprite::LoopFrame(bool ahead)
{	
	loopForward = ahead;
	if(totalFrames>0)
	{
		if(ahead == true)
		{
			forward = forward + 1;
			if(forward > totalFrames-1) 
			{
				forward = 0; 
			}
		}
		else
		{
			backward = backward - 1;
			if(backward < 0) 
			{
				backward = totalFrames-1;
			}
		}
	}
}

bool T_Sprite::LoopFrameOnce(bool ahead)
{
	loopForward = ahead;
	if(totalFrames<=0) return true;
	if(totalFrames>0)
	{
		if(ahead == true)
		{
			forward = forward + 1;
			if(forward > totalFrames-1) 
			{
				forward = 0; 
				return true;
			}
			else
			{
				return false;
			}
		}
		if(ahead == false)
		{
			backward = backward - 1;
			if(backward < 0) 
			{
				backward = totalFrames-1;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

void T_Sprite::Draw(HDC hdc)
{
	int frmIndex = 0;
	if(frameSequence != NULL && backward>=0 && forward>=0)
	{
		if(loopForward == false)
			frmIndex = frameSequence[backward];
		else
			frmIndex = frameSequence[forward];
	}
	else
	{
		if(loopForward == false)
			frmIndex = backward;
		else
			frmIndex = forward;
	}
	if(&spImg != NULL)
	{
		if(Visible == true)
		{
			if(totalFrames==0)
			{
				spImg.PaintRegion(
					spImg.GetBmpHandle(), hdc, (int)X, (int)Y, 
					0, 0, Width, Height, frameRatio, frameRotate, frameAlpha
				);
			}
			else
			{
				spImg.PaintFrame(
					spImg.GetBmpHandle(), hdc, (int)X, (int)Y, frmIndex,  
					frameCols, Width, Height, frameRatio, frameRotate, frameAlpha
				);
			}
		}
	}
}

bool T_Sprite::MoveTo(IN POINT mousePT, IN POINT desPT, IN RECT Boundary)
{
	int  xRatio, yRatio; 
	if(active == true)
	{
		SIZE ratioFrameSize = GetRatioSize();
		RECT HotRect;
		HotRect.left = (long)(desPT.x-5);
		HotRect.top = (long)(desPT.y-5);
		HotRect.right = (long)(desPT.x+5);
		HotRect.bottom = (long)(desPT.y+5);
		T_Util::GetBevelSpeed(desPT, mousePT, speed, xRatio, yRatio);
		BOOL ToDesPos = PtInRect(&HotRect, mousePT);
		if(ToDesPos == TRUE)
		{
			return true;
		}
		if(ToDesPos == FALSE)
		{		
			int nextStepX = (int)xRatio;
			int nextStepY = (int)yRatio;
			POINT nextPT = {(long)(GetX()+nextStepX), (long)(GetY()+nextStepY)};
			if(nextPT.x <=Boundary.left) 
				nextStepX = Boundary.left - (int)GetX();
			if((nextPT.x+ratioFrameSize.cx) >=Boundary.right) 
				nextStepX = Boundary.right - ratioFrameSize.cx - (int)GetX();
			if(nextPT.y <=Boundary.top) 
				nextStepY = Boundary.top - (int)GetY();
			if((nextPT.y+ratioFrameSize.cy) >=Boundary.bottom) 
				nextStepY = Boundary.bottom - ratioFrameSize.cy - (int)GetY();
			Move(nextStepX, nextStepY);	
			return false;
		}
	}
	return true;
}

bool T_Sprite::MoveTo(IN POINT mousePT, IN POINT desPT, IN T_Map* map)
{
	int  xRatio, yRatio; 
	if(active == true)
	{
		SIZE ratioFrameSize = GetRatioSize();
		RECT HotRect;
		HotRect.left = (long)(desPT.x-5);
		HotRect.top = (long)(desPT.y-5);
		HotRect.right = (long)(desPT.x+5);
		HotRect.bottom = (long)(desPT.y+5);
		T_Util::GetBevelSpeed(desPT, mousePT, speed, xRatio, yRatio);
		BOOL ToDesPos = PtInRect(&HotRect, mousePT);
		if(ToDesPos == TRUE)
		{
			return true;
		}
		if(ToDesPos == FALSE)
		{		
			int nextStepX = (int)xRatio;
			int nextStepY = (int)yRatio;
			int x =0;
			int y = 0;
			if(!CollideWith(map)) 
			{
				x = GetX();
				y = GetY();
				Move(nextStepX, nextStepY);	
			}
			if(CollideWith(map))
			{
				SetPosition(x, y);
			}
			return false;
		}
	}
	return true;
}

int T_Sprite::GetDir(POINT mousePT)
{
	int dir = -1;
	POINT spNowXY = {(long)GetX(),(long)GetY()};
	SIZE ratioFrameSize = GetRatioSize();
	RECT SpriteRect;
	SpriteRect.left = (long)GetX();
	SpriteRect.top = (long)GetY();
	SpriteRect.right = (long)(GetX() + ratioFrameSize.cx);
	SpriteRect.bottom = (long)(GetY() + ratioFrameSize.cy);
	if( mousePT.x <= SpriteRect.left && 
		mousePT.y >= SpriteRect.top && 
		mousePT.y <= SpriteRect.bottom)
	{
		dir = DIR_LEFT;
	}
	else if(mousePT.x >= SpriteRect.right && 
		mousePT.y >= SpriteRect.top && mousePT.y <= SpriteRect.bottom)
	{
		dir = DIR_RIGHT;
	}
	else if(mousePT.y <= (SpriteRect.bottom-ratioFrameSize.cy/2) &&
		mousePT.x > SpriteRect.left && mousePT.x < SpriteRect.right)
	{
		dir = DIR_UP;
	}
	else if(mousePT.y > (SpriteRect.bottom-ratioFrameSize.cy/2) &&
		mousePT.x > SpriteRect.left && mousePT.x < SpriteRect.right)
	{
		dir = DIR_DOWN;
	}
	else if(mousePT.x < SpriteRect.left && mousePT.y < SpriteRect.top)
	{
		dir = DIR_LEFT_UP;
	}
	else if(mousePT.x < SpriteRect.left && mousePT.y > SpriteRect.bottom)
	{
		dir = DIR_LEFT_DOWN;
	}
	else if(mousePT.x > SpriteRect.right && mousePT.y < SpriteRect.top)
	{
		dir = DIR_RIGHT_UP;
	}
	else if(mousePT.x > SpriteRect.right && mousePT.y > SpriteRect.bottom)
	{
		dir = DIR_RIGHT_DOWN;
	}
	else
	{
		dir = -1;			
	}
	return dir;
}

bool T_Sprite::CollideWith(IN T_Map* map)
{
	if(map->getMapRows() == 0 && map->getMapCols() == 0)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}
	if ((!(map->IsVisible())) || (!(this->IsVisible()))) 
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}
	int mapLeft = map->GetX();
	int mapTop =  map->GetY();
	int mapRight = mapLeft+map->GetWidth();
	int mapBottom = mapTop+map->GetHeight();
	int tW = map->getTileWidth();
	int tH = map->getTileHeight();
	int spLeft = this->GetCollideRect()->left;
	int spTop = this->GetCollideRect()->top;
	int spRight =this->GetCollideRect()->right;
	int spBottom = this->GetCollideRect()->bottom;
	int tNumCols = map->getMapCols();
	int tNumRows = map->getMapRows();
	int spW = spRight-spLeft;
	int spH = spBottom-spTop;
	RECT lprcDst;
	RECT mapRect={mapLeft, mapTop, mapRight, mapBottom};
	RECT spRect={spLeft, spTop, spRight, spBottom};
	if ((IntersectRect(&lprcDst, &mapRect, &spRect))==FALSE)
	{
		mapBlockPT.x = -1;
		mapBlockPT.y = -1;
		return false;
	}
	int startRow = (spTop <= mapTop) ? 0 : (spTop - mapTop) / tH;
	int endRow = (spBottom < mapBottom) ? (spBottom - 1 - mapTop) / tH : tNumRows - 1;
	int startCol = (spLeft <= mapLeft) ? 0 : (spLeft - mapLeft) / tW;
	int endCol = (spRight < mapRight) ? (spRight - 1 - mapLeft) / tW : tNumCols - 1;
	for (int row = startRow; row <= endRow; ++row) 
	{
		for (int col = startCol; col <= endCol; ++col) 
		{
			if (map->getTile(col, row) != 0) 
			{
				mapBlockPT.x = col;
				mapBlockPT.y = row;
				int x=GetX(), y = GetY();
				switch(GetDir())
				{
				case DIR_LEFT:
					x = GetX() + GetSpeed();
					y = GetY();
					break;
				case DIR_RIGHT:
					x = GetX() - GetSpeed();
					y = GetY();
					break;
				case DIR_UP:
					x = GetX();
					y = GetY() + GetSpeed();
					break;
				case DIR_DOWN:
					x = GetX();
					y = GetY() - GetSpeed();
					break;
				}
				SetPosition(x, y);
				return true;
			}
		}
	}
	return false;
}