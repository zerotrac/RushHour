#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>

using namespace std;

int GetRandomInt(int RangeL, int RangeR)
{
	int randint = rand() % (RangeR - RangeL + 1) + RangeL;
	return randint;
}