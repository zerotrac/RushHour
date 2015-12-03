#pragma once

#include "RushHour.h"
#include "Random.h"
#include "Jumping.h"

BOOL GameStart = FALSE;
BOOL LeftKeyDown = FALSE;
BOOL IsOnFire = FALSE;
BOOL ScoreBoard = FALSE;
int HeroVelocity = 0;
int ScorePerFrame = 4;
int CharacterCount = -1;

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
	m_hShieldBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_SHIELD));
	//m_hHeroUpBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_HEROUP));
	//m_hHeroDownBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_HERODOWN));
	//m_hero = CreateHero(WNDWIDTH / 4, WNDHEIGHT - HERO_TO_GROUND, HERO_SIZE_X, HERO_SIZE_Y, m_hHeroBmp, 0, 0, HERO_MAX_FRAME_NUM);
	m_hCoinBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_COIN));
	m_coin = CreateCoin(100, 100, COIN_SIZE, COIN_SIZE, m_hCoinBmp);
	//m_laser = CreateLaser(100, 100, 39, 43, 18, 1, 200);
	//加载游戏状态位图
	m_hGameStatusBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_GAME_STATUS));
	//m_gameStatus = CreateGameStatus(10, 8, GAME_STATUS_SIZE_X, GAME_STATUS_SIZE_Y, m_hGameStatusBmp);
	m_hScoreboardBmp = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_SCOREBOARD));
	//m_missile = CreateMissile(0, 0, 0, 0, FALSE, 0);
	
	for (int k = 0; k < 2; k++)
	{
		m_hHeroDieBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_herodieBmpNames[k]));
	}
	for (int k = 0; k < 3; k++)
	{
		m_hLaserBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_laserBmpNames[k]));
	}
	for (int k = 0; k < 2; k++)
	{
		m_hMissileBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_missileBmpNames[k]));
	}

	//加载背景位图
	int k;
	for (k = 0; k < BACKGROUND_COLOR_NUM; k++)
	{
		m_hBackgroundBmp[k] = LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(m_backgroundBmpNames[k]));
	}
	/*for (k = 0; k < MAX_BACKGROUND_NUM; k++)
	{
		int randk = 0;
		m_background[k] = CreateBackground(BACKGROUND_SIZE_X * k, 0, BACKGROUND_SIZE_X, BACKGROUND_SIZE_Y, m_hBackgroundBmp[randk]);
	}*/

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
	//创建英雄、建筑
	
	//m_building = CreateBuilding(0, 70, BUILDING_SIZE_X, BUILDING_SIZE_Y, m_hBuildingBmp);
	//创建背景
	
	//创建地形
	/*for (k = 0; k < MAX_TERRIAN_NUM; k++)
	{
		if (k % 4 == 0) continue;
		m_terrian[k] = CreateTerrian(k * 65, 100 + 50 * (k % 2), BLOCK_SIZE_X, 300, m_hBlockBmp[k % 4], m_hRoofkBmp[k % 2], ROOF_SIZE_Y, BLOCK_SIZE_Y);
	}*/
	//创建游戏状态
	
	//启动计时器
	SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
	//SetTimer(hWnd, SCORE_ID, SCORE_ELAPSE, NULL);
	GameStatusInitialize();
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
	SelectObject(hdcBmp, m_coin.hBmp);
	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 5; j++)
			{
				if (AlphaCoin[k][i][j])
				{
					TransparentBlt(
						hdcBuffer, CoinPosition[k][0] + j * COIN_SIZE, CoinPosition[k][1] + i * COIN_SIZE,
						m_coin.size.cx, m_coin.size.cy,
						hdcBmp, 0, 0, 50, 50,
						RGB(255, 255, 255)
					);
				}
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

	for (int i = 0; i < 100; i++)
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
			SelectObject(hdcBmp, m_hMissileBmp[1]);
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

	//绘制Hero到缓存
	if (m_hero.alive)
	{
		SelectObject(hdcBmp, m_hero.hBmp);
		int HeroBitLabel;
		if (m_hero.Status == 0) HeroBitLabel = m_hero.curFrameIndex % HERO_MAX_FRAME_NUM;
		if (m_hero.Status == 1) HeroBitLabel = HERO_MAX_FRAME_NUM + m_hero.curFrameIndex % HERO_MAX_FRAME_UP;
		if (m_hero.Status == 2) HeroBitLabel = HERO_MAX_FRAME_NUM + HERO_MAX_FRAME_UP + m_hero.curFrameIndex % HERO_MAX_FRAME_DOWN;
		TransparentBlt(
			hdcBuffer, m_hero.pos.x, m_hero.pos.y,
			m_hero.size.cx, m_hero.size.cy,
			hdcBmp, 0, m_hero.size.cy * HeroBitLabel, m_hero.size.cx, m_hero.size.cy,
			RGB(255, 255, 255)
			);
		if (m_hero.invincible)
		{
			SelectObject(hdcBmp, m_hShieldBmp);
			TransparentBlt(
				hdcBuffer, m_hero.pos.x + HERO_SIZE_X - 9, m_hero.pos.y - 28,
				137 * 0.18, 543 * 0.18, hdcBmp, 0, 0, 137, 543, RGB(0, 0, 0)
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
				hdcBuffer, WNDWIDTH / 5, WNDHEIGHT / 5,
				WNDWIDTH / 5 * 3, WNDHEIGHT / 5 * 3, hdcBmp, 0, 0, 582, 288,
				RGB(255, 255, 255)
			);
		SelectObject(hdcBmp, m_coin.hBmp);
		char word1[] = "DISTANCE";
		for (int k = 0; k < strlen(word1); k++)
		{
			Transform(9, word1[k] - 65);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 5 + 30 + k * 44, basey = WNDHEIGHT / 5 + 30 + i * 8;
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
		int num1 = m_gameStatus.totalDist, cnt1 = -1;
		int numC1[5] = {0};
		while (num1)
		{
			numC1[++cnt1] = num1 % 10;
			num1 /= 10;
		}
		reverse(numC1, numC1 + 5);
		for (int k = 0; k < min(CharacterCount, 5); k++)
		{
			Transform(9, numC1[k] + 26);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 5 + 500 + k * 44, basey = WNDHEIGHT / 5 + 30 + i * 8;
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
			Transform(9, word4[k] - 65);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 5 + 725 + k * 33, basey = WNDHEIGHT / 5 + 43 + i * 6;
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
			Transform(9, word2[k] - 65);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 5 + 30 + k * 44, basey = WNDHEIGHT / 5 + 200 + i * 8;
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
			Transform(9, numC2[k] + 26);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 5 + 544 + k * 44, basey = WNDHEIGHT / 5 + 200 + i * 8;
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
				hdcBuffer, WNDWIDTH / 5 + 725, WNDHEIGHT / 5 + 225, 30, 30, hdcBmp, 0, 0, 50, 50,
				RGB(255, 255, 255)
			);
		}
		char word3[] = "PRESS R TO RESTART";
		for (int k = 0; k < strlen(word3); k++)
		{
			Transform(9, word3[k] - 65);
			for (int i = 0; i < 7; i++)
			{
				int basex = WNDWIDTH / 5 + 470 + k * 16, basey = WNDHEIGHT / 5 + 350 + i * 3;
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
	SelectObject(hdcBmp, m_gameStatus.hBmp);
	TransparentBlt(hdcBuffer, m_gameStatus.pos.x, m_gameStatus.pos.y, m_gameStatus.size.cx, m_gameStatus.size.cy,
		hdcBmp, 0, m_gameStatus.size.cy * m_gameStatus.isPaused,
		m_gameStatus.size.cx, m_gameStatus.size.cy, RGB(255, 255, 255));

	//绘制分数
	TCHAR szDist[13];
	SetTextColor(hdcBuffer, RGB(0, 0, 0));
	SetBkMode(hdcBuffer, TRANSPARENT);
	TextOut(hdcBuffer, 100, 15, szDist, wsprintf(szDist, _T("距离:%d"), m_gameStatus.totalDist));
	TextOut(hdcBuffer, 100, 30, szDist, wsprintf(szDist, _T("金币:%d"), m_gameStatus.totalCoin));
	TextOut(hdcBuffer, 100, 45, szDist, wsprintf(szDist, _T("生命:%d"), m_hero.life));

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
		KillTimer(hWnd, SCORE_ID);
		SetTimer(hWnd, ENDING_ID, ENDING_ELAPSE, NULL);
		Sleep(1500);
		ScoreBoard = TRUE;
		CharacterCount = -1;
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
	gameStatus.totalDist = 0;
	gameStatus.totalCoin = 0;
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
			BOOL judgecollision = CheckCollision();
			if (judgecollision)
			{
				m_hero.life--;
				if (!m_hero.life)
				{
					m_hero.alive = FALSE;
					IsOnFire = FALSE;
					//KillTimer(hWnd, TIMER_ID);
					//KillTimer(hWnd, SCORE_ID);
				}
				else
				{
					m_hero.invincible = 100;
					for (int i = 0; i < MAX_LASER_NUM; i++)
					{
						m_laser[i].blink = FALSE;
					}
				}
				m_missile.active = FALSE;
			}
		}
	}
	if (wParam == SCORE_ID)
	{
		//GameStatusUpdate();
		ScorePerFrame++;
		if (LaserRandom >= 9) LaserRandom -= 3;
		if (MissileRandom >= 90) MissileRandom -= 30;

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
		CoinPosition[k][0] -= ScorePerFrame * 4;
		if (CoinPosition[k][0] + COIN_SIZE * 5 < 0)
		{
			int prev = k - 1;
			if (prev == -1) prev = 9;
			CoinPosition[k][0] = CoinPosition[prev][0] + GetRandomInt(WNDWIDTH / 3, WNDWIDTH);
			CoinPosition[k][1] = GetRandomInt(0, WNDHEIGHT - COIN_SIZE * 8);
			Transform(k, GetRandomInt(0, 25));
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
			m_laser[i].pos.x -= ScorePerFrame * 4;
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
				m_laser[LaserAxis] = CreateLaser(WNDWIDTH, position, 39, 43, 18, 1, length, !m_hero.invincible);
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
				m_missile = CreateMissile(WNDWIDTH, m_hero.pos.y + 10, 111, 42, TRUE, 0);
			}
			else
			{
				m_missile.pos.y = m_hero.pos.y + 3;
			}
		}
		else
		{
			m_missile.pos.x -= ScorePerFrame * 12;
			if (m_missile.pos.x + 167 < 0) m_missile.active = false;
		}
	}
	else if (!MissileGenerate && m_hero.alive)
	{
		int roll = GetRandomInt(0, LaserRandom);
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
	//m_hero.pos.x += 1;
	//m_hero.pos.x = m_hero.pos.x >= WNDWIDTH ? 0 : m_hero.pos.x;
	//更新动作
	int HeroAcceleration;
	if (IsOnFire)
	{
		HeroAcceleration = -Gravity;
		if (HeroVelocity > 0) HeroAcceleration = -1.5 * Gravity;
		m_hero.Status = 1;
	}
	else
	{
		HeroAcceleration = Gravity;
		if (HeroVelocity < 0) HeroAcceleration = 1.5 * Gravity;
		m_hero.Status = 2;
	}
	int HeroDisplacement = HeroVelocity + 0.5 * HeroAcceleration;
	HeroVelocity += HeroAcceleration;
	int HeroPosY = m_hero.pos.y + HeroDisplacement;
	if (HeroPosY < 0)
	{
		HeroPosY = 0;
		HeroVelocity = 0;
	}
	else if (HeroPosY > WNDHEIGHT - HERO_TO_GROUND)
	{
		HeroPosY = WNDHEIGHT - HERO_TO_GROUND;
		HeroVelocity = 0;
		m_hero.Status = 0;
	}
	m_hero.pos.y = HeroPosY;

	++m_hero.curFrameIndex;
	m_hero.curFrameIndex = m_hero.curFrameIndex >= HERO_MAX_FRAME ? 0 : m_hero.curFrameIndex;
	if (m_hero.invincible) m_hero.invincible--;
}
VOID BackgroundUpdate()
{
	int k;
	for(k = 0; k < MAX_BACKGROUND_NUM; k++)
	{
		m_background[k].pos.x -= ScorePerFrame * 4;
		if (m_background[k].pos.x + m_background[k].size.cx < 0)
		{
			m_background[k].pos.x += MAX_BACKGROUND_NUM * BACKGROUND_SIZE_X;
			int randk = GetRandomInt(0, BACKGROUND_COLOR_NUM - 1);
			if (!GameStart) randk = 0;
			m_background[k].hBmp = m_hBackgroundBmp[randk];
		}
	}
	m_others[2].pos.x -= ScorePerFrame * 4;
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
	LaserGenerate -= ScorePerFrame * 4;
	if (LaserGenerate < 0) LaserGenerate = 0;
	MissileGenerate -= ScorePerFrame * 4;
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
		IsOnFire = true;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case VK_DOWN:
		if (!m_hero.alive) break;
		InvalidateRect(hWnd, NULL, FALSE);
		break;
	case 0x52: //VK_R = 0x52
		if (m_hero.alive) break;
		GameStatusInitialize();
		GameParameterInitialize();

		GameStart = TRUE;
		IsOnFire = FALSE;
		ScoreBoard = FALSE;
		HeroVelocity = 0;
		ScorePerFrame = 4;
		SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
		SetTimer(hWnd, SCORE_ID, SCORE_ELAPSE, NULL);
		KillTimer(hWnd, ENDING_ID);
		m_others[2] = CreateOthers(WNDWIDTH / 4, WNDHEIGHT - 160, 115 * 0.62, 108 * 0.62, m_hOthersBmp[2]);

		InvalidateRect(hWnd, NULL, FALSE);
		break;
	default:
		break;
	}
}
VOID KeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	//TODO
	switch (wParam)
	{
	case VK_UP:
		//m_hero.pos.y += 50;
		IsOnFire = false;
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
		if (MouseInButton(ptMouse, m_button[0]))
		{
			GameParameterInitialize();
			SetTimer(hWnd, SCORE_ID, SCORE_ELAPSE, NULL);
			GameStart = TRUE;
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
		if (!m_gameStatus.isPaused)
		{
			KillTimer(hWnd, TIMER_ID);
			KillTimer(hWnd, SCORE_ID);
			m_gameStatus.isPaused = TRUE;
		}
		else
		{
			SetTimer(hWnd, TIMER_ID, TIMER_ELAPSE, NULL);
			SetTimer(hWnd, SCORE_ID, SCORE_ELAPSE, NULL);
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