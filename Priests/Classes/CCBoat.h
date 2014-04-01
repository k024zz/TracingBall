#pragma once
#include "cocos2d.h"

class CCBoat : public cocos2d::CCSprite
{
public:
	CCBoat(void);
	~CCBoat(void);

	static CCBoat* createWithVoid();
	void loadSprite(cocos2d::CCSprite *pSprite);
	void unloadSprite(cocos2d::CCSprite *pSprite);
	void move();
	bool isSpare();
	bool isMoving();

	CREATE_FUNC(CCBoat);

private:
	void stopMoving();
	bool	m_isMoving;
	cocos2d::CCSprite* m_passenger1;
	cocos2d::CCSprite* m_passenger2;
};

