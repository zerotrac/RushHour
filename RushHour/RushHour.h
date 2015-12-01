#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <math.h>
#include "resource.h"

/*定义宏变量，WNDWIDTH、WNDHEIGHT为窗口的宽和高*/
#define TIMER_ID             1
#define TIMER_ELAPSE         40
#define SCORE_ID             2
#define SCORE_ELAPSE         10000

#define WNDWIDTH             1280
#define WNDHEIGHT            640
#define BACKGROUND_COLOR_NUM 2
#define BLOCK_COLOR_NUM      4
#define ROOF_COLOR_NUM       2
#define MAX_TERRIAN_NUM      10
#define MAX_BACKGROUND_NUM   5
#define BUTTON_NUM           5
#define OTHERS_NUM           3

//定义资源的尺寸         
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

/*英雄结构体，英雄位图、位置、大小*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
	int     curFrameIndex;
	int     Status;
	int     maxFrameSize;
}Hero;

/*游戏背景结构体*/
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

/*背景建筑结构体，建筑位图、位置、大小、类型*/
typedef struct
{
	HBITMAP	hBmp;
	POINT	pos;
	SIZE	size;
}Building;

/*地形结构体，地形方块位图、屋顶位图、位置、大小、屋顶以及方块大小、方块个数*/
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

/*游戏状态结构体*/
typedef struct
{
	HBITMAP	hBmp;
	POINT   pos;
	SIZE	size;
	BOOL	isPaused;
	int     totalDist;
	int     totalCoin;
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

/*全局变量*/
static TCHAR szWindowClass[] = _T("win32app");
static TCHAR szTitle[] = _T("Jetpack Mouseride");

/*声明位图句柄*/

HBITMAP m_hBuildingBmp;
HBITMAP m_hHeroBmp;
HBITMAP m_hHeroUpBmp;
HBITMAP m_hHeroDownBmp;
HBITMAP m_hCoinBmp;
HBITMAP m_hGameStatusBmp;
HBITMAP m_hBackgroundBmp[BACKGROUND_COLOR_NUM];
HBITMAP	m_hBlockBmp[BLOCK_COLOR_NUM];
HBITMAP	m_hRoofkBmp[ROOF_COLOR_NUM];
HBITMAP m_hButtonBmp[BUTTON_NUM * 2];
HBITMAP m_hOthersBmp[OTHERS_NUM + 2];

/*定义方块颜色数组，与m_hBlockBmp[BLOCK_COLOR_NUM]个数对应，0表示蓝色方块，1表示绿色方块，2表示橙色方块，3表示粉色方块*/
int	m_blockBmpNames[] = {IDB_BLUE_BLOCK, IDB_GREEN_BLOCK, IDB_ORANGE_BLOCK, IDB_PINK_BLOCK};
/*定义屋顶颜色数组，与m_hRoofkBmp[ROOF_COLOR_NUM]个数对应，0表示黑色屋顶，1表示灰色屋顶*/
int	m_roofBmpNames[] = {IDB_BLACK_ROOF, IDB_GREY_ROOF};
/*定义背景数组，与m_hBackgroundBmp[BACKGROUND_COLOR_NUM]个数对应，0表示没有窗户，1表示有窗户*/
int m_backgroundBmpNames[] = {IDB_BG1, IDB_BG2};
int m_buttonBmpNames[] = {IDB_BUTTON11, IDB_BUTTON12,
						  IDB_BUTTON21, IDB_BUTTON22,
						  IDB_BUTTON31, IDB_BUTTON32,
						  IDB_BUTTON41, IDB_BUTTON42,
						  IDB_BUTTON51, IDB_BUTTON52};
int m_othersBmpNames[] = {IDB_BACKBOARD, IDB_JETPACK, IDB_MOUSEHOLE};

/*声明英雄、建筑、地形、游戏状态*/
Hero          m_hero;
Coin          m_coin;
Building      m_building;
Terrian       m_terrian[MAX_TERRIAN_NUM];
Background    m_background[MAX_BACKGROUND_NUM];
GameStatus    m_gameStatus;
Button        m_button[BUTTON_NUM * 2];
Others        m_others[OTHERS_NUM];
/*全局函数*/
//窗体过程函数
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//初始化
VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam);

/*************************************************
Fucntion : 创建英雄人物
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
	curFrameIndex当前帧
	maxFrameSize最大帧率
*************************************************/ 
Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int Status, int maxFrameSize);
Coin CreateCoin(LONG posX, LONG poxY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Background CreateBackground(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

Button CreateButton(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, BOOL active);
Others CreateOthers(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建背景建筑
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建游戏状态
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBmp表示位图句柄
*************************************************/ 
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp);

/*************************************************
Fucntion : 创建单个地形
Parameter:
	posX、posY表示矩形左上角的坐标
	sizeX、sizeY表示矩形的size
	hBlockBmp表示方块位图句柄
	hRoofBmp表示屋顶位图句柄
	roofHeight屋顶宽度和高度
	blockHeight方块宽度和高度
*************************************************/ 
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight);

//双缓冲绘制
VOID Render(HWND hWnd);
//定时器事件
VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam);
//hero更新
VOID HeroUpdate();
//背景更新
VOID BackgroundUpdate();
//地形更新
VOID TerrianUpdate();
//游戏状态更新
VOID GameStatusUpdate();
//判断是否点击暂停
VOID OthersUpdate();
VOID CoinUpdate();

BOOL Paused(POINT);
//键盘按下事件处理
VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
//键盘松开事件处理
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
//左鼠标点击事件
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
VOID LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
//鼠标移动事件
VOID MouseMove(HWND hWnd, WPARAM wParam, LPARAM lparam);