#include "LogoScene.h"
#include "ScreenChanger/ScreenChanger.h"
#include "MainScene.h"
#include "Constant/Constant.h"
#include "Music/MusicManager.h"
USING_NS_CC;

bool LogoScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		Constant::init();
		CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("all.plist");

		CCLayerColor* ly = CCLayerColor::create(ccc4(255,255,255,255));
		addChild(ly);

		CCSprite* logo = CCSprite::createWithSpriteFrameName("title.png");
		logo->setPosition(SC_SCPP(0.5f,0.6f));
		addChild(logo,1);

		this->scheduleOnce(schedule_selector(LogoScene::loadAllRes),0.2f);

		this->runAction(CCSequence::createWithTwoActions(
			CCDelayTime::create(1.0f),
			CCCallFunc::create(this,callfunc_selector(LogoScene::toGameScene))
			));
		bRet = true;
	} while (0);
	return bRet;
}

cocos2d::CCScene* LogoScene::scene()
{
	CCScene* scene = CCScene::create();
	CCLayer* layer = LogoScene::create();
	scene->addChild(layer);
	return scene;
}

void LogoScene::toGameScene()
{
	CCScene *pScene = MainScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f,pScene));
}

void LogoScene::loadAllRes( float dt )
{
	MusicManager::initAll();
	CCTextureCache::sharedTextureCache()->addImage("all.png");
}
