#include ".\\TinyEngine\\T_Map.h"
#include ".\\TinyEngine\\T_Audio.h"
#include ".\\TinyEngine\\T_Sprite.h" 
#include ".\\TinyEngine\\T_Scene.h"
#include ".\\TinyEngine\\T_AI.h"
#include ".\\TinyEngine\\T_Engine.h"
#include ".\\TinyEngine\\T_Menu.h"

// 定义用于保存每关角色资源信息的结构体
typedef struct
{
	LPCTSTR imgName;		// 图像名称
	int		frameWidth;		// 帧宽
	int		frameHeight;	// 帧高
	int		Speed;			// 角色初始的移动速度
	int		Score;			// 角色角色分值
	float	Ratio;			// 帧图片放大或缩小比
	int		Life;			// 角色生命值
}NPCTYPEINFO;
// 定义vector容器类型的数据类型vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

class FlyGame: public T_Engine{

public:
	int wnd_width;									//游戏窗口宽
	int wnd_height;									//游戏窗口高
	POINT mouse_pt;									//当前鼠标位置

	static const int NPC_TYPE_NUM= 4;				//NPC种类数量
	static NPCTYPEINFO NpcSprites[NPC_TYPE_NUM+1];	//NCP种类信息

	//………………………………………………………………………………………………………
	// 与游戏菜单相关的变量
	//………………………………………………………………………………………………………
	T_Menu* gameMenu;								//游戏菜单
	void LoadGameMenu(int type);					//加载菜单
	void SetMenuPara(wstring* menuItems, int itemSize, int m_w, int m_h, int posType = 0);//设置菜单参数

	//………………………………………………………………………………………………………
	// 与声音资源相关的变量
	//………………………………………………………………………………………………………
	AudioDX			dxSnd;							// DirectSound对象
	AudioDXBuffer*	gameOverSound;					// 游戏结束声音
	AudioDXBuffer*	gameWinSound;					// 游戏胜利声音
	AudioDXBuffer*	bulletSound;					// 子弹发射声音
	AudioDXBuffer*	bkgMusic;						// 游戏背景音乐
	AudioDXBuffer*  explosionSound;					// 爆炸声音1
	AudioDXBuffer*	mouseDownSound;					// 鼠标按下声音
	AudioDXBuffer*	mouseOverSound;					// 鼠标移到按钮的声音
	AudioDXBuffer*  powerupSound1;					// 得到奖励物品1的声音
	AudioDXBuffer*  powerupSound2;					// 得到奖励物品2的声音
	void LoadSound(HWND hwnd);						// 加载音频
	void PlayGameSound();							// 播放声音

	//………………………………………………………………………………………………………
	// 与DisplayINfo相关的变量
	//………………………………………………………………………………………………………
	T_Graph* award1Img;								// 状态栏奖励1的个数（即全灭机会）
	T_Graph* totalscoreBmp;							// 状态栏玩家总分数小图片
	void LoadImageRes();							// 加载状态栏要显示的图片

	//………………………………………………………………………………………………………
	// 与游戏场景相关的变量
	//………………………………………………………………………………………………………
	T_Scene* t_scene;								// 游戏场景
	T_Sprite* player;								// 游戏玩家
	T_Map* gameBkg;									// 游戏背景（即地图）

	vSpriteSet npc_set;								// NPC角色集合
	vSpriteSet player_bomb_set;						// 玩家炮弹集合
	vSpriteSet award_set;							// 奖励集合

	void LoadMap();                                 // 加载地图（背景）
	void LoadPlayer();                              // 加载玩家

	void LoadNpc(int num, int type);                // 加载NPC （操作npc_set集合）
	void UpdateNpcPos();                            // 更新NPC位置

	void LoadBomb(T_Sprite* sp, vSpriteSet& bombSet, int iTime,int type=1); //加载炮弹（操作player_bomb_set集合）
	void UpdateBombPos(vSpriteSet* bombSet);        // 更新炮弹位置
	void BombCollide(T_Sprite* bomb);               // 炮弹碰撞处理

	void LoadAward(int type);						// 加载奖励（操作award_set集合）
	void UpdateAwardPos();							// 更新奖励位置

	void UpdateAnimation();							// 更新角色动画帧序列号

	//………………………………………………………………………………………………………
	// 与游戏其他相关的变量
	//………………………………………………………………………………………………………
	void GameLevelClear();							// 过关升级清理上一个资源
	void GameLevelRun();							// 过关后重新初始化新一关资源

	int time;										// 按帧数计数
	int count;										// 游戏总积分
	int level;										// 游戏关数
	int award1num;									// 当前获得奖励1的个数（全灭机会）
	int award2time;									// 当前获得奖励2的剩余时间（双路子弹）
	void DisplayInfo(HDC hdc, int game_state);		// 显示状态栏信息
	void KillAllNpc();								// 全灭NPC

	virtual ~FlyGame(void);
	FlyGame(HINSTANCE h_instance, LPCTSTR sz_winclass, LPCTSTR sz_title, 
		WORD icon = NULL, WORD sm_icon = NULL, 
		int winwidth = 600, int winheight = 800);

	void GameInit();		
	void GameLogic();		
	void GameEnd();
	void GamePaint(HDC hdc);
	void GameKeyAction(int ActionType=KEY_SYS_NONE);
	void GameMouseAction(int x, int y, int ActionType);	
};