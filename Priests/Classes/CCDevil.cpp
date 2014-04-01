#include "CCDevil.h"
USING_NS_CC;

CCDevil::CCDevil(void)
{
}


CCDevil::~CCDevil(void)
{
}

CCDevil* CCDevil::createWithVoid()
{
	CCDevil* pCCDevil = CCDevil::create();

	//读取plist
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("devil_0.plist", "devil_0.png");

	//生成站立动画
	CCArray *frames = CCArray::create();
	for( int i=0; i<6; i++ )
	{
		char pngName[30];
		sprintf(pngName, "devil_stand_0%d.png", i+1);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pngName);
		frames->addObject(frame);
	}
	CCAnimation* pStand = CCAnimation::createWithSpriteFrames(frames, 1./6);
	frames->release();


	//生成攻击动画
	frames = CCArray::create();
	for( int i=0; i<7; i++ )
	{
		char pngName[30];
		sprintf(pngName, "devil_attack_0%d.png", i+1);
		CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(pngName);
		frames->addObject(frame);
	}
	CCAnimation* pAttack = CCAnimation::createWithSpriteFrames(frames, 1./6);
	frames->release();

	pCCDevil->setScaleX(-0.7);
	pCCDevil->setScaleY(0.7);
	pCCDevil->m_pStand = CCAnimate::create(pStand);
	pCCDevil->m_pAttack = CCAnimate::create(pAttack);
	pCCDevil->runAction(CCRepeatForever::create(pCCDevil->m_pStand));

	return pCCDevil;
}

void CCDevil::attack()
{
	this->runAction(m_pAttack);
}

bool CCDevil::containsTouchLocation(CCPoint touchPoint)
{
	CCRect rect = this->boundingBox();
	float x, y, width, height;
	x = rect.getMinX() + 20;
	y = rect.getMinY();
	width = rect.getMaxX() - rect.getMinX() - 40;
	height = rect.getMaxY() - rect.getMinY();

	return CCRect(x, y, width, height).containsPoint(touchPoint);
}