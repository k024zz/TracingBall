#include "GameOverScene.h"
#include "HelloWorldScene.h"
using namespace cocos2d;

GameOverScene::GameOverScene(void)
{
}


GameOverScene::~GameOverScene(void)
{
}

bool GameOverScene::init()
{
	bool bRet = false;
	do
	{
		CC_BREAK_IF(! CCLayer::init());
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		//this->initWithColor(ccc4(0, 0, 0, 0));
		this->_label = CCLabelTTF::create("", "Cooper Std", 60);
		CC_BREAK_IF(! _label);
		_label->retain();
		_label->setColor(ccc3(255, 255, 255));
		_label->setPosition(ccp(winSize.width / 2, winSize.height / 2 + 30 ));

		//添加背景图片
		/*
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		CCSprite* pSprite = CCSprite::create("BGP001.jpg");
		CC_BREAK_IF(! pSprite);
		pSprite->setPosition(ccp(size.width/2, size.height/2));
		this->addChild(pSprite, 0);
		*/


		this->addChild(_label);

		
		//三秒后切换场景
		this->runAction( CCSequence::create( CCDelayTime::create(3), 
																	  CCCallFunc::create( this, callfunc_selector(GameOverScene::gameOverDone) ), 
																	  NULL) );
		bRet = true;
	} while (0);

	return bRet;
}

void GameOverScene::gameOverDone()
{
	// 用導演類重新加載HelloWorldScene場景
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0, HelloWorld::scene()));
}

GameOverScene *GameOverScene::create()
{
    GameOverScene *pRet = new GameOverScene();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

