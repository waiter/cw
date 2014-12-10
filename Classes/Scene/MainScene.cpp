#include "MainScene.h"
#include "ScreenChanger/ScreenChanger.h"
#include "Util/Button.h"
#include "GameScene.h"
#include "Util/Utils.h"
#include "GlobleDefine.h"
#include "Music/MusicManager.h"
USING_NS_CC;

MainScene::MainScene()
{

}

MainScene::~MainScene()
{

}

bool MainScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		INIT_RAND();
		std::vector<cocos2d::ccColor3B> temp = Utils::getDiffColor(3,ccWHITE);
		ccColor3B tempC = temp.at(0);
		CCLayerColor* lc0 = CCLayerColor::create(ccc4(tempC.r,tempC.g,tempC.b,255));
		addChild(lc0);

		CCSpriteBatchNode* bn = CCSpriteBatchNode::create("all.png",15);
		addChild(bn);
		for(int i = 0 ; i < 15 ; i++){
			CCSprite* ball = CCSprite::createWithSpriteFrameName("circle.png");
			bn->addChild(ball);
			resetBalls(ball);
		}

		CCLayerColor* lc = CCLayerColor::create(ccc4(255,255,255,200));
		addChild(lc);

		CCSprite* title = CCSprite::createWithSpriteFrameName("game_title.png");
		title->setPosition(SC_SCPP(0.5f,0.75f));
		addChild(title);

		Button* startB = Button::create(
			CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("start.png"),
			this, menu_selector(MainScene::startCallBack)
			);
		startB->setPosition(SC_SCPP(0.5f,0.4f));
		addChild(startB);

		this->setKeypadEnabled(true);
		bRet = true;
	} while (0);
	return bRet;
}

cocos2d::CCScene* MainScene::scene()
{
	CCScene* scene = CCScene::create();
	MainScene* layer = MainScene::create();
	scene->addChild(layer);
	return scene;
}

void MainScene::startCallBack( cocos2d::CCObject* pSender )
{
	MusicManager::playSound(SOUND_CLICK);
	CCScene *pScene = GameScene::scene();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f,pScene));
}

void MainScene::resetBalls( cocos2d::CCObject* ball )
{
	CCSprite* b = (CCSprite*)ball;
	b->setOpacity(0);
	b->setColor(ccc3(255.0f*CCRANDOM_0_1(),255.0f*CCRANDOM_0_1(),255.0f*CCRANDOM_0_1()));
	float width = SC_WIDTH()-100.0f;
	float height = SC_HEIGHT()-100.0f;
	b->stopAllActions();
	b->setPosition(ccpAdd(ccp(width*CCRANDOM_0_1(),height*CCRANDOM_0_1()),ccp(50,50)  ));
	b->runAction(CCSequence::create(
		CCDelayTime::create(0.1f +  1.0f*CCRANDOM_0_1()  ),
		CCFadeIn::create(0.5f),
		CCDelayTime::create(0.5f + 1.0f*CCRANDOM_0_1()),
		CCFadeOut::create(0.5f),
		CCCallFuncO::create(this,callfuncO_selector(MainScene::resetBalls),b),
		NULL));
}

void MainScene::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}
