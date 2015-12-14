#pragma once
#pragma comment(lib, "winmm.lib")

//#pragma comment(lib, "Gdi32.lib")
//#pragma comment(lib, "user32.lib")

#include "RushHour.h"
#include "Random.h"
#include "Jumping.h"

BOOL GameStart = FALSE;
BOOL LeftKeyDown = FALSE;
BOOL IsOnFire = FALSE;
BOOL ScoreBoard = FALSE;
BOOL AppStore = FALSE;
BOOL SelectMode = FALSE;
BOOL NextOperationValid = TRUE;
BOOL DoOperation = FALSE;
BOOL AntiGravity = FALSE;

BOOL LuckyCoin = FALSE;
BOOL LaserInterference = FALSE;
BOOL AntMan = FALSE;
BOOL SoulRing = FALSE;

int HeroVelocity = 0;
double ScorePerFrame = 4.0;
int CharacterCount = -1;
int ElapseCount = 0;
int GameMode = 1;
int MachineCount = 0;
int MachineCountdown = 0;
int MachineRecord = 0;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	srand(time(0));

	WNDCLASSEX wcex;
	HWND hWnd;
	MSG msg;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APP_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Call to RegisterClassEx failed!"),	_T("Mega Plane"), MB_OK);
		return 1;
	}

	// The parameters to CreateWindow explained:
	// szWindowClass: the name of the application
	// szTitle: the text that appears in the title bar
	// WS_OVERLAPPEDWINDOW: the type of window to create,~WS_THICKFRAME  fixed window size
	// CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	// WNDWIDTH, WNDHEIGHT: initial size (width, length)
	// NULL: the parent of this window
	// NULL: this application does not have a menu bar
	// hInstance: the first parameter from WinMain
	// NULL: not used in this application
	hWnd = CreateWindow(szWindowClass, szTitle,	WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
						CW_USEDEFAULT, CW_USEDEFAULT, WNDWIDTH, WNDHEIGHT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, _T("Call to CreateWindow failed!"), _T("Mega Plane"), MB_OK);
		return 1;
	}

	// The parameters to ShowWindow explained:
	// hWnd: the value returned from CreateWindow
	// nCmdShow: the fourth parameter from WinMain
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			//初始化
			Init(hWnd, wParam, lParam);
			break;
		case WM_PAINT:
			//绘制
			Render(hWnd);
			break;
		case WM_KEYDOWN:
			//键盘按下事件
			if (GameStart) KeyDown(hWnd, wParam, lParam);
			if (AppStore) KeyDown2(hWnd, wParam, lParam);
			if (SelectMode) KeyDown3(hWnd, wParam, lParam);
			break;
		case WM_KEYUP:
			//键盘松开事件
			if (GameStart && m_hero.alive) KeyUp(hWnd, wParam, lParam);
			break;
		case WM_MOUSEMOVE:
			//鼠标移动事件
			MouseMove(hWnd, wParam, lParam);
			break;
		case WM_LBUTTONDOWN:
			//左鼠标事件
			LeftKeyDown = TRUE;
			LButtonDown(hWnd, wParam, lParam);
			break;
		case WM_LBUTTONUP:
			LeftKeyDown = FALSE;
			LButtonUp(hWnd, wParam, lParam);
			break;
		case WM_TIMER:
			//定时器事件
			TimerUpdate(hWnd, wParam, lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
			break;
	}
	return 0;
}

VOID Init(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//加载Building位图
	m_hBuildingBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BUILDING));

	//加载英雄位图
	m_hHeroBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_HERO));
	m_hHeroMistBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_HEROMIST));
	m_hHeroInvBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_HEROINV));
	m_hShieldBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_SHIELD));
	m_hShieldInvBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_SHIELDINV));
	m_hCoinBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_COIN));
	m_coin = CreateCoin(100, 100, COIN_SIZE, COIN_SIZE, m_hCoinBmp);
	m_hLuckyBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_COINB));;
	m_hLifeBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_LIFE));
	m_hGameStatusBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_GAME_STATUS));
	m_hScoreboardBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_SCOREBOARD));
	
	for (int k = 0; k < 10; k++)
	{
		m_hNumberBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_numberBmpNames[k]));
		m_hSmallNumberBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_smallnumberBmpNames[k]));
	}
	for (int k = 0; k < 2; k++)
	{
		m_hHeroDieBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_herodieBmpNames[k]));
	}
	for (int k = 0; k < 3; k++)
	{
		m_hLaserBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_laserBmpNames[k]));
	}
	for (int k = 0; k < 3; k++)
	{
		m_hMissileBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_missileBmpNames[k]));
	}

	//加载背景位图
	int k;
	for (k = 0; k < BACKGROUND_COLOR_NUM; k++)
	{
		m_hBackgroundBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_backgroundBmpNames[k]));
	}

	//加载按钮位图
	m_hOthersBmp[0] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_othersBmpNames[0]));
	m_others[0] = CreateOthers(WNDWIDTH - 425, 0, 425, 460, m_hOthersBmp[0]);
	m_hOthersBmp[1] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_othersBmpNames[1]));
	m_others[1] = CreateOthers(250, 100, 450 * 1.2, 250 * 1.2, m_hOthersBmp[1]);
	m_hOthersBmp[2] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_othersBmpNames[2]));
	m_others[2] = CreateOthers(WNDWIDTH / 4, WNDHEIGHT - 160, 115 * 0.62, 108 * 0.62, m_hOthersBmp[2]);
	for (k = 0; k < BUTTON_NUM * 2; k++)
	{
		m_hButtonBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_buttonBmpNames[k]));
		int group = k / 2;
		if (k % 2 == 0)
			m_button[k] = CreateButton(WNDWIDTH  - 340, group * 90 + 5, 320, 90, m_hButtonBmp[k], TRUE);
		else
			m_button[k] = CreateButton(WNDWIDTH  - 340, group * 90 + 5, 320, 90, m_hButtonBmp[k], FALSE);
	}

	for (int k = 0; k < OTHERS_NUM; k++)
	{
		m_hOthersBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_othersBmpNames[k]));
	}
	//创建游戏状态
	//启动计时器
	SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
	//SetTimer(hWnd, SCORE_ID, SCORE_ELAPSE, NULL);
	GameStatusInitialize();
	PlaySound((LPCTSTR)IDR_BGM, NULL, SND_RESOURCE | SND_LOOP | SND_ASYNC);
}

VOID Render(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	//开始绘制
	hdc = BeginPaint(hWnd, &ps);

	HDC	hdcBmp, hdcBuffer;
	HBITMAP	cptBmp;

	cptBmp = CreateCompatibleBitmap(hdc, WNDWIDTH, WNDHEIGHT);
	hdcBmp = CreateCompatibleDC(hdc);
	hdcBuffer = CreateCompatibleDC(hdc);
	SelectObject(hdcBuffer, cptBmp);

	//绘制背景到缓存
	int k;
	for (k = 0; k < MAX_BACKGROUND_NUM; k++)
	{
		Background background = m_background[k];
		SelectObject(hdcBmp, background.hBmp);
		BitBlt(
		hdcBuffer, background.pos.x, background.pos.y,
		background.size.cx, background.size.cy,
		hdcBmp, 0, 0, SRCCOPY);
	}

	SelectObject(hdcBmp, m_gameStatus.hBmp);
	TransparentBlt(hdcBuffer, m_gameStatus.pos.x, m_gameStatus.pos.y, m_gameStatus.size.cx, m_gameStatus.size.cy,
		hdcBmp, 0, m_gameStatus.size.cy * m_gameStatus.isPaused,
		m_gameStatus.size.cx, m_gameStatus.size.cy, RGB(255, 255, 255)
	);

	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 5; j++)
			{
				if (AlphaCoin[k][i][j] == 1)
				{
					SelectObject(hdcBmp, m_coin.hBmp);
					TransparentBlt(
						hdcBuffer, CoinPosition[k][0] + j * COIN_SIZE, CoinPosition[k][1] + i * COIN_SIZE,
						m_coin.size.cx, m_coin.size.cy,
						hdcBmp, 0, 0, 50, 50,
						RGB(255, 255, 255)
					);
				}
				else if (AlphaCoin[k][i][j] == 3)
				{
					SelectObject(hdcBmp, m_hLuckyBmp);
					TransparentBlt(
						hdcBuffer, CoinPosition[k][0] + j * COIN_SIZE, CoinPosition[k][1] + i * COIN_SIZE,
						m_coin.size.cx, m_coin.size.cy,
						hdcBmp, 0, 0, 50, 50,
						RGB(255, 255, 255)
					);
				}
			}
	}

	for (int i = 0; i < MAX_LASER_NUM; i++)
	{
		if (m_laser[i].used)
		{
			SelectObject(hdcBmp, m_hLaserBmp[0]);
			TransparentBlt(
				hdcBuffer, m_laser[i].pos.x, m_laser[i].pos.y,
				m_laser[i].sizeside.cx, m_laser[i].sizeside.cy,
				hdcBmp, 0, 0, m_laser[i].sizeside.cx, m_laser[i].sizeside.cy,
				RGB(255, 255, 255)
			);
			SelectObject(hdcBmp, m_hLaserBmp[1]);
			TransparentBlt(
				hdcBuffer, m_laser[i].pos.x, m_laser[i].pos.y + m_laser[i].length,
				m_laser[i].sizeside.cx, m_laser[i].sizeside.cy,
				hdcBmp, 0, 0, m_laser[i].sizeside.cx, m_laser[i].sizeside.cy,
				RGB(255, 255, 255)
			);
			if (m_laser[i].blink)
			{
				SelectObject(hdcBmp, m_hLaserBmp[2]);
				for (int k = 31; k < m_laser[i].length + 12; k++)
				{
					TransparentBlt(hdcBuffer, m_laser[i].pos.x + 10, m_laser[i].pos.y + k,
						m_laser[i].sizemid.cx, m_laser[i].sizemid.cy,
						hdcBmp, 0, LaserCurrentFrame / 2, m_laser[i].sizemid.cx, m_laser[i].sizemid.cy,
						RGB(0, 0, 0)
					);
				}
			}
		}
	}
	if (m_missile.active)
	{
		if (m_missile.countdown)
		{
			if (m_missile.countdown > 10)
				SelectObject(hdcBmp, m_hMissileBmp[1]);
			else
				SelectObject(hdcBmp, m_hMissileBmp[2]);
			TransparentBlt(
				hdcBuffer, m_missile.pos.x, m_missile.pos.y,
				m_missile.size.cx, m_missile.size.cy,
				hdcBmp, 0, 0, 500, 437,
				RGB(255, 255, 255)
			);
		}
		else
		{
			SelectObject(hdcBmp, m_hMissileBmp[0]);
			TransparentBlt(
				hdcBuffer, m_missile.pos.x, m_missile.pos.y,
				m_missile.size.cx, m_missile.size.cy,
				hdcBmp, 0, 0, 167, 63,
				RGB(255, 255, 255)
			);
		}
	}

	

	//绘制按钮到缓存
	for (k = 0; k < OTHERS_NUM; k++)
	{
		Others others = m_others[k];
		SelectObject(hdcBmp, others.hBmp);
		if (k == 0)
		{
			TransparentBlt(
				hdcBuffer, others.pos.x, others.pos.y,
				others.size.cx, others.size.cy,
				hdcBmp, 0, 0, 285, 462,
				RGB(255, 255, 255)
			);
		}
		else if (k == 1)
		{
			TransparentBlt(
				hdcBuffer, others.pos.x, others.pos.y,
				others.size.cx, others.size.cy,
				hdcBmp, 0, 0, 450, 250,
				RGB(255, 255, 255)
			);
		}
		else
		{
			TransparentBlt(
				hdcBuffer, others.pos.x, others.pos.y,
				others.size.cx, others.size.cy,
				hdcBmp, 0, 0, 115, 108,
				RGB(255, 255, 255)
			);
		}
	}
	if (!AppStore && !SelectMode)
	{
		for (k = 0; k < BUTTON_NUM * 2; k++)
		{
			Button button = m_button[k];
			if (button.active)
			{
				SelectObject(hdcBmp, button.hBmp);
				TransparentBlt(
					hdcBuffer, button.pos.x, button.pos.y,
					button.size.cx, button.size.cy,
					hdcBmp, 0, 0, BUTTON_SIZE_X, BUTTON_SIZE_Y,
					RGB(255, 255, 255)
				);
			}
		}
	}

	//绘制Hero到缓存
	if (m_hero.alive)
	{
		int HeroBitLabel;
		if (m_hero.Status == 0) HeroBitLabel = m_hero.curFrameIndex % HERO_MAX_FRAME_NUM;
		if (m_hero.Status == 1 || GameMode == 3 || GameMode == 4) HeroBitLabel = HERO_MAX_FRAME_NUM + m_hero.curFrameIndex % HERO_MAX_FRAME_UP;
		if (m_hero.Status == 2) HeroBitLabel = HERO_MAX_FRAME_NUM + HERO_MAX_FRAME_UP + m_hero.curFrameIndex % HERO_MAX_FRAME_DOWN;
		if (GameMode == 2 && AntiGravity) HeroBitLabel = HERO_MAX_FRAME_NUM + HERO_MAX_FRAME_UP + HERO_MAX_FRAME_DOWN - 1 - HeroBitLabel;
		if (GameMode == 4 && DoOperation)
		{
			SelectObject(hdcBmp, m_hero.hBmp);
			if (MachineCountdown >= 5)
			{
				TransparentBlt(
					hdcBuffer, m_hero.pos.x, m_hero.pos.y + HERO_SIZE_Y * (10.0 - MachineCountdown) / 10,
					m_hero.size.cx, m_hero.size.cy * (MachineCountdown - 5) / 5,
					hdcBmp, 0, m_hero.size.cy * HeroBitLabel + HERO_SIZE_Y * (10.0 - MachineCountdown) / 10,
					m_hero.size.cx, m_hero.size.cy - HERO_SIZE_Y * (10.0 - MachineCountdown) / 5,
					RGB(255, 255, 255)
				);
			}
			else
			{
				TransparentBlt(
					hdcBuffer, m_hero.pos.x, m_hero.pos.y + HERO_SIZE_Y * MachineCountdown / 10,
					m_hero.size.cx, m_hero.size.cy * (5 - MachineCountdown) / 5,
					hdcBmp, 0, m_hero.size.cy * HeroBitLabel + HERO_SIZE_Y * MachineCountdown / 10,
					m_hero.size.cx, m_hero.size.cy - HERO_SIZE_Y * MachineCountdown / 5,
					RGB(255, 255, 255)
				);
			}
		}
		else if (GameMode == 2 && AntiGravity)
		{
			SelectObject(hdcBmp, m_hHeroInvBmp);
			TransparentBlt(
				hdcBuffer, m_hero.pos.x, m_hero.pos.y,
				m_hero.size.cx, m_hero.size.cy,
				hdcBmp, 0, m_hero.size.cy * HeroBitLabel, m_hero.size.cx, m_hero.size.cy,
				RGB(255, 255, 255)
			);
		}
		else
		{
			SelectObject(hdcBmp, m_hero.hBmp);
			TransparentBlt(
				hdcBuffer, m_hero.pos.x, m_hero.pos.y,
				m_hero.size.cx, m_hero.size.cy,
				hdcBmp, 0, m_hero.size.cy * HeroBitLabel, m_hero.size.cx, m_hero.size.cy,
				RGB(255, 255, 255)
			);
		}
		if (m_hero.invincible)
		{
			if (GameMode == 2 && AntiGravity)
			{
				SelectObject(hdcBmp, m_hShieldInvBmp);
				TransparentBlt(
					hdcBuffer, m_hero.pos.x + HERO_SIZE_X - 9, m_hero.pos.y - 10,
					137 * 0.18, 543 * 0.18, hdcBmp, 0, 0, 137, 543, RGB(0, 0, 0)
				);
			}
			else
			{
				SelectObject(hdcBmp, m_hShieldBmp);
				TransparentBlt(
					hdcBuffer, m_hero.pos.x + HERO_SIZE_X - 9, m_hero.pos.y - 28,
					137 * 0.18, 543 * 0.18, hdcBmp, 0, 0, 137, 543, RGB(0, 0, 0)
				);
			}
		}
		if (GameMode == 4 && GameStart && !DoOperation)
		{
			SelectObject(hdcBmp, m_hHeroMistBmp);
			HeroBitLabel = m_hero.curFrameIndex % HERO_MAX_FRAME_UP;
			TransparentBlt(
				hdcBuffer, m_hero.pos.x + ScorePerFrame * 4 * 5, HeroPosition[MachineCount],
				m_hero.size.cx, m_hero.size.cy,
				hdcBmp, 0, m_hero.size.cy * HeroBitLabel, m_hero.size.cx, m_hero.size.cy,
				RGB(255, 255, 255)
				);
		}
	}
	else
	{
		if (m_hero.pos.y == WNDHEIGHT - HERO_TO_GROUND)
		{
			SelectObject(hdcBmp, m_hHeroDieBmp[1]);
			TransparentBlt(
				hdcBuffer, m_hero.pos.x, m_hero.pos.y,
				64, 62, hdcBmp, 0, 0, 64, 62,
				RGB(255, 255, 255)
			);
		}
		else
		{
			SelectObject(hdcBmp, m_hHeroDieBmp[0]);
			TransparentBlt(
				hdcBuffer, m_hero.pos.x, m_hero.pos.y,
				64, 62, hdcBmp, 0, 0, 64, 62,
				RGB(255, 255, 255)
			);
		}
	}
	if (ScoreBoard)
	{
		SelectObject(hdcBmp, m_hScoreboardBmp);
			TransparentBlt(
				hdcBuffer, WNDWIDTH / 20, WNDHEIGHT / 10,
				WNDWIDTH / 3 * 2, WNDHEIGHT / 3 * 2, hdcBmp, 0, 0, 581, 255,
				RGB(255, 255, 255)
			);
		SelectObject(hdcBmp, m_coin.hBmp);
		char word9[] = "GAME OVER";
		for (int k = 0; k < strlen(word9); k++)
		{
			Transform(9, word9[k] - 65, FALSE);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 10 + 123 + k * 55, basey = WNDHEIGHT / 4 + i * 10;
				for (int j = 0; j < 5; j++)
				{
					if (AlphaCoin[9][i][j])
					{
						TransparentBlt(
							hdcBuffer, basex + j * 10, basey,
							10, 10, hdcBmp, 0, 0, 50, 50,
							RGB(255, 255, 255)
						);
					}
				}
			}
		}
		char word1[] = "DISTANCE";
		for (int k = 0; k < strlen(word1); k++)
		{
			Transform(9, word1[k] - 65, FALSE);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 20 + 80 + k * 44, basey = WNDHEIGHT / 5 + 170 + i * 8;
				for (int j = 0; j < 5; j++)
				{
					if (AlphaCoin[9][i][j])
					{
						TransparentBlt(
							hdcBuffer, basex + j * 8, basey,
							8, 8, hdcBmp, 0, 0, 50, 50,
							RGB(255, 255, 255)
						);
					}
				}
			}
		}
		int num1 = (int)m_gameStatus.totalDist, cnt1 = -1;
		int numC1[5] = {0};
		while (num1)
		{
			numC1[++cnt1] = num1 % 10;
			num1 /= 10;
		}
		reverse(numC1, numC1 + 5);
		for (int k = 0; k < min(CharacterCount, 5); k++)
		{
			Transform(9, numC1[k] + 26, FALSE);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 5 + 340 + k * 44, basey = WNDHEIGHT / 5 + 170 + i * 8;
				for (int j = 0; j < 5; j++)
				{
					if (AlphaCoin[9][i][j])
					{
						TransparentBlt(
							hdcBuffer, basex + j * 8, basey,
							8, 8, hdcBmp, 0, 0, 50, 50,
							RGB(255, 255, 255)
						);
					}
				}
			}
		}
		char word4[] = "M";
		for (int k = 0; k < strlen(word4); k++)
		{
			if (CharacterCount <= 5) break;
			Transform(9, word4[k] - 65, FALSE);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 5 + 565 + k * 33, basey = WNDHEIGHT / 5 + 183 + i * 6;
				for (int j = 0; j < 5; j++)
				{
					if (AlphaCoin[9][i][j])
					{
						TransparentBlt(
							hdcBuffer, basex + j * 6, basey,
							6, 6, hdcBmp, 0, 0, 50, 50,
							RGB(255, 255, 255)
						);
					}
				}
			}
		}
		char word2[] = "COINS";
		for (int k = 0; k < strlen(word2); k++)
		{
			Transform(9, word2[k] - 65, FALSE);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 20 + 80 + k * 44, basey = WNDHEIGHT / 5 + 265 + i * 8;
				for (int j = 0; j < 5; j++)
				{
					if (AlphaCoin[9][i][j])
					{
						TransparentBlt(
							hdcBuffer, basex + j * 8, basey,
							8, 8, hdcBmp, 0, 0, 50, 50,
							RGB(255, 255, 255)
						);
					}
				}
			}
		}
		int num2 = m_gameStatus.totalCoin, cnt2 = -1;
		int numC2[4] = {0};
		while (num2)
		{
			numC2[++cnt2] = num2 % 10;
			num2 /= 10;
		}
		reverse(numC2, numC2 + 4);
		for (int k = 0; k < 4; k++)
		{
			if (CharacterCount <= 6 + k) break;
			Transform(9, numC2[k] + 26, FALSE);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 5 + 384 + k * 44, basey = WNDHEIGHT / 5 + 265 + i * 8;
				for (int j = 0; j < 5; j++)
				{
					if (AlphaCoin[9][i][j])
					{
						TransparentBlt(
							hdcBuffer, basex + j * 8, basey,
							8, 8, hdcBmp, 0, 0, 50, 50,
							RGB(255, 255, 255)
						);
					}
				}
			}
		}
		if (CharacterCount > 10)
		{
			TransparentBlt(
				hdcBuffer, WNDWIDTH / 5 + 565, WNDHEIGHT / 5 + 290, 30, 30, hdcBmp, 0, 0, 50, 50,
				RGB(255, 255, 255)
			);
		}
		char word3[] = "PRESS R TO RESTART";
		for (int k = 0; k < strlen(word3); k++)
		{
			Transform(9, word3[k] - 65, FALSE);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 10 + 445 + k * 16, basey = WNDHEIGHT / 5 + 333 + i * 3;
				for (int j = 0; j < 5; j++)
				{
					if (AlphaCoin[9][i][j])
					{
						TransparentBlt(
							hdcBuffer, basex + j * 3, basey,
							3, 3, hdcBmp, 0, 0, 50, 50,
							RGB(255, 255, 255)
						);
					}
				}
			}
		}
	}

	//绘制游戏状态
	//暂停或继续位图
	

	//绘制分数
	TCHAR szDist[100];
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);

	//TextOut(hdcBuffer, 0, WNDHEIGHT / 2, szDist, wsprintf(szDist, _T("Status = %d"), m_hero.Status));
	/*TextOut(hdcBuffer, 0, WNDHEIGHT / 2, szDist, wsprintf(szDist, _T("Dist = %d"), (int)(ScorePerFrame * 4 * 5)));
	TextOut(hdcBuffer, 0, WNDHEIGHT / 2 + 50, szDist, wsprintf(szDist, _T("MCount = %d"), HeroPosition[MachineCount]));
	TextOut(hdcBuffer, 0, WNDHEIGHT / 2 + 100, szDist, wsprintf(szDist, _T("MRecord = %d"), MachineRecord));
	TextOut(hdcBuffer, 0, WNDHEIGHT / 2 + 150, szDist, wsprintf(szDist, _T("HPosY = %d"), m_hero.pos.y));
	TextOut(hdcBuffer, 0, WNDHEIGHT / 2 + 200, szDist, wsprintf(szDist, _T("MCountDown = %d"), MachineCountdown));*/

	if (GameStart)
	{
		int m0, arr[10], arrcnt;
		m0 = (int)m_gameStatus.totalDist, arrcnt = 0;
		for (int i = 1; i <= 5; i++)
		{
			arr[++arrcnt] = m0 % 10;
			m0 /= 10;
		}
		reverse(arr + 1, arr + arrcnt + 1);
		for (int i = 1; i <= 5; i++)
		{
			SelectObject(hdcBmp, m_hNumberBmp[arr[i]]);
			TransparentBlt(hdcBuffer, 29 * (i - 1), 0, 29, 32,
				hdcBmp, 0, 0, 29, 32, RGB(255, 255, 255)
			);
		}
		m0 = m_gameStatus.totalCoin, arrcnt = 0;
		for (int i = 1; i <= 4; i++)
		{
			arr[++arrcnt] = m0 % 10;
			m0 /= 10;
		}
		reverse(arr + 1, arr + arrcnt + 1);
		for (int i = 1; i <= 4; i++)
		{
			SelectObject(hdcBmp, m_hSmallNumberBmp[arr[i]]);
			TransparentBlt(hdcBuffer, 14 * (i - 1), 35, 14, 18,
				hdcBmp, 0, 0, 14, 18, RGB(255, 255, 255)
			);
		}
		SelectObject(hdcBmp, m_hCoinBmp);
		TransparentBlt(hdcBuffer, 60, 40, 12, 12,
			hdcBmp, 0, 0, 50, 50, RGB(255, 255, 255)
		);
		SelectObject(hdcBmp, m_hLifeBmp);
		for (int i = 1; i <= m_hero.life; i++)
		{
			TransparentBlt(hdcBuffer, 587 * 0.05 * (i - 1), 55, 587 * 0.05, 662 * 0.05,
				hdcBmp, 0, 0, 587, 662, RGB(0, 0, 0)
			);
		}
	}
	
	if (AppStore)
	{
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 15, szDist, wsprintf(szDist, _T("幸运金币(C): ")));
		if (!LuckyCoin)
		{
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			TextOut(hdcBuffer, 1200, 15, szDist, wsprintf(szDist, _T("未激活")));
		}
		else
		{
			SetTextColor(hdcBuffer, RGB(0, 255, 0));
			TextOut(hdcBuffer, 1200, 15, szDist, wsprintf(szDist, _T("已激活")));
		}
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 45, szDist, wsprintf(szDist, _T("每一枚金币都有概率变成幸运金币，")));
		TextOut(hdcBuffer, 950, 60, szDist, wsprintf(szDist, _T("它的价值等于3枚普通金币。")));

		TextOut(hdcBuffer, 950, 120, szDist, wsprintf(szDist, _T("电磁干扰器(E)：")));
		if (!LaserInterference)
		{
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			TextOut(hdcBuffer, 1200, 120, szDist, wsprintf(szDist, _T("未激活")));
		}
		else
		{
			SetTextColor(hdcBuffer, RGB(0, 255, 0));
			TextOut(hdcBuffer, 1200, 120, szDist, wsprintf(szDist, _T("已激活")));
		}
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 150, szDist, wsprintf(szDist, _T("有概率使得激光失去效果，")));
		TextOut(hdcBuffer, 950, 165, szDist, wsprintf(szDist, _T("失去效果的激光可以直接穿过。")));

		TextOut(hdcBuffer, 950, 225, szDist, wsprintf(szDist, _T("蚁人(A)：")));
		if (!AntMan)
		{
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			TextOut(hdcBuffer, 1200, 225, szDist, wsprintf(szDist, _T("未激活")));
		}
		else
		{
			SetTextColor(hdcBuffer, RGB(0, 255, 0));
			TextOut(hdcBuffer, 1200, 225, szDist, wsprintf(szDist, _T("已激活")));
		}
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 255, szDist, wsprintf(szDist, _T("召唤蚁人小伙伴入侵导弹，")));
		TextOut(hdcBuffer, 950, 270, szDist, wsprintf(szDist, _T("损坏的导弹飞行速度降低。")));

		TextOut(hdcBuffer, 950, 330, szDist, wsprintf(szDist, _T("灵魂之戒(S)：")));
		if (!SoulRing)
		{
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			TextOut(hdcBuffer, 1200, 330, szDist, wsprintf(szDist, _T("未激活")));
		}
		else
		{
			SetTextColor(hdcBuffer, RGB(0, 255, 0));
			TextOut(hdcBuffer, 1200, 330, szDist, wsprintf(szDist, _T("已激活")));
		}
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 360, szDist, wsprintf(szDist, _T("游戏开始时减少一点生命值，")));
		TextOut(hdcBuffer, 950, 375, szDist, wsprintf(szDist, _T("但是无敌时间增加一倍。")));

		if (!GameStart) TextOut(hdcBuffer, 1110, 430, szDist, wsprintf(szDist, _T("Press B to return back")));
	}

	if (SelectMode)
	{
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 15, szDist, wsprintf(szDist, _T("火箭飞人模式(Jetpack Joyride)(J): ")));
		if (GameMode != 1)
		{
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			TextOut(hdcBuffer, 1200, 15, szDist, wsprintf(szDist, _T("未激活")));
		}
		else
		{
			SetTextColor(hdcBuffer, RGB(0, 255, 0));
			TextOut(hdcBuffer, 1200, 15, szDist, wsprintf(szDist, _T("已激活")));
		}
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 45, szDist, wsprintf(szDist, _T("按住↑键使人物飞行。")));

		TextOut(hdcBuffer, 950, 120, szDist, wsprintf(szDist, _T("重力小子模式(Gravity Guy)(G)：")));
		if (GameMode != 2)
		{
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			TextOut(hdcBuffer, 1200, 120, szDist, wsprintf(szDist, _T("未激活")));
		}
		else
		{
			SetTextColor(hdcBuffer, RGB(0, 255, 0));
			TextOut(hdcBuffer, 1200, 120, szDist, wsprintf(szDist, _T("已激活")));
		}
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 150, szDist, wsprintf(szDist, _T("使用↑键改变重力方向。")));

		TextOut(hdcBuffer, 950, 225, szDist, wsprintf(szDist, _T("利物鸟模式(Profit Bird)(P)：")));
		if (GameMode != 3)
		{
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			TextOut(hdcBuffer, 1200, 225, szDist, wsprintf(szDist, _T("未激活")));
		}
		else
		{
			SetTextColor(hdcBuffer, RGB(0, 255, 0));
			TextOut(hdcBuffer, 1200, 225, szDist, wsprintf(szDist, _T("已激活")));
		}
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 255, szDist, wsprintf(szDist, _T("使用↑键使人物飞行。")));

		TextOut(hdcBuffer, 950, 330, szDist, wsprintf(szDist, _T("传送器模式(Freaking Teleporter)(T)：")));
		if (GameMode != 4)
		{
			SetTextColor(hdcBuffer, RGB(255, 0, 0));
			TextOut(hdcBuffer, 1200, 330, szDist, wsprintf(szDist, _T("未激活")));
		}
		else
		{
			SetTextColor(hdcBuffer, RGB(0, 255, 0));
			TextOut(hdcBuffer, 1200, 330, szDist, wsprintf(szDist, _T("已激活")));
		}
		SetTextColor(hdcBuffer, RGB(255, 255, 255));
		TextOut(hdcBuffer, 950, 360, szDist, wsprintf(szDist, _T("使用↑键使人物瞬间移动。")));

		if (!GameStart) TextOut(hdcBuffer, 1110, 430, szDist, wsprintf(szDist, _T("Press B to return back")));
	}

	//最后将所有的信息绘制到屏幕上
	BitBlt(hdc, 0, 0, WNDWIDTH, WNDHEIGHT, hdcBuffer, 0, 0, SRCCOPY);

	//回收资源所占的内存
	DeleteObject(cptBmp);
	DeleteDC(hdcBuffer);
	DeleteDC(hdcBmp);

	//结束绘制
	EndPaint(hWnd, &ps);

	if (!ScoreBoard && !m_hero.alive && m_hero.pos.y == WNDHEIGHT - HERO_TO_GROUND)
	{
		KillTimer(hWnd, TIMER_ID);
		//KillTimer(hWnd, SCORE_ID);
		SetTimer(hWnd, ENDING_ID, ENDING_ELAPSE, NULL);
		Sleep(1500);
		ScoreBoard = TRUE;
		AppStore = TRUE;
		CharacterCount = -1;
		m_others[0] = CreateOthers(WNDWIDTH - 425, 0, 425, 460, m_hOthersBmp[0]);
	}
}

Hero CreateHero(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, int curFrameIndex, int Status, int maxFrameSize)
{
	Hero hero;
	hero.hBmp = hBmp;
	hero.pos.x = posX;
	hero.pos.y = posY;
	hero.size.cx = sizeX;
	hero.size.cy = sizeY;
	hero.curFrameIndex = curFrameIndex;
	hero.Status = Status;
	hero.maxFrameSize = maxFrameSize;
	hero.invincible = 0;
	hero.life = 3;
	hero.alive = TRUE;
	return hero;
}
Coin CreateCoin(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Coin coin;
	coin.hBmp = hBmp;
	coin.pos.x = posX;
	coin.pos.y = posY;
	coin.size.cx = sizeX;
	coin.size.cy = sizeY;
	return coin;
}
Laser CreateLaser(LONG posX, LONG posY, LONG sizeXs, LONG sizeYs, LONG sizeXm, LONG sizeYm, int length, BOOL blink)
{
	Laser laser;
	laser.pos.x = posX;
	laser.pos.y = posY;
	laser.sizeside.cx = sizeXs;
	laser.sizeside.cy = sizeYs;
	laser.sizemid.cx = sizeXm;
	laser.sizemid.cy = sizeYm;
	laser.length = length;
	laser.used = TRUE;
	laser.active = TRUE;
	laser.blink = blink;
	return laser;
}
Missile CreateMissile(LONG posX, LONG posY, LONG sizeX, LONG sizeY, BOOL active, int countdown)
{
	Missile missile;
	missile.pos.x = posX;
	missile.pos.y = posY;
	missile.size.cx = sizeX;
	missile.size.cy = sizeY;
	missile.active = active;
	missile.countdown = countdown;
	return missile;
}
Background CreateBackground(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Background background;
	background.hBmp = hBmp;
	background.pos.x = posX;
	background.pos.y = posY;
	background.size.cx = sizeX;
	background.size.cy = sizeY;
	return background;
}
Button CreateButton(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp, BOOL active)
{
	Button button;
	button.hBmp = hBmp;
	button.pos.x = posX;
	button.pos.y = posY;
	button.size.cx = sizeX;
	button.size.cy = sizeY;
	button.active = active;
	return button;
}
Others CreateOthers(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Others others;
	others.hBmp = hBmp;
	others.pos.x = posX;
	others.pos.y = posY;
	others.size.cx = sizeX;
	others.size.cy = sizeY;
	return others;
}
Building CreateBuilding(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	Building building;
	building.hBmp = hBmp;
	building.pos.x = posX;
	building.pos.y = posY;
	building.size.cx = sizeX;
	building.size.cy = sizeY;
	return building;
}
GameStatus CreateGameStatus(LONG posX, LONG posY, LONG sizeX, LONG sizeY, HBITMAP hBmp)
{
	GameStatus gameStatus;
	gameStatus.pos.x = posX;
	gameStatus.pos.y = posY;
	gameStatus.size.cx = sizeX;
	gameStatus.size.cy = sizeY;
	gameStatus.hBmp = hBmp;
	gameStatus.totalDist = 0.0;
	gameStatus.totalCoin = 0;
	gameStatus.currentCoin = 0;
	gameStatus.isPaused = false;
	return gameStatus;
}
Terrian CreateTerrian(LONG posX, LONG posY, LONG sizeX, LONG sizeY, 
					  HBITMAP hBlockBmp, HBITMAP hRoofBmp, int roofHeight, int blockHeight)
{
	Terrian terrian;
	terrian.pos.x = posX;
	terrian.pos.y = posY;
	terrian.size.cx = sizeX;
	terrian.size.cy = sizeY;
	terrian.hBlockBmp = hBlockBmp;
	terrian.hRoofBmp = hRoofBmp;
	terrian.roofWidth = sizeX;
	terrian.roofHeight = roofHeight;
	terrian.blockWidth = sizeX;
	terrian.blockHeight = blockHeight;
	terrian.blockNum = (int)ceil((sizeY - roofHeight) * 1.0 / blockHeight); 
	return terrian;
}

VOID TimerUpdate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if (wParam == TIMER_ID)
	{
		HeroUpdate();
		BackgroundUpdate();
		TerrianUpdate();
		if (GameStart)
		{
			GameStatusUpdate();
			OthersUpdate();
			CoinUpdate();
			MissileUpdate();
			LaserUpdate();
			BOOL judgecollision = FALSE;;
			if (!(GameMode == 4 && DoOperation && MachineCountdown >= 5)) judgecollision = CheckCollision();
			if (judgecollision)
			{
				m_hero.life--;
				if (!m_hero.life)
				{
					m_hero.alive = FALSE;
					IsOnFire = FALSE;
					AntiGravity = FALSE;
					PlaySound((LPCTSTR)IDR_DEATH, NULL, SND_RESOURCE | SND_LOOP | SND_ASYNC);
					//KillTimer(hWnd, TIMER_ID);
					//KillTimer(hWnd, SCORE_ID);
				}
				else
				{
					m_hero.invincible = 100 * (1 + SoulRing);
					for (int i = 0; i < MAX_LASER_NUM; i++)
					{
						m_laser[i].blink = FALSE;
					}
				}
				m_missile.active = FALSE;
			}
			ElapseCount++;
			if (ElapseCount % 1 == 0)
			{
				ScorePerFrame += 0.0025;
			}
			if (ElapseCount % 400 == 0)
			{
				if (LaserRandom >= 9) LaserRandom -= 3;
				if (MissileRandom >= 90) MissileRandom -= 30;
			}
			if (ElapseCount % 1200 == 0)
			{
				Gravity++;
			}
		}
	}
	if (wParam == ENDING_ID)
	{
		CharacterCount++;
	}
	InvalidateRect(hWnd, NULL, FALSE);
}
VOID CoinUpdate()
{
	for (int k = 0; k < 10; k++)
	{
		CoinPosition[k][0] -= (int)(ScorePerFrame * 4 + 0.5);
		if (CoinPosition[k][0] + COIN_SIZE * 5 < 0)
		{
			int prev = k - 1;
			if (prev == -1) prev = 9;
			CoinPosition[k][0] = CoinPosition[prev][0] + GetRandomInt(WNDWIDTH / 3, WNDWIDTH);
			CoinPosition[k][1] = GetRandomInt(0, WNDHEIGHT - COIN_SIZE * 8);
			Transform(k, GetRandomInt(0, 25), LuckyCoin);
		}
	}
}
VOID LaserUpdate()
{
	LaserCurrentFrame++;
	if (LaserCurrentFrame == LASER_MAX_FRAME) LaserCurrentFrame = 0;
	for (int i = 0; i < MAX_LASER_NUM; i++)
		if (m_laser[i].used)
		{
			m_laser[i].pos.x -= (int)(ScorePerFrame * 4 + 0.5);
		}

	if (!LaserGenerate)
	{
		bool LaserJudge = TRUE;
		for (int i = 0; i < 10; i++)
		{
			if (!(CoinPosition[i][0] >= WNDWIDTH + COIN_SIZE || CoinPosition[i][0] + COIN_SIZE * 6 <= WNDWIDTH))
			{
				LaserJudge = FALSE;
				break;
			}
		}
		if (LaserJudge)
		{
			int roll = GetRandomInt(0, LaserRandom);
			if (roll == 0)
			{
				LaserAxis++;
				if (LaserAxis == MAX_LASER_NUM) LaserAxis = 0;
				int length = GetRandomInt(WNDHEIGHT / 5, WNDHEIGHT / 5 * 3);
				int position = GetRandomInt(0, WNDHEIGHT - length - 60);
				int destroy = GetRandomInt(0, 4);
				BOOL brlink = !(m_hero.invincible | (LaserInterference && destroy == 0));
				m_laser[LaserAxis] = CreateLaser(WNDWIDTH, position, 39, 43, 18, 1, length, brlink);
				LaserGenerate = 200;
			}
		}
	}
}
VOID MissileUpdate()
{
	if (m_hero.invincible) return;

	if (m_missile.active)
	{
		if (m_missile.countdown)
		{
			m_missile.countdown--;
			if (!m_missile.countdown)
			{
				m_missile = CreateMissile(WNDWIDTH, m_missile.pos.y + 7, 111, 42, TRUE, 0);
			}
			else if (m_missile.countdown > 10)
			{
				m_missile.pos.y = m_hero.pos.y + 3;
			}
		}
		else
		{
			m_missile.pos.x -= (int)(ScorePerFrame * 12 + 0.5) / (1 + AntMan);
			if (m_missile.pos.x + 167 < 0) m_missile.active = false;
		}
	}
	else if (!MissileGenerate && m_hero.alive)
	{
		int roll = GetRandomInt(0, MissileRandom);
		if (roll == 0)
		{
			m_missile = CreateMissile(WNDWIDTH - 85, m_hero.pos.y + 3, 64, 56, TRUE, 50);
		}
	}
}
VOID OthersUpdate()
{
	INT delta = 15;
	if (m_others[1].pos.y + m_others[1].size.cy > 0) m_others[1].pos.y -= delta;
	if (m_others[0].pos.x < WNDWIDTH)
	{
		m_others[0].pos.x += delta;
		for (int i = 0; i < BUTTON_NUM * 2; i++)
		{
			m_button[i].pos.x += delta;
		}
	}
}
VOID HeroUpdate()
{
	//TODO
	//更新位置
	//更新动作
	double HeroAcceleration = 0;
	if (!m_hero.alive)
	{
		HeroAcceleration = 1.0 * Gravity;
		if (HeroVelocity < 0) HeroAcceleration = 2.0 * Gravity;
	}
	else
	{
		if (GameMode == 1)
		{
			if (IsOnFire)
			{
				HeroAcceleration = 1.0 * (-Gravity);
				if (HeroVelocity > 0) HeroAcceleration = -2.0 * Gravity;
				m_hero.Status = 1;
			}
			else
			{
				HeroAcceleration = 1.0 * Gravity;
				if (HeroVelocity < 0) HeroAcceleration = 2.0 * Gravity;
				m_hero.Status = 2;
			}
		}
		if (GameMode == 2)
		{
			if (DoOperation)
			{
				DoOperation = FALSE;
			}
			if (AntiGravity)
			{
				HeroAcceleration = 1.0 * (-Gravity);
				if (HeroVelocity > 0) HeroAcceleration = -2.0 * Gravity;
			}
			else
			{
				HeroAcceleration = 1.0 * Gravity;
				if (HeroVelocity < 0) HeroAcceleration = 2.0 * Gravity;
			}
			m_hero.Status = 2;
		}
		if (GameMode == 3)
		{
			if (DoOperation)
			{
				HeroAcceleration = -12.0 * Gravity;
				DoOperation = FALSE;
			}
			else
			{
				HeroAcceleration = 1.0 * Gravity;
			}
		}
		if (GameMode == 4 && DoOperation)
		{
			MachineCountdown--;
			if (MachineCountdown == 4) m_hero.pos.y = MachineRecord;
			if (MachineCountdown == 0) DoOperation = FALSE;
		}
	}
	double HeroDisplacement = 1.0 * HeroVelocity + 0.5 * HeroAcceleration;
	HeroVelocity += (int)HeroAcceleration;
	int HeroPosY = m_hero.pos.y + (int)HeroDisplacement;
	if (HeroPosY <= 0)
	{
		HeroPosY = 0;
		HeroVelocity = 0;
		if (AntiGravity && GameMode == 2) m_hero.Status = 0;
	}
	else if (HeroPosY >= WNDHEIGHT - HERO_TO_GROUND)
	{
		HeroPosY = WNDHEIGHT - HERO_TO_GROUND;
		HeroVelocity = 0;
		m_hero.Status = 0;
	}
	m_hero.pos.y = HeroPosY;

	++m_hero.curFrameIndex;
	m_hero.curFrameIndex = m_hero.curFrameIndex >= HERO_MAX_FRAME ? 0 : m_hero.curFrameIndex;
	if (m_hero.invincible) m_hero.invincible--;
	++MachineCount;
	if (MachineCount == 66) MachineCount = 0;
}
VOID BackgroundUpdate()
{
	int k;
	for(k = 0; k < MAX_BACKGROUND_NUM; k++)
	{
		m_background[k].pos.x -= (int)(ScorePerFrame * 4 + 0.5);
		if (m_background[k].pos.x + m_background[k].size.cx < 0)
		{
			m_background[k].pos.x += MAX_BACKGROUND_NUM * BACKGROUND_SIZE_X;
			int randk = GetRandomInt(0, BACKGROUND_COLOR_NUM - 1);
			if (!GameStart) randk = 0;
			m_background[k].hBmp = m_hBackgroundBmp[randk];
		}
	}
	m_others[2].pos.x -= (int)(ScorePerFrame * 4 + 0.5);
}
VOID TerrianUpdate()
{
	//TODO
	int k;
	for (k = 0; k < MAX_TERRIAN_NUM; ++k)
	{
		m_terrian[k].pos.x -= 2;
		if (m_terrian[k].pos.x + m_terrian[k].size.cx < 0)
		{
			m_terrian[k].pos.x += MAX_TERRIAN_NUM * m_terrian[k].size.cx;
		}
	}
}
VOID GameStatusUpdate()
{
	//TODO
	m_gameStatus.totalDist += ScorePerFrame;
	LaserGenerate -= (int)(ScorePerFrame * 4 + 0.5);
	if (LaserGenerate < 0) LaserGenerate = 0;
	MissileGenerate -= (int)(ScorePerFrame * 4 + 0.5);
	if (MissileGenerate < 0) MissileGenerate = 0;
}

BOOL Paused(POINT ptMouse)
{
	RECT rPause;

	rPause.left = m_gameStatus.pos.x;
	rPause.top = m_gameStatus.pos.y;
	rPause.right = m_gameStatus.pos.x + m_gameStatus.size.cx;
	rPause.bottom = m_gameStatus.pos.y + m_gameStatus.size.cy;

	return PtInRect(&rPause, ptMouse);
}
BOOL MouseInButton(POINT ptMouse, Button button)
{
	RECT rButton;

	rButton.left = button.pos.x;
	rButton.top = button.pos.y;
	rButton.right = button.pos.x + 320;
	rButton.bottom = button.pos.y + 90;

	return PtInRect(&rButton, ptMouse);
}

VOID KeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:
		if (!m_hero.alive) break;
		//m_hero.pos.y -= 50;
		if (GameMode == 1)
		{
			IsOnFire = true;
		}
		if (GameMode == 2)
		{
			if (NextOperationValid && !DoOperation)
			{
				DoOperation = TRUE;
				NextOperationValid = FALSE;
				AntiGravity = ~AntiGravity;
			}
		}
		if (GameMode == 3)
		{
			if (NextOperationValid && !DoOperation)
			{
				DoOperation = TRUE;
				NextOperationValid = FALSE;
			}
		}
		if (GameMode == 4)
		{
			if (NextOperationValid && !DoOperation)
			{
				DoOperation = TRUE;
				MachineCountdown = 10;
				int NextMachineCount = MachineCount + 1;
				if (NextMachineCount >= 66) NextMachineCount -= 66;
				MachineRecord = HeroPosition[NextMachineCount];
				NextOperationValid = FALSE;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case VK_DOWN:
		if (!m_hero.alive) break;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	//case 0x52: //VK_R = 0x52
	case 'R':
		if (m_hero.alive) break;
		GameStatusInitialize();
		GameParameterInitialize(LuckyCoin, SoulRing);

		GameStart = TRUE;
		IsOnFire = FALSE;
		ScoreBoard = FALSE;
		AppStore = FALSE;
		HeroVelocity = 0;
		ScorePerFrame = 4.0;
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		//SetTimer(hWnd, SCORE_ID, SCORE_ELAPSE, NULL);
		KillTimer(hWnd, ENDING_ID);
		m_others[0] = CreateOthers(WNDWIDTH, WNDHEIGHT, 425, 460, m_hOthersBmp[0]);
		m_others[2] = CreateOthers(WNDWIDTH / 4, WNDHEIGHT - 160, 115 * 0.62, 108 * 0.62, m_hOthersBmp[2]);
		Gravity = 2;
		ElapseCount = 0;
		NextOperationValid = TRUE;
		DoOperation = FALSE;
		PlaySound((LPCTSTR)IDR_BGM, NULL, SND_RESOURCE | SND_LOOP | SND_ASYNC);
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case 'P':
		if (!m_hero.alive) break;
		if (!m_gameStatus.isPaused)
		{
			KillTimer(hWnd, TIMER_ID);
			//KillTimer(hWnd, SCORE_ID);
			m_gameStatus.isPaused = TRUE;
		}
		else
		{
			SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
			//SetTimer(hWnd, SCORE_ID, SCORE_ELAPSE, NULL);
			m_gameStatus.isPaused = FALSE;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		break;
	}
}
VOID KeyDown2(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case 'C':
			LuckyCoin = !LuckyCoin;
			break;
		case 'E':
			LaserInterference = !LaserInterference;
			break;
		case 'A':
			AntMan = !AntMan;
			break;
		case 'S':
			SoulRing = !SoulRing;
			break;
		case 'B':
			if (!GameStart)
			{
				AppStore = FALSE;
			}
			break;
	}
	InvalidateRect(hWnd, NULL, FALSE);
}
VOID KeyDown3(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case 'J':
			GameMode = 1;
			break;
		case 'G':
			GameMode = 2;
			break;
		case 'P':
			GameMode = 3;
			break;
		case 'T':
			GameMode = 4;
			break;
		case 'B':
			if (!GameStart)
			{
				SelectMode = FALSE;
			}
			break;
	}
	InvalidateRect(hWnd, NULL, FALSE);
}
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:
		if (GameMode == 1)
		{
			IsOnFire = false;
		}
		if (GameMode == 2)
		{
			NextOperationValid = TRUE;
		}
		if (GameMode == 3)
		{
			NextOperationValid = TRUE;
		}
		if (GameMode == 4)
		{
			NextOperationValid = TRUE;
		}
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case VK_DOWN:
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		break;
	}
}
VOID MouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);
	if (!GameStart)
	{
		for (int i = 0; i < BUTTON_NUM * 2; i += 2)
		{
			int group = i / 2;
			if(MouseInButton(ptMouse, m_button[i]))
			{
				m_button[i].active = FALSE;
				m_button[i + 1].active = TRUE;
				if (LeftKeyDown)
				{
					m_button[i].pos.x = m_button[i + 1].pos.x = WNDWIDTH - 340 + 16;
					m_button[i].pos.y = m_button[i + 1].pos.y = group * 90 + 5 + 4.5;
					m_button[i].size.cx = m_button[i + 1].size.cx = 320 - 16 * 2;
					m_button[i].size.cy = m_button[i + 1].size.cy = 90 - 4.5 * 2;
				}
			}
			else
			{
				m_button[i].active = TRUE;
				m_button[i + 1].active = FALSE;
				if (LeftKeyDown)
				{
					m_button[i].pos.x = m_button[i + 1].pos.x = WNDWIDTH - 340;
					m_button[i].pos.y = m_button[i + 1].pos.y = group * 90 + 5;
					m_button[i].size.cx = m_button[i + 1].size.cx = 320;
					m_button[i].size.cy = m_button[i + 1].size.cy = 90;
				}
			}
		}
	}
	//InvalidateRect(hWnd, NULL, FALSE);
}
VOID LButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	if (!GameStart)
	{
		for (int i = 0; i < BUTTON_NUM * 2; i += 2)
		{
			int group = i / 2;
			m_button[i].pos.x = m_button[i + 1].pos.x = WNDWIDTH - 340;
			m_button[i].pos.y = m_button[i + 1].pos.y = group * 90 + 5;
			m_button[i].size.cx = m_button[i + 1].size.cx = 320;
			m_button[i].size.cy = m_button[i + 1].size.cy = 90;
		}
		if (!AppStore && !SelectMode && MouseInButton(ptMouse, m_button[0]))
		{
			GameParameterInitialize(LuckyCoin, SoulRing);
			//SetTimer(hWnd, SCORE_ID, SCORE_ELAPSE, NULL);
			GameStart = TRUE;
		}
		else if (!AppStore && !SelectMode && MouseInButton(ptMouse, m_button[4]))
		{
			AppStore = TRUE;
		}
		else if (!AppStore && !SelectMode && MouseInButton(ptMouse, m_button[2]))
		{
			SelectMode = TRUE;
		}
	}
	InvalidateRect(hWnd, NULL, FALSE);
}
VOID LButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	POINT ptMouse;
	ptMouse.x = LOWORD(lParam);
	ptMouse.y = HIWORD(lParam);

	//如果点击了继续或暂停图标
	
	if (Paused(ptMouse))
	{
		if (!m_hero.alive) return;
		if (!m_gameStatus.isPaused)
		{
			KillTimer(hWnd, TIMER_ID);
			//KillTimer(hWnd, SCORE_ID);
			m_gameStatus.isPaused = TRUE;
		}
		else
		{
			SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
			//SetTimer(hWnd, SCORE_ID, SCORE_ELAPSE, NULL);
			m_gameStatus.isPaused = FALSE;
		}
		InvalidateRect(hWnd, NULL, FALSE);
	}
	else if (!GameStart)
	{
		for (int i = 0; i < BUTTON_NUM * 2; i += 2)
		{
			int group = i / 2;
			if(MouseInButton(ptMouse, m_button[i]))
			{
				m_button[i].pos.x = m_button[i + 1].pos.x = WNDWIDTH - 340 + 16;
				m_button[i].pos.y = m_button[i + 1].pos.y = group * 90 + 5 + 4.5;
				m_button[i].size.cx = m_button[i + 1].size.cx = 320 - 16 * 2;
				m_button[i].size.cy = m_button[i + 1].size.cy = 90 - 4.5 * 2;
			}
			else
			{
				m_button[i].pos.x = m_button[i + 1].pos.x = WNDWIDTH - 340;
				m_button[i].pos.y = m_button[i + 1].pos.y = group * 90 + 5;
				m_button[i].size.cx = m_button[i + 1].size.cx = 320;
				m_button[i].size.cy = m_button[i + 1].size.cy = 90;
			}
		}
		InvalidateRect(hWnd, NULL, FALSE);
	}
}