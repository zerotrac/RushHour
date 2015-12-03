#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>

const int Gravity = 2;
const int Alphabet[36][5] = {{0x3F, 0x48, 0x48, 0x48, 0x3F}, //A
					  	 	 {0x7F, 0x49, 0x49, 0x49, 0x36}, //B
							 {0x3E, 0x41, 0x41, 0x41, 0x22}, //C
							 {0x7F, 0x41, 0x41, 0x22, 0x1C}, //D
							 {0x7F, 0x49, 0x49, 0x49, 0x41}, //E
							 {0x7F, 0x48, 0x48, 0x48, 0x40}, //F
							 {0x3E, 0x41, 0x41, 0x49, 0x4F}, //G
							 {0x7F, 0x08, 0x08, 0x08, 0x7F}, //H
							 {0x41, 0x41, 0x7F, 0x41, 0x41}, //I
							 {0x02, 0x41, 0x41, 0x7E, 0x40}, //J
							 {0x7F, 0x08, 0x14, 0x22, 0x41}, //K
							 {0x7F, 0x01, 0x01, 0x01, 0x01}, //L
							 {0x7F, 0x20, 0x18, 0x20, 0x7F}, //M
							 {0x7F, 0x10, 0x08, 0x04, 0x7F}, //N
							 {0x3E, 0x41, 0x41, 0x41, 0x3E}, //O
							 {0x7F, 0x48, 0x48, 0x48, 0x30}, //P
							 {0x3E, 0x41, 0x45, 0x42, 0x3D}, //Q
							 {0x7F, 0x48, 0x4C, 0x4A, 0x31}, //R
							 {0x32, 0x49, 0x49, 0x49, 0x26}, //S
							 {0x40, 0x40, 0x7F, 0x40, 0x40}, //T
							 {0x7E, 0x01, 0x01, 0x01, 0x7E}, //U
							 {0x7C, 0x02, 0x01, 0x02, 0x7C}, //V
							 {0x7E, 0x01, 0x1E, 0x01, 0x7E}, //W
							 {0x63, 0x14, 0x08, 0x14, 0x63}, //X
							 {0x60, 0x10, 0x0F, 0x10, 0x60}, //Y
							 {0x43, 0x45, 0x49, 0x51, 0x61}, //Z
							 {0x3E, 0x45, 0x49, 0x51, 0x3E}, //0
							 {0x00, 0x21, 0x7F, 0x01, 0x00}, //1
							 {0x21, 0x43, 0x45, 0x49, 0x31}, //2
							 {0x42, 0x41, 0x51, 0x69, 0x46}, //3
							 {0x0C, 0x14, 0x24, 0x7F, 0x04}, //4
							 {0x72, 0x51, 0x51, 0x51, 0x4E}, //5
							 {0x1E, 0x29, 0x49, 0x49, 0x06}, //6
							 {0x40, 0x47, 0x48, 0x50, 0x60}, //7
							 {0x36, 0x49, 0x49, 0x49, 0x36}, //8
							 {0x30, 0x49, 0x49, 0x4A, 0x3C}  //9
							};

int AlphaCoin[10][7][5] = {0};
int CoinPosition[10][2];

void Transform(int label, int num)
{
	int BinaryLine[8];
	for (int i = 0; i < 5; i++)
	{
		int p = Alphabet[num][i];
		for (int j = 0; j < 8; j++)
		{
			BinaryLine[7 - j] = p & 1;
			p >>= 1;
		}
		for (int j = 1; j < 8; j++)
		{
			AlphaCoin[label][j - 1][i] = BinaryLine[j];
		}
	}
}

/*

(x1, y1)|-------|
		|		|
		|-------|(x2, y2)

*/

BOOL Collision(Hero hero, int x1, int y1, int x2, int y2, int type)
{
	if (!hero.alive && type != 1) return FALSE;
	int hx1 = hero.pos.x, hy1 = hero.pos.y;
	int hx2 = hx1 + hero.size.cx, hy2 = hy1 + hero.size.cy;
	if (hx2 < x1 || hx1 > x2) return FALSE;
	if (hy2 < y1 || hy1 > y2) return FALSE;
	return TRUE;
}

int LaserGenerate;
int LaserAxis;
int LaserCurrentFrame;
int LaserRandom;
int MissileGenerate;
int MissileRandom;

BOOL CheckCollision()
{
	BOOL CollisionwithMissile = FALSE;
	BOOL CollisionwithLaser = FALSE;
	if (m_missile.active)
	{
		CollisionwithMissile = Collision(m_hero,
										 m_missile.pos.x, m_missile.pos.y,
										 m_missile.pos.x + m_missile.size.cx, m_missile.pos.y + m_missile.size.cy, 0);
	}

	for (int i = 0; i < 30; i++)
	{
		if (m_laser[i].active && m_laser[i].blink)
		{
			CollisionwithLaser |= Collision(m_hero, m_laser[i].pos.x + 13, m_laser[i].pos.y + 15,
											m_laser[i].pos.x + 26, m_laser[i].pos.y + m_laser[i].length + 12, 0);
		}
	}

	for (int k = 0; k < 10; k++)
	{
		for (int i = 0; i < 7; i++)
			for (int j = 0; j < 5; j++)
			{
				if (AlphaCoin[k][i][j])
				{
					if (Collision(m_hero,
								  CoinPosition[k][0] +  j      * COIN_SIZE, CoinPosition[k][1] +  i      * COIN_SIZE,
								  CoinPosition[k][0] + (j + 1) * COIN_SIZE, CoinPosition[k][1] + (i + 1) * COIN_SIZE, 1))
					{
						AlphaCoin[k][i][j] = 0;
						m_gameStatus.totalCoin++;
						if (m_hero.alive && m_gameStatus.totalCoin % 100 == 0) m_hero.life++;
					}
				}
			}
	}

	return CollisionwithMissile | CollisionwithLaser;
}

VOID GameStatusInitialize()
{
	m_hero = CreateHero(WNDWIDTH / 4, WNDHEIGHT - HERO_TO_GROUND, HERO_SIZE_X, HERO_SIZE_Y, m_hHeroBmp, 0, 0, HERO_MAX_FRAME_NUM);

	m_missile = CreateMissile(0, 0, 0, 0, FALSE, 0);

	for (int k = 0; k < 10; k++)
	{
		if (k == 0)
		{
			CoinPosition[k][0] = GetRandomInt(WNDWIDTH * 2, WNDWIDTH * 3);
		}
		else
		{
			CoinPosition[k][0] = CoinPosition[k - 1][0] + GetRandomInt(WNDWIDTH / 3, WNDWIDTH);
		}
		CoinPosition[k][1] = GetRandomInt(0, WNDHEIGHT - COIN_SIZE * 8);
		Transform(k, GetRandomInt(0, 25));
	}
	for (int k = 0; k < MAX_BACKGROUND_NUM; k++)
	{
		int randk = 0;
		m_background[k] = CreateBackground(BACKGROUND_SIZE_X * k, 0, BACKGROUND_SIZE_X, BACKGROUND_SIZE_Y, m_hBackgroundBmp[randk]);
	}
	for (int k = 0; k < MAX_LASER_NUM; k++)
	{
		m_laser[k].active = FALSE;
	}
}

VOID GameParameterInitialize()
{
	LaserGenerate = WNDWIDTH * 2;
	LaserAxis = -1;
	LaserCurrentFrame = 0;
	LaserRandom = 30;
	MissileGenerate = WNDWIDTH;
	MissileRandom = 300;
	for (int i = 0; i < MAX_LASER_NUM; i++)
	{
		m_laser[i].used = FALSE;
	}
	m_button[0].active = TRUE;
	m_button[1].active = FALSE;
	m_gameStatus = CreateGameStatus(10, 8, GAME_STATUS_SIZE_X, GAME_STATUS_SIZE_Y, m_hGameStatusBmp);
}