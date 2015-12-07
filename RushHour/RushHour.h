#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include "resource.h"

/*����������WNDWIDTH��WNDHEIGHTΪ���ڵĿ�͸�*/
#define TIMER_ID             1
#define TIMER_ELAPSE         40
//#define SCORE_ID             2
//#define SCORE_ELAPSE         15000
//#define GRAVITY_ID           3
//#define GRAVITY_ELAPSE       30000
#define ENDING_ID            3
#define ENDING_ELAPSE        300

#define WNDWIDTH             1280
#define WNDHEIGHT            640
#define BACKGROUND_COLOR_NUM 2
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      10
#define MAX_BACKGROUND_NUM   5
#define BUTTON_NUM           5
#define OTHERS_NUM           3

//������Դ�ĳߴ�         
#define HERO_SIZE_X          54
#define HERO_SIZE_Y          62
#define HERO_MAX_FRAME_NUM   4
#define HERO_MAX_FRAME_UP    7
#define HERO_MAX_FRAME_DOWN  1
#define COIN_SIZE            30
#define BACKGROUND_SIZE_X    480
#define BACKGROUND_SIZE_Y    640
#define BUILDING_SIZE_X      500
#define BUILDING_SIZE_Y      250
#define ROOF_SIZE_X          65
#define ROOF_SIZE_Y          33
#define BLOCK_SIZE_X         65
#define BLOCK_SIZE_Y         65
#define GAME_STATUS_SIZE_X   40
#define GAME_STATUS_SIZE_Y   30
#define BUTTON_SIZE_X        406
#define BUTTON_SIZE_Y        119
#define HERO_TO_GROUND       120

#define HERO_MAX_FRAME       HERO_MAX_FRAME_NUM * HERO_MAX_FRAME_UP * HERO_MAX_FRAME_DOWN
#define LASER_MAX_FRAME      16
#define MAX_LASER_NUM        20

/*Ӣ�۽ṹ�壬Ӣ��λͼ��λ�á���С*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     Status;
	int     maxFrameSize;
	int     invincible;
	int     life;
	BOOL    alive;
}Hero;
/*��Ϸ�����ṹ��*/
typedef struct
{
	HBITMAP hBmp;
	POINT   pos;
	SIZE    size;
}Background;
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
}Coin;
typedef struct
{
	POINT pos;
	SIZE sizeside;
	SIZE sizemid;
	int length;
	BOOL used;
	BOOL active;
	BOOL blink;
}Laser;
/*���������ṹ�壬����λͼ��λ�á���С������*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
}Building;
/*���νṹ�壬���η���λͼ���ݶ�λͼ��λ�á���С���ݶ��Լ������С���������*/
typedef struct
{
	HBITMAP hBlockBmp;
	HBITMAP hRoofBmp;
	POINT	pos;
	SIZE    size;
	int     roofWidth;
	int     roofHeight;
	int     blockWidth;
	int     blockHeight;
	int     blockNum;
}Terrian;
/*��Ϸ״̬�ṹ��*/
typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isPaused;
	double  totalDist;
	int     totalCoin;
	int     currentCoin;
}GameStatus;
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
	BOOL active;
}Button;
typedef struct
{
	HBITMAP hBmp;
	POINT pos;
	SIZE size;
}Others;
typedef struct
{
	POINT pos;
	SIZE size;
	INT countdown;
	BOOL active;
}Missile;
/*ȫ�ֱ���*/
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Jetpack Mouseride");

/*����λͼ���*/

HBITMAP m_hBuildingBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hHeroUpBmp;
HBITMAP m_hHeroDownBmp;
HBITMAP m_hHeroDieBmp[2];
HBITMAP m_hCoinBmp;
HBITMAP m_hLuckyBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP m_hBackgroundBmp[BACKGROUND_COLOR_NUM];
HBITMAP	m_hBlockBmp[BLOCK_COLOR_NUM];
HBITMAP	m_hRoofkBmp[ROOF_COLOR_NUM];
HBITMAP m_hButtonBmp[BUTTON_NUM * 2];
HBITMAP m_hOthersBmp[OTHERS_NUM + 2];
HBITMAP m_hLaserBmp[3];
HBITMAP m_hMissileBmp[3];
HBITMAP m_hScoreboardBmp;
HBITMAP m_hShieldBmp;
HBITMAP m_hLifeBmp;
HBITMAP m_hNumberBmp[10];
HBITMAP m_hSmallNumberBmp[10];

/*���巽����ɫ���飬��m_hBlockBmp[BLOCK_COLOR_NUM]������Ӧ��0��ʾ��ɫ���飬1��ʾ��ɫ���飬2��ʾ��ɫ���飬3��ʾ��ɫ����*/
int	m_blockBmpNames[] = {IDB_BLUE_BLOCK, IDB_GREEN_BLOCK, IDB_ORANGE_BLOCK, IDB_PINK_BLOCK};
/*�����ݶ���ɫ���飬��m_hRoofkBmp[ROOF_COLOR_NUM]������Ӧ��0��ʾ��ɫ�ݶ���1��ʾ��ɫ�ݶ�*/
int	m_roofBmpNames[] = {IDB_BLACK_ROOF, IDB_GREY_ROOF};
/*���屳�����飬��m_hBackgroundBmp[BACKGROUND_COLOR_NUM]������Ӧ��0��ʾû�д�����1��ʾ�д���*/
int m_backgroundBmpNames[] = {IDB_BG1, IDB_BG2};
int m_buttonBmpNames[] = {IDB_BUTTON11, IDB_BUTTON12,
						  IDB_BUTTON21, IDB_BUTTON22,
						  IDB_BUTTON31, IDB_BUTTON32,
						  IDB_BUTTON41, IDB_BUTTON42,
						  IDB_BUTTON51, IDB_BUTTON52};
int m_othersBmpNames[] = {IDB_BACKBOARD, IDB_JETPACK, IDB_MOUSEHOLE};
int m_laserBmpNames[] = {IDB_LASERU, IDB_LASERD, IDB_LASER};
int m_missileBmpNames[] = {IDB_MISSILE, IDB_WARNING, IDB_WARNINGG};
int m_herodieBmpNames[] = {IDB_HEROD0, IDB_HEROD1};
int m_numberBmpNames[] = {IDB_NUM0, IDB_NUM1, IDB_NUM2, IDB_NUM3, IDB_NUM4, IDB_NUM5, IDB_NUM6, IDB_NUM7, IDB_NUM8, IDB_NUM9};
int m_smallnumberBmpNames[] = {IDB_SNUM0, IDB_SNUM1, IDB_SNUM2, IDB_SNUM3, IDB_SNUM4, IDB_SNUM5, IDB_SNUM6, IDB_SNUM7, IDB_SNUM8, IDB_SNUM9};
/*����Ӣ�ۡ����������Ρ���Ϸ״̬*/
Hero          m_hero;
Coin          m_coin;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
Background    m_background[MAX_BACKGROUND_NUM];
GameStatus    m_gameStatus;
Button        m_button[BUTTON_NUM * 2];
Others        m_others[OTHERS_NUM];
Laser         m_laser[MAX_LASER_NUM];
Missile       m_missile;

/*ȫ�ֺ���*/
//������̺���
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//��ʼ��
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : ����Ӣ������
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
	curFrameIndex��ǰ֡
	maxFrameSize���֡��
*************************************************/ 
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int Status, int maxFrameSize);
Coin CreateCoin(LONG posX, LONG poxY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
Laser CreateLaser(LONG posX, LONG posY, LONG sizeXs, LONG sizeYs, LONG sizeXm, LONG sizeYm, int length);
Background CreateBackground(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
Button CreateButton(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, BOOL active);
Others CreateOthers(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
/*************************************************
Fucntion : ������������
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
*************************************************/ 
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
/*************************************************
Fucntion : ������Ϸ״̬
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBmp��ʾλͼ���
*************************************************/ 
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
/*************************************************
Fucntion : ������������
Parameter:
	posX��posY��ʾ�������Ͻǵ�����
	sizeX��sizeY��ʾ���ε�size
	hBlockBmp��ʾ����λͼ���
	hRoofBmp��ʾ�ݶ�λͼ���
	roofHeight�ݶ���Ⱥ͸߶�
	blockHeight�����Ⱥ͸߶�
*************************************************/ 
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight);
Missile CreateMissile(LONG posX, LONG posY, LONG sizeX, LONG sizeY, BOOL active, int countdown);

//˫�������
VOID Render(HWND hWnd);
//��ʱ���¼�
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//hero����
VOID HeroUpdate();
//��������
VOID BackgroundUpdate();
//���θ���
VOID TerrianUpdate();
//��Ϸ״̬����
VOID GameStatusUpdate();
//�ж��Ƿ�����ͣ
VOID OthersUpdate();
VOID CoinUpdate();
VOID LaserUpdate();
VOID MissileUpdate();
BOOL Paused(POINT);
//���̰����¼�����
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID KeyDown2(HWND hWnd, WPARAM wParam, LPARAM lParam);
//�����ɿ��¼�����
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
//��������¼�
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
//����ƶ��¼�
VOID MouseMove(HWND hWnd, WPARAM wParam, LPARAM lparam);