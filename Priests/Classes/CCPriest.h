#pragma once
#include "cocos2d.h"
USING_NS_CC;

class CCPriest : public CCSprite, public CCTargetedTouchDelegate
{
public:
	CCPriest(void);
	~CCPriest(void);

	static CCPriest* createWithVoid();
	virtual bool containsTouchLocation(CCPoint touchPoint);

	CREATE_FUNC(CCPriest);

private:
	CCAnimate* m_pStand;
};