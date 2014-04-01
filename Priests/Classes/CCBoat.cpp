#include "CCBoat.h"
#include "HelloWorldScene.h"

USING_NS_CC;

extern CCPoint g_LeftCoast[6];
extern CCPoint g_RightCoast[6];

CCBoat::CCBoat(void)
{
}

CCBoat::~CCBoat(void)
{
	m_passenger1 = NULL;
	m_passenger2 = NULL;
}

CCBoat* CCBoat::createWithVoid()
{
	CCBoat* pCCBoat = CCBoat::create();
	pCCBoat->initWithFile("boat.png");
	pCCBoat->m_isMoving = false;
	pCCBoat->setScale(0.6);
	pCCBoat->m_passenger1 = NULL;
	pCCBoat->m_passenger2 = NULL;

	return pCCBoat;
}


void CCBoat::loadSprite(CCSprite *pSprite)
{
	
	CCPoint boatPos = this->getPosition();
	CCPoint spritePos = pSprite->getPosition();
	int screenWidth = CCDirector::sharedDirector()->getVisibleSize().width;
	if( (screenWidth/2 - boatPos.x)*(screenWidth/2 - spritePos.x) > 0 ) //精灵与船在同侧
	{
		if(m_passenger1 == NULL)
		{
			m_passenger1 = pSprite;
			pSprite->setZOrder(pSprite->getZOrder()+1);
			pSprite->setPosition(ccp(boatPos.x-50, boatPos.y+50));
		}
		else if(m_passenger2 == NULL)
		{
			m_passenger2 = pSprite;
			pSprite->setZOrder(pSprite->getZOrder()+1);
			pSprite->setPosition(ccp(boatPos.x+50, boatPos.y+50));
		}
	}
}

void CCBoat::unloadSprite(CCSprite *pSprite)
{
	CCPoint boatPos = this->getPosition(); 
	float screenWidth = CCDirector::sharedDirector()->getVisibleSize().width;

	if(m_passenger1 == pSprite)
	{
		pSprite->setZOrder(pSprite->getZOrder()-1);
		m_passenger1 = NULL;
	}
	else
	{
		pSprite->setZOrder(pSprite->getZOrder()-1);
		m_passenger2 = NULL;
	}

	if(boatPos.x < screenWidth/2)
	{
		pSprite->setPosition(g_LeftCoast[pSprite->getTag()]);
		float scaleX = pSprite->getScaleX();
		if(scaleX > 0)
		{
			pSprite->setScaleX(-scaleX);
		}
	}
	else
	{
		pSprite->setPosition(g_RightCoast[pSprite->getTag()]);
		float scaleX = pSprite->getScaleX();
		if(scaleX < 0)
		{
			pSprite->setScaleX(-scaleX);
		}
	}
}

void CCBoat::move()
{
	if(m_passenger1 || m_passenger2)
	{
		m_isMoving = true;

		float screenWidth = CCDirector::sharedDirector()->getVisibleSize().width;

		CCPoint srcPoint = this->getPosition();
		CCPoint destPoint = ccp(screenWidth-srcPoint.x, srcPoint.y);
		
		if(m_passenger1)
		{
			m_passenger1->runAction(CCMoveBy::create(1, destPoint-srcPoint));
		}
		if(m_passenger2)
		{
			m_passenger2->runAction(CCMoveBy::create(1, destPoint-srcPoint));
		}

		CCFiniteTimeAction* pActSeq = CCSequence::create(CCMoveBy::create(1, destPoint-srcPoint), CCCallFunc::create(this, callfunc_selector(CCBoat::stopMoving)),NULL);

		this->runAction(pActSeq);
	}
}

bool CCBoat::isMoving()
{
	return m_isMoving;
}

void CCBoat::stopMoving()
{
	m_isMoving = false;
}
