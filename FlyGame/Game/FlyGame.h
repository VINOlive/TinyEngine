#include ".\\TinyEngine\\T_Map.h"
#include ".\\TinyEngine\\T_Audio.h"
#include ".\\TinyEngine\\T_Sprite.h" 
#include ".\\TinyEngine\\T_Scene.h"
#include ".\\TinyEngine\\T_AI.h"
#include ".\\TinyEngine\\T_Engine.h"
#include ".\\TinyEngine\\T_Menu.h"

// �������ڱ���ÿ�ؽ�ɫ��Դ��Ϣ�Ľṹ��
typedef struct
{
	LPCTSTR imgName;		// ͼ������
	int		frameWidth;		// ֡��
	int		frameHeight;	// ֡��
	int		Speed;			// ��ɫ��ʼ���ƶ��ٶ�
	int		Score;			// ��ɫ��ɫ��ֵ
	float	Ratio;			// ֡ͼƬ�Ŵ����С��
	int		Life;			// ��ɫ����ֵ
}NPCTYPEINFO;
// ����vector�������͵���������vSpriteSet
typedef vector<T_Sprite*> vSpriteSet;

class FlyGame: public T_Engine{

public:
	int wnd_width;									//��Ϸ���ڿ�
	int wnd_height;									//��Ϸ���ڸ�
	POINT mouse_pt;									//��ǰ���λ��

	static const int NPC_TYPE_NUM= 4;				//NPC��������
	static NPCTYPEINFO NpcSprites[NPC_TYPE_NUM+1];	//NCP������Ϣ

	//������������������������������������������������������������������������������
	// ����Ϸ�˵���صı���
	//������������������������������������������������������������������������������
	T_Menu* gameMenu;								//��Ϸ�˵�
	void LoadGameMenu(int type);					//���ز˵�
	void SetMenuPara(wstring* menuItems, int itemSize, int m_w, int m_h, int posType = 0);//���ò˵�����

	//������������������������������������������������������������������������������
	// ��������Դ��صı���
	//������������������������������������������������������������������������������
	AudioDX			dxSnd;							// DirectSound����
	AudioDXBuffer*	gameOverSound;					// ��Ϸ��������
	AudioDXBuffer*	gameWinSound;					// ��Ϸʤ������
	AudioDXBuffer*	bulletSound;					// �ӵ���������
	AudioDXBuffer*	bkgMusic;						// ��Ϸ��������
	AudioDXBuffer*  explosionSound;					// ��ը����1
	AudioDXBuffer*	mouseDownSound;					// ��갴������
	AudioDXBuffer*	mouseOverSound;					// ����Ƶ���ť������
	AudioDXBuffer*  powerupSound1;					// �õ�������Ʒ1������
	AudioDXBuffer*  powerupSound2;					// �õ�������Ʒ2������
	void LoadSound(HWND hwnd);						// ������Ƶ
	void PlayGameSound();							// ��������

	//������������������������������������������������������������������������������
	// ��DisplayINfo��صı���
	//������������������������������������������������������������������������������
	T_Graph* award1Img;								// ״̬������1�ĸ�������ȫ����ᣩ
	T_Graph* totalscoreBmp;							// ״̬������ܷ���СͼƬ
	void LoadImageRes();							// ����״̬��Ҫ��ʾ��ͼƬ

	//������������������������������������������������������������������������������
	// ����Ϸ������صı���
	//������������������������������������������������������������������������������
	T_Scene* t_scene;								// ��Ϸ����
	T_Sprite* player;								// ��Ϸ���
	T_Map* gameBkg;									// ��Ϸ����������ͼ��

	vSpriteSet npc_set;								// NPC��ɫ����
	vSpriteSet player_bomb_set;						// ����ڵ�����
	vSpriteSet award_set;							// ��������

	void LoadMap();                                 // ���ص�ͼ��������
	void LoadPlayer();                              // �������

	void LoadNpc(int num, int type);                // ����NPC ������npc_set���ϣ�
	void UpdateNpcPos();                            // ����NPCλ��

	void LoadBomb(T_Sprite* sp, vSpriteSet& bombSet, int iTime,int type=1); //�����ڵ�������player_bomb_set���ϣ�
	void UpdateBombPos(vSpriteSet* bombSet);        // �����ڵ�λ��
	void BombCollide(T_Sprite* bomb);               // �ڵ���ײ����

	void LoadAward(int type);						// ���ؽ���������award_set���ϣ�
	void UpdateAwardPos();							// ���½���λ��

	void UpdateAnimation();							// ���½�ɫ����֡���к�

	//������������������������������������������������������������������������������
	// ����Ϸ������صı���
	//������������������������������������������������������������������������������
	void GameLevelClear();							// ��������������һ����Դ
	void GameLevelRun();							// ���غ����³�ʼ����һ����Դ

	int time;										// ��֡������
	int count;										// ��Ϸ�ܻ���
	int level;										// ��Ϸ����
	int award1num;									// ��ǰ��ý���1�ĸ�����ȫ����ᣩ
	int award2time;									// ��ǰ��ý���2��ʣ��ʱ�䣨˫·�ӵ���
	void DisplayInfo(HDC hdc, int game_state);		// ��ʾ״̬����Ϣ
	void KillAllNpc();								// ȫ��NPC

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