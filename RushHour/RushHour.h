#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include <Mmsystem.h>
#include <mmreg.h>
#include "resource.h"

#define TIMER_ID             1
#define TIMER_ELAPSE         40
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
#define HERO_TO_GROUND       128

#define HERO_MAX_FRAME       HERO_MAX_FRAME_NUM * HERO_MAX_FRAME_UP * HERO_MAX_FRAME_DOWN
#define LASER_MAX_FRAME      16
#define MAX_LASER_NUM        20

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

static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Jetpack Mouseride");

HBITMAP m_hBuildingBmp;
HBITMAP m_hHeroBmp, m_hHeroMistBmp, m_hHeroInvBmp;
HBITMAP m_hHeroDieBmp[2];
HBITMAP m_hCoinBmp;
HBITMAP m_hLuckyBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP m_hBackgroundBmp[BACKGROUND_COLOR_NUM];
HBITMAP m_hButtonBmp[BUTTON_NUM * 2];
HBITMAP m_hOthersBmp[OTHERS_NUM * 2];
HBITMAP m_hLaserBmp[6];
HBITMAP m_hMissileBmp[6];
HBITMAP m_hScoreboardBmp;
HBITMAP m_hShieldBmp, m_hShieldInvBmp;
HBITMAP m_hLifeBmp;
HBITMAP m_hNumberBmp[10];
HBITMAP m_hSmallNumberBmp[10];

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

Hero          m_hero;
Coin          m_coin;
Background    m_background[MAX_BACKGROUND_NUM];
GameStatus    m_gameStatus;
Button        m_button[BUTTON_NUM * 2];
Others        m_others[OTHERS_NUM * 2];
Laser         m_laser[MAX_LASER_NUM * 2];
Missile       m_missile;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int Status, int maxFrameSize);
Coin CreateCoin(LONG posX, LONG poxY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
Laser CreateLaser(LONG posX, LONG posY, LONG sizeXs, LONG sizeYs, LONG sizeXm, LONG sizeYm, int length);
Background CreateBackground(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
Button CreateButton(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, BOOL active);
Others CreateOthers(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);
Missile CreateMissile(LONG posX, LONG posY, LONG sizeX, LONG sizeY, BOOL active, int countdown);

VOID Render(HWND hWnd);
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID HeroUpdate();
VOID BackgroundUpdate();
VOID TerrianUpdate();
VOID GameStatusUpdate();
VOID OthersUpdate();
VOID CoinUpdate();
VOID LaserUpdate();
VOID MissileUpdate();

BOOL Paused(POINT);

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID KeyDown2(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID KeyDown3(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID MouseMove(HWND hWnd, WPARAM wParam, LPARAM lparam);