#pragma once
#include "cocos2d.h"
USING_NS_CC;

class CCDevil : public CCSprite, public CCTargetedTouchDelegate
{
public:
	CCDevil(void);
	~CCDevil(void);

	static CCDevil* createWithVoid();
	void attack();
	virtual bool containsTouchLocation(CCPoint touchPoint);

	CREATE_FUNC(CCDevil);

private:
	CCAnimate* m_pStand;
	CCAnimate* m_pAttack;
};

