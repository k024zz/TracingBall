#pragma once
#include "cocos2d.h"

class GameOverScene :
	public cocos2d::CCLayer
{
public:
	GameOverScene(void);
	~GameOverScene(void);

	virtual bool init();

	static GameOverScene* create(void);

	void gameOverDone();

	//通过这个类来获得CCLabelTTF的指针
	CC_SYNTHESIZE_READONLY(cocos2d::CCLabelTTF*, _label, Label);
};



