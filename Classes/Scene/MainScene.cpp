#include "MainScene.h"
#include "ScreenChanger/ScreenChanger.h"
#include "Util/Button.h"
#include "GameScene.h"
#include "Util/Utils.h"
#include "GlobleDefine.h"
#include "Music/MusicManager.h"
#include "Constant/Constant.h"
#include "ChangeScene.h"
USING_NS_CC;

MainScene::MainScene()
	:_needChangeToLa(1)
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
		_color = temp.at(1);
		CCLayerColor* lc0 = CCLayerColor::create(ccc4(tempC.r,tempC.g,tempC.b,255));
		addChild(lc0);

		CCSpriteBatchNode* bn = CCSpriteBatchNode::create("total.png",15);
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

		float tx=SC_WIDTH()/2;
		float ty = SC_HEIGHT() * 0.4f;
		if(Constant::isShowAd){
			float dty = 60;
			startB->setPositionY(ty+dty);

			Button* noAdB = Button::create(
				CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("noad.png"),
				this, menu_selector(MainScene::noAdCallBack)
				);
			noAdB->setPosition(ccp(tx,ty-dty));
			addChild(noAdB);
		}

		char s[100] = {0};
		float by = ty - 160;
		float laly = 70;
		float lalx = 100;
		float bx = tx - 1.5f * lalx;
		for(int i = 0 ; i < 8 ; i++){
			CCPoint tempP = ccp(bx + lalx * (i%4),by - laly*(int)(i/4));
			sprintf(s , "flag_%s.png",Constant::getLaStr(i+1).c_str());
			if(i+1 == Constant::getCurrentLa()){
				CCLayerColor* lal = CCLayerColor::create(ccc4(127,127,127,255));
				lal->setContentSize(CCSizeMake(90,60));
				lal->ignoreAnchorPointForPosition(false);
				lal->setPosition(tempP);
				addChild(lal);

				CCSprite* flag = CCSprite::createWithSpriteFrameName(s);
				flag->setPosition(tempP);
				addChild(flag);
			}else{
				Button* flag = Button::create(
					CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s),
					this, menu_selector(MainScene::flagCallBack)
					);
				flag->setPosition(tempP);
				addChild(flag,i);
			}

		}

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

void MainScene::noAdCallBack( cocos2d::CCObject* pSender )
{
	MusicManager::playSound(SOUND_CLICK);
	Utils::toNoAd();
}

void MainScene::flagCallBack( cocos2d::CCObject* pSender )
{
	int index = ((CCSprite*)pSender)->getZOrder();
	if(index > - 1&& index < 8){
		MusicManager::playSound(SOUND_CLICK);
		_needChangeToLa = index+1;
		showBigFrame(false,_color);
	}
}

void MainScene::changeLa()
{
	CCScene* s = CCScene::create();
	ChangeScene* l = ChangeScene::create();
	s->addChild(l);
	l->setBgColor(_color);
	l->setChangeToLa(_needChangeToLa);
	CCDirector::sharedDirector()->replaceScene(s);
	l->startChange();
}

void MainScene::showBigFrame( bool isOpen , cocos2d::ccColor3B color )
{
	float time = 0.5f;
	Button* startB = Button::create(
		CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("start.png"),
		NULL, NULL
		);
	startB->setAnchorPoint(CCPointZero);
	startB->setScaleX(SC_WIDTH()/startB->getContentSize().width );
	startB->setScaleY(SC_HEIGHT() / startB->getContentSize().height);
	startB->setOpacity(0);
	addChild(startB,100);

	CCLayerColor* c1 = CCLayerColor::create(ccc4(color.r,color.g,color.b,255),SC_WIDTH(),SC_HEIGHT()/2.0f);
	addChild(c1,101);

	CCLayerColor* c2 = CCLayerColor::create(ccc4(color.r,color.g,color.b,255),SC_WIDTH(),SC_HEIGHT()/2.0f);
	addChild(c2,101);

	if(isOpen){
		c1->setPositionY(SC_HEIGHT() / 2.0f);
		c1->runAction(CCSequence::createWithTwoActions(
			CCEaseSineInOut::create(CCMoveTo::create(time,ccp(0,SC_HEIGHT()))),
			CCRemoveSelf::create()
			));
		c2->runAction(CCSequence::createWithTwoActions(
			CCEaseSineInOut::create(CCMoveTo::create(time,ccp(0,-SC_HEIGHT()/2.0f))),
			CCRemoveSelf::create()
			));
		startB->runAction(CCSequence::createWithTwoActions(
			CCDelayTime::create(time),
			CCRemoveSelf::create()
			));
	}else{
		c1->setPositionY(SC_HEIGHT());
		c2->setPositionY(-SC_HEIGHT() / 2.0f);
		c1->runAction(CCSequence::createWithTwoActions(
			CCEaseSineInOut::create(CCMoveTo::create(time,ccp(0,SC_HEIGHT()/2.0f))),
			CCRemoveSelf::create()
			));
		c2->runAction(CCSequence::createWithTwoActions(
			CCEaseSineInOut::create(CCMoveTo::create(time,ccp(0,0))),
			CCRemoveSelf::create()
			));
		startB->runAction(CCSequence::createWithTwoActions(
			CCDelayTime::create(time),
			CCCallFunc::create(this,callfunc_selector(MainScene::changeLa))
			));
	}
}
