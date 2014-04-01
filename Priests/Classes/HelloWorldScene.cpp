#include "HelloWorldScene.h"
#include "CCDevil.h"
#include "CCPriest.h"
#include "CCBoat.h"
#include "GameOverScene.h"
#include <typeinfo>

USING_NS_CC;

#define LIMITTIME 60

// Golbal
CCArray *g_pSpriteArray;
CCBoat *g_pBoat;
CCPoint g_LeftCoast[6];
CCPoint g_RightCoast[6]; 
CCLabelTTF* g_pTimeLabel;
float		g_remainTime;
bool		g_GameisRunning;

//Functions
void replaceScene(char* str);


CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "GoNormal.png",
                                        "GoSelected.png",
                                        this,
                                        menu_selector(HelloWorld::menuCloseCallback));
    pCloseItem->setScale(0.15);
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - 60) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 1);


	// 提示按钮以及提示字开始
	CCMenuItemImage *pHintItem = CCMenuItemImage::create(
		"hint.png",
		"hint.png",
		this,
		menu_selector(HelloWorld::menuHintCallback));
	pHintItem->setScale(0.15);
	pHintItem->setPosition(ccp(origin.x + visibleSize.width/2,
		origin.y + visibleSize.height - 120) );

	CCMenu *pHintMenu = CCMenu::create(pHintItem, NULL);
	pHintMenu->setPosition(CCPointZero);
	this->addChild(pHintMenu, 1);
	m_pHint = NULL;

	//添加背景
	CCSprite* pBackground = CCSprite::create("background.png");
	pBackground->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
	pBackground->setScaleX(1.6);
	pBackground->setZOrder(2);
	this->addChild(pBackground, 0);

	//添加地形
	CCSprite* pTerrain = CCSprite::create("terrain.png");
	pTerrain->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y+15));
	this->addChild(pTerrain, 1);

	//创建船精灵
	g_pBoat = CCBoat::createWithVoid();
	g_pBoat->setPosition(ccp(200, 60));
	this->addChild(g_pBoat, 5);

    //  添加河流
    CCSprite* pSprite = CCSprite::create("blue.png");
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y-240));
    this->addChild(pSprite, 6);


	//初始化位置数组
	for(int i=0; i<6; i++)
	{
		if(i<3)
		{
			g_LeftCoast[i] = ccp(40+i*70, 200);
			g_RightCoast[i] = ccp(visibleSize.width-40-i*70, 200);
		}
		else
		{
			g_LeftCoast[i] = ccp(120+i*40, 200);
			g_RightCoast[i] = ccp(visibleSize.width-120-i*40, 200);
		}
	}

	//创建6个精灵
	g_pSpriteArray = CCArray::create();
	g_pSpriteArray->retain();

	for(int i=0; i<6; i++)
	{
		CCSprite *pCharacter;
		if(i < 3)
		{
			pCharacter = CCDevil::createWithVoid();
		}
		else
		{
			pCharacter = CCPriest::createWithVoid();
		}
		pCharacter->setTag(i);
		pCharacter->setPosition(g_LeftCoast[i]);
		this->addChild(pCharacter,3);
		g_pSpriteArray->addObject(pCharacter);
	}

	//初始化时间
	g_remainTime = LIMITTIME;

	//生成时间字体
	char str[128];
	sprintf(str, "%.2f", g_remainTime);
	g_pTimeLabel = CCLabelTTF::create(str, "Consolas", 48);
	g_pTimeLabel->setPosition(ccp(origin.x +g_pTimeLabel->getContentSize().width/2 ,
		origin.y + visibleSize.height - g_pTimeLabel->getContentSize().height-10));
	this->addChild(g_pTimeLabel, 8);

	//每帧调用update
	scheduleUpdate();
	g_GameisRunning = true;
	

	//开启触碰监听
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);

    return true;
}

void HelloWorld::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint touchPoint = convertTouchToNodeSpace(pTouch);
	for( int i = 0; i < g_pSpriteArray->count(); i++ )
	{
		CCSprite *pSprite = (CCDevil *)(g_pSpriteArray->objectAtIndex(i));
		//CCMessageBox(typeid(*pSprite).name(), "");

		if( strcmp(typeid(*pSprite).name(), "class CCDevil") == 0 )
		{
			if(((CCDevil*)pSprite)->containsTouchLocation(touchPoint))
			{
				if(pSprite->getPositionY() >= 200)
				{
					g_pBoat->loadSprite(pSprite);
				}
				else
				{
					g_pBoat->unloadSprite(pSprite);
				}
				break;
			}
		}
		else if( strcmp(typeid(*pSprite).name(), "class CCPriest") == 0 )
		{
			if(((CCPriest*)pSprite)->containsTouchLocation(touchPoint))
			{
				if(pSprite->getPositionY() >= 200)
				{
					g_pBoat->loadSprite(pSprite);
				}
				else
				{
					g_pBoat->unloadSprite(pSprite);
				}
				break;
			}
		}
	}
}

void HelloWorld::checkGame()
{
	if(g_remainTime > 0)
	{
		float midX = CCDirector::sharedDirector()->getVisibleSize().width/2;

		int NumofLeftDevil = 0;
		int NumofRightDevil = 0;
		int NumofLeftPriest = 0;
		int NumofRightPriest = 0;

		for(int i=0; i<g_pSpriteArray->count(); i++)
		{


			CCSprite* pSprite = (CCSprite*)g_pSpriteArray->objectAtIndex(i);
			if(pSprite->getPositionX() < midX)
			{
				if(i<3)
				{
					NumofLeftDevil++;
				}
				else
				{
					NumofLeftPriest++;
				}
			}
			else
			{
				if(i<3)
				{
					NumofRightDevil++;
				}
				else
				{
					NumofRightPriest++;
				}
			}

		}

		if(NumofLeftPriest > 0 && NumofLeftDevil > NumofLeftPriest)
		{
			replaceScene("You Lose!");
		}

		if(NumofRightPriest > 0 && NumofRightDevil > NumofRightPriest)
		{
			replaceScene("You Lose!");
		}

		if(NumofRightPriest + NumofRightDevil == 6)
		{
			replaceScene("You Win!");
		}
	}
}

void HelloWorld::update(float deltaTime)
{
	if(g_GameisRunning)
	{
		g_remainTime -= deltaTime;

		if(g_remainTime < 0)
		{
			g_remainTime = 0;
			g_GameisRunning = false;
			replaceScene("Time out!");
		}

		char str[128];
		sprintf(str, "%.2f", g_remainTime);

		g_pTimeLabel->setString(str);
	}
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	if(!g_pBoat->isMoving())
	{
		g_pBoat->move();
		
		CCSequence *pActSeq = CCSequence::create(CCDelayTime::create(1.1),CCCallFunc::create(this, callfunc_selector(HelloWorld::checkGame)),NULL);
		this->runAction(pActSeq);
	}
}
void HelloWorld::menuHintCallback(CCObject* pSender)
{
	if(!g_pBoat->isMoving())
	{
		if (m_pHint != NULL)
		{
			m_pHint->removeFromParent();
			m_pHint = NULL;
		}
		m_pHint = CCLabelTTF::create(showNextStep()->getCString(), "Thonburi", 20);
		CCSize size = CCDirector::sharedDirector()->getVisibleSize();
		m_pHint->setPosition(ccp(size.width / 2, size.height - 15));
		this->addChild(m_pHint);
	}
}

CCString *HelloWorld::showNextStep()
{
	int arr[3];
	float midX = CCDirector::sharedDirector()->getVisibleSize().width / 2;

	if (g_pBoat->getPositionX() < midX)
		arr[0] = 0;
	else
		arr[0] = 1;

	arr[1] = arr[2] = 0;
	for (int i = 0; i < g_pSpriteArray->count(); i++)
	{
		CCSprite *pSprite = (CCSprite*)g_pSpriteArray->objectAtIndex(i);
		if (pSprite->getPositionX() < midX)
		{
			if (i < 3)
				arr[1]++;
			else
				arr[2]++;
		}
	}
	return Helper::getInstance()->nextState(arr);
}

void replaceScene(char* str)
{
	GameOverScene* gameOverScene = GameOverScene::create();
	if(gameOverScene)
	{
		gameOverScene->getLabel()->setString(str);
		CCScene* pScene = CCScene::create();
		pScene->addChild(gameOverScene);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.0, pScene));
	}
}

HelloWorld::~HelloWorld()
{
}

