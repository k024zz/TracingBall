#include "Helper.h"
#include "cocos2d.h"
#include "CCPriest.h"
#include "CCDevil.h"
#include "HelloWorldScene.h"

USING_NS_CC;

Helper::Helper()
{
	init();
}

int Helper::chg(int *arr)
{
	int ret = 0;
	for (int i = 0; i < 3; i++)
		ret = ret*4 + arr[i];
	return ret;
}
void Helper::unchg(int state, int *arr)
{
	for (int i = 2; i >= 0; i--)
		arr[i] = state % 4, state /= 4;
}
bool Helper::gameOver(int *arr)
{
	if (arr[1] > arr[2] && arr[2] > 0)
		return true;
	if (3-arr[1] > 3-arr[2] && 3-arr[2] > 0)
		return true;
	return false;
}
void Helper::expand(int curState)
{
	int curArr[3], nexArr[3];
	
	unchg(curState, curArr);

	CCLOG("%d %d %d", curArr[0], curArr[1], curArr[2]);
	CCLOG("----");

	if (curArr[0] == 0)
	{
		int nDevil = curArr[1], nPriest = curArr[2];
		for (int iDevil = 0; iDevil <= nDevil; iDevil++) for (int iPriest = 0; iPriest <= nPriest; iPriest++)
		{
			if (iDevil + iPriest <= 0 || iDevil + iPriest > 2) continue;
			nexArr[0] = 1;
			nexArr[1] = nDevil - iDevil;
			nexArr[2] = nPriest - iPriest;
			if (!gameOver(nexArr))
			{
				int nexState = chg(nexArr);
				
				CCLOG("%d %d %d", nexArr[0], nexArr[1], nexArr[2]);

				if (m_nStep[nexState] > m_nStep[curState] + 1)
				{
					m_nStep[nexState] = m_nStep[curState] + 1;
					m_nPath[nexState] = make_pair(iDevil, iPriest);
					q.push(nexState);
				}
			}
		}
	}
	else
	{
		int nDevil = 3-curArr[1], nPriest = 3-curArr[2];
		for (int iDevil = 0; iDevil <= nDevil; iDevil++) for (int iPriest = 0; iPriest <= nPriest; iPriest++)
		{
			if (iDevil + iPriest <= 0 || iDevil + iPriest > 2) continue;
			nexArr[0] = 0;
			nexArr[1] = 3 - (nDevil - iDevil);
			nexArr[2] = 3 - (nPriest - iPriest);
			if (!gameOver(nexArr))
			{
				CCLOG("%d %d %d", nexArr[0], nexArr[1], nexArr[2]);
				int nexState = chg(nexArr);
				if (m_nStep[nexState] > m_nStep[curState] + 1)
				{
					m_nStep[nexState] = m_nStep[curState] + 1;
					m_nPath[nexState] = make_pair(iDevil, iPriest);
					q.push(nexState);
				}
			}
		}
	}
	CCLOG("");
}

void Helper::init()
{
	for (int i = 0; i < MAX_STATE; i++)
	{
		m_nStep[i] = INF;
		m_nPath[i] = make_pair(-1, -1);
	}
	m_nStep[16] = 0;
	q.push(16);

	static int t = 0;
	while (!q.empty())
	{
		int curState = q.front(); q.pop();
		t++;
		expand(curState);
	}
	CCLOG("tot:%d", t);
}

CCString *Helper::nextState(int *cnt)
{
	for (int i = 0; i < 3; i++)
		CCLOG("cnt[%d] = %d", i, cnt[i]);
	int curState = chg(cnt);
	if (m_nPath[curState] == make_pair(-1, -1))
		return CCString::create("");
	return CCString::createWithFormat("The Next state is \
		Move (%d Devil(s), %d Priest(s) to the %s riverside.\
		", m_nPath[curState].first, m_nPath[curState].second,
		cnt[0]==0?"right":"left");
}