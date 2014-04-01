#ifndef __GAME_HELPER__
#define __GAME_HELPER__

#include <algorithm>
#include <queue>
#include <string>
#include <stack>
#include "cocos2d.h"
using namespace std;

USING_NS_CC;

class Helper
{
public:
	static Helper *getInstance()
	{
		static Helper *s_pInstance = new Helper();
		return s_pInstance;
	}
	CCString *nextState(int *cnt);
private:
	Helper();
	void init();

	int chg(int *arr);
	void unchg(int state, int *arr);
	bool gameOver(int *arr);
	void expand(int curState);

	static const int MAX_STATE = 2*4*4, INF = 1e3;
	queue<int> q;
	pair<int, int> m_nPath[MAX_STATE];
	int m_nStep[MAX_STATE];
};
#endif