#include "GameScene.h"
#include "Util/Utils.h"
#include "Util/Button.h"
#include "ScreenChanger/ScreenChanger.h"
#include "Constant/Constant.h"
#include "GlobleDefine.h"
#include "Music/MusicManager.h"
USING_NS_CC;

GameScene::GameScene()
	:_bgColor(NULL)
	,_currentColorSize(3)
	,_currentSize(3)
	,_currentOtherSizeCount(1)
	,_allBalls(NULL)
	,_stage(0)
	,_isCanAction(false)
	,_colorflagBalls(NULL)
	,_timeProgress(NULL)
	,_score(NULL)
	,_flagBn(NULL)
	,_isStart(false)
	,_gameOverLayer(NULL)
	,_bestScore(NULL)
	,_tips(NULL)
{

}

GameScene::~GameScene()
{
	CC_SAFE_RELEASE_NULL(_bgColor);
	CC_SAFE_RELEASE_NULL(_allBalls);
	CC_SAFE_RELEASE_NULL(_colorflagBalls);
	CC_SAFE_RELEASE_NULL(_timeProgress);
	CC_SAFE_RELEASE_NULL(_score);
	CC_SAFE_RELEASE_NULL(_flagBn);
	CC_SAFE_RELEASE_NULL(_gameOverLayer);
	CC_SAFE_RELEASE_NULL(_bestScore);
	CC_SAFE_RELEASE_NULL(_tips);
}

bool GameScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		INIT_RAND();
		_stage = 0;
		resetWithStage(true);
		Constant::reset();
		ccColor3B start = _currentColors.at(0);

		_bgColor = CCLayerColor::create(ccc4(start.r,start.g,start.b,255));
		addChild(_bgColor);
		_bgColor->retain();

		CCSpriteBatchNode* bn = CCSpriteBatchNode::create("all.png",26);
		addChild(bn,1);

		float DisP = 640.0f / 5.0f;
		CCPoint startP = ccpSub(SC_SCPP(0.5f,0.45f),ccp(2.0f* DisP,2.0f* DisP));

		_allBalls = CCArray::createWithCapacity(25);
		_allBalls->retain();
		for(int i = 0 ; i < 25 ; i++){
			Button* ball = Button::create(
				CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("circle.png"),
				NULL,NULL);
			ball->setSelectedTarget(this,menu_selector(GameScene::ballSelectCallBack));
			ball->setPosition(ccpAdd(startP,ccp( DisP*(i%5),DisP* ((int)i/5) )));
			bn->addChild(ball,i);

			_allBalls->addObject(ball);
		}

		_tips = CCSprite::createWithSpriteFrameName("points.png");
		bn->addChild(_tips,26);
		_tips->setAnchorPoint(ccp(0.5f,1));
		_tips->retain();
		_tips->setVisible(false);

		float leftHight = SC_HEIGHT() - (startP.y + DisP*4.0f + 60.0f);
		CCLayerColor* lc = CCLayerColor::create(ccc4(255,255,255,200));
		lc->setContentSize(CCSizeMake(SC_WIDTH(),leftHight));
		lc->setPositionY(SC_HEIGHT()-leftHight);
		addChild(lc,2);

		_flagBn = CCSpriteBatchNode::create("all.png",11);
		lc->addChild(_flagBn);
		_flagBn->retain();

		_colorflagBalls = CCArray::createWithCapacity(10);
		_colorflagBalls->retain();
		float scale = leftHight *0.8f / 300.0f;
		float tmpS = SC_WIDTH() / (10.0f * 120.0f);
		scale = scale > tmpS ? tmpS : scale;
		scale = scale > 1.0f ? 1 : scale;
		for(int i = 0 ; i < 4 ; i++){
			CCSprite* flag = CCSprite::createWithSpriteFrameName("circle.png");
			flag->setScale(scale);
			flag->setPosition(ccp(SC_WIDTH()/2.0f,2.5f* leftHight/3.0f));
			_flagBn->addChild(flag);

			_colorflagBalls->addObject(flag);
		}

		CCSprite* timeBarBg = CCSprite::createWithSpriteFrameName("timebarbg.png");
		timeBarBg->setPosition(ccp(SC_WIDTH()/2.0f,1.5f* leftHight/3.0f));
		_flagBn->addChild(timeBarBg);

		_timeProgress = CCProgressTimer::create(CCSprite::createWithSpriteFrameName("timebar.png"));
		_timeProgress->setType(kCCProgressTimerTypeBar);
		_timeProgress->setMidpoint(ccp(0,0.5f));
		_timeProgress->setBarChangeRate(ccp(1,0));
		_timeProgress->setColor(ccc3(200,52,43));
		_timeProgress->setPosition(timeBarBg->getPosition());
		lc->addChild(_timeProgress);
		_timeProgress->retain();

		_timeProgress->setPercentage(Constant::getTimePercent()*100.0f);

		_score = CCLabelBMFont::create("0","font.fnt");
		_score->setPosition(ccp(SC_WIDTH()/2.0f,0.5f* leftHight/3.0f));
		_score->setColor(ccBLACK);
		lc->addChild(_score);
		_score->retain();

		float overHeight = SC_HEIGHT()-leftHight;
		_gameOverLayer = CCLayerColor::create(ccc4(255,255,255,200));
		_gameOverLayer->setContentSize(CCSizeMake(SC_WIDTH(),overHeight));
		addChild(_gameOverLayer,2);
		_gameOverLayer->retain();

		CCSprite* bestS = CCSprite::createWithSpriteFrameName("bestscore.png");
		bestS->setPosition(ccp(SC_WIDTH()/2.0f , overHeight * 0.8f));
		bestS->setColor(ccBLACK);
		_gameOverLayer->addChild(bestS);

		_bestScore = CCLabelBMFont::create("000","font.fnt");
		_bestScore->setPosition(ccp(SC_WIDTH()/2.0f , overHeight * 0.7f));
		_bestScore->setColor(ccBLACK);
		_gameOverLayer->addChild(_bestScore);
		_bestScore->retain();

		Button* restartB = Button::create(
			CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("restart.png"),
			this, menu_selector(GameScene::reStartCallBack)
			);
		restartB->setPosition(ccp(SC_WIDTH()/2.0f , overHeight * 0.5f));
		_gameOverLayer->addChild(restartB);

		Button* shareB = Button::create(
			CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("share.png"),
			this, menu_selector(GameScene::shareCallBack)
			);
		shareB->setPosition(ccp(SC_WIDTH()/2.0f , overHeight * 0.3f));
		_gameOverLayer->addChild(shareB);

		_gameOverLayer->setVisible(false);

		resetStage();
		this->scheduleUpdate();
		this->setKeypadEnabled(true);
		bRet = true;
	} while (0);
	return bRet;
}

cocos2d::CCScene* GameScene::scene()
{
	CCScene* scene = CCScene::create();
	GameScene* layer = GameScene::create();
	scene->addChild(layer);
	return scene;
}

void GameScene::resetStage()
{
	std::vector<int> allIndex;
	int count = _allBalls->count();
	for(int i = 0 ; i < count ; i++){
		CCSprite* sp = (CCSprite*)_allBalls->objectAtIndex(i);
		sp->stopAllActions();
		sp->setVisible(false);
		allIndex.push_back(i);
	}
	_actionBalls.clear();
	_currentIndex.clear();
	for(int i = 0 ; i < _currentSize ; i++){
		int leftIndex = allIndex.size();
		int nowIndex = RAND_INT_0_B1(leftIndex);
		_actionBalls.push_back(allIndex.at(nowIndex));
		allIndex.erase(allIndex.begin()+nowIndex);
		if(i < _currentOtherSizeCount){
			int dc = _currentColorSize - 2;
			int otherIndex = RAND_INT_0_B1(dc);
			_currentIndex.push_back(2+otherIndex);
		}else{
			_currentIndex.push_back(1);
		}
	}
	for(int i = 0 ; i < _currentSize ; i++){
		int ind = _actionBalls.at(i);
		CCSprite* sp = (CCSprite*)_allBalls->objectAtIndex(ind);
		sp->setColor(_currentColors.at(_currentIndex.at(i)));
		sp->setScale(0);
		sp->setVisible(true);
		sp->runAction(CCEaseBackOut::create(CCScaleTo::create(0.7f,1)));
		sp->setZOrder(i);
		if(i == 0 && _stage == 0 && Constant::isNeedTip && _tips){
			_tips->stopAllActions();
			_tips->setVisible(true);
			_tips->setColor(_currentColors.at(1));
			_tips->setPosition(sp->getPosition());
			_tips->runAction(CCRepeatForever::create(CCSequence::createWithTwoActions(
				CCEaseSineInOut::create(CCMoveBy::create(0.5f,ccp(0,-30))),
				CCEaseSineInOut::create(CCMoveBy::create(0.5f,ccp(0,30)))
				)));
			Constant::isNeedTip = false;
		}
	}
	scheduleOnce(schedule_selector(GameScene::moveEnd),0.5f);

	int flagCount = _colorflagBalls->count();
	CCSprite* tmp = (CCSprite*)_colorflagBalls->objectAtIndex(0);
	float scale = tmp->getScale();
	if(_currentColorSize - 1 > flagCount){
		float tempY = tmp->getPositionY();
		for(int i = 0 ; i < _currentColorSize-flagCount-1 ; i++){
			CCSprite* flag = CCSprite::createWithSpriteFrameName("circle.png");
			flag->setScale(scale);
			flag->setPosition(ccp(SC_WIDTH()/2.0f,tempY));
			_flagBn->addChild(flag);

			_colorflagBalls->addObject(flag);
		}
	}
	flagCount = _colorflagBalls->count();
	float dis = scale * 120.0f;
	float startX = SC_WIDTH()/2.0f - dis * (_currentColorSize / 2.0f - 1);
	for(int i = 0 ; i < flagCount ; i++){
		CCSprite* sp1 = (CCSprite*)_colorflagBalls->objectAtIndex(i);
		if(i < _currentColorSize -1){
			sp1->setVisible(true);
			sp1->setPositionX(startX+i*dis);
			sp1->setColor(_currentColors.at(i+1));
		}else{
			sp1->setVisible(false);
		}
	}
}

void GameScene::ballSelectCallBack( cocos2d::CCObject* pSender )
{
	if(!_isCanAction){
		return;
	}
	if(!_isStart){
		_isStart = true;
		if(_tips){
			_tips->removeFromParentAndCleanup(true);
			CC_SAFE_RELEASE_NULL(_tips);
		}
	}
	MusicManager::playSound(SOUND_CLICK);

	Button* bt = (Button*)pSender;
	int index = bt->getZOrder();
	int ci = _currentIndex.at(index);
	if(ci < _currentColorSize -1){
		ci++;
	}else{
		ci = 1;
	}
	_currentIndex.at(index) = ci;
	bt->setColor(_currentColors.at(ci));
	bt->stopAllActions();
	bt->setScale(1);
	bt->runAction(CCSequence::create(
		CCEaseSineOut::create(CCScaleTo::create(0.2f,0.8f)),
		CCEaseSineInOut::create(CCScaleTo::create(0.3f,1.1f)),
		CCEaseSineIn::create(CCScaleTo::create(0.1f,1)),
		NULL));
	Constant::addTime(CLICK_ADD_TIME);

	char s[100] = {0};
	sprintf(s, "%d" , Constant::addScore( 1 ));
	_score->setString(s);

	checkAllColor();
}

void GameScene::checkAllColor()
{
	bool isClear = true;
	int nowC = _currentIndex.at(0);
	for(int i = 1 ; i < _currentSize ; i++){
		if(nowC != _currentIndex.at(i)){
			isClear = false;
			break;
		}
	}

	if(isClear){
		char s[100] = {0};
		sprintf(s, "%d" , Constant::addScore( (_currentSize-2)*10*(_currentColorSize-2) ));
		_score->setString(s);


		_stage++;
		ccColor3B nowcolor = _currentColors.at(nowC);
		resetWithStage();

		_bgColor->runAction(CCSequence::createWithTwoActions(
			CCTintTo::create(0.5f,nowcolor.r,nowcolor.g,nowcolor.b),
			CCCallFunc::create(this,callfunc_selector(GameScene::resetStage))
			));
		_isCanAction = false;

		Constant::addTime(LEVEL_ADD_TIME);
		float now = _timeProgress->getPercentage();
		_timeProgress->runAction(CCProgressFromTo::create(0.6f,now,Constant::getTimePercent()*100.0f));

		MusicManager::playSound(SOUND_CHANGE);
	}
}

void GameScene::moveEnd( float dt )
{
	_isCanAction = true;
}

void GameScene::update( float dt )
{
	if(_isStart){
		if(_isCanAction){
			bool isContiune = Constant::passTime(dt);
			_timeProgress->setPercentage(Constant::getTimePercent()*100.0f);
			if(!isContiune){
				Constant::gameover();
				_isStart = false;
				_isCanAction = false;
				char s[100] = {0};
				sprintf(s , "%d",Constant::getBestScore());
				_bestScore->setString(s);
				float hei = _gameOverLayer->getContentSize().height;
				_gameOverLayer->setPositionY(-hei);
				_gameOverLayer->setVisible(true);
				_gameOverLayer->runAction(CCEaseSineOut::create(CCMoveTo::create(0.6f,CCPointZero)));

				Constant::playTimes++;
				if(Constant::playTimes%5 == 0){
					Utils::showAd();
				}
			}
		}
	}
}

void GameScene::reStartCallBack( cocos2d::CCObject* pSender )
{
	MusicManager::playSound(SOUND_CLICK);
	float hei = _gameOverLayer->getContentSize().height;
	_gameOverLayer->stopAllActions();
	_gameOverLayer->runAction(CCSequence::createWithTwoActions(
		CCEaseSineIn::create(CCMoveTo::create(0.6f,ccp(0,-hei))),
		CCCallFunc::create(this,callfunc_selector(GameScene::restartGame))
		));
}

void GameScene::shareCallBack( cocos2d::CCObject* pSender )
{
	MusicManager::playSound(SOUND_CLICK);

	char s[512] = {0};
	CCDirector::sharedDirector()->pause();
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	CCScene* scene = CCDirector::sharedDirector()->getRunningScene();
	CCRenderTexture* re = CCRenderTexture::create(size.width,size.height);
	re->setPosition(size.width / 2.0f , size.height / 2.0f);
	scene->addChild(re , -10);
	re->begin();
	this->visit();
	re->end();
	sprintf(s , "%sImage%lld.jpg",Constant::savePath.c_str(),getMillSec());
	re->saveToFile(s);
	re->removeFromParentAndCleanup(true);
	CCDirector::sharedDirector()->resume();

	CCLayerColor* f = CCLayerColor::create(ccc4(255,255,255,150));
	f->setContentSize(size);
	f->setPosition(size.width / 2.0f , size.height / 2.0f);
	f->ignoreAnchorPointForPosition(false);
	this->addChild(f,100);
	f->runAction(CCEaseExponentialOut::create(CCFadeOut::create(0.5f)));

	Utils::share(s);
}

void GameScene::resetWithStage( bool isRandom /*= false*/ )
{
	ccColor3B startColor = ccWHITE;
	if(!isRandom){
		startColor = _currentColors.at(_currentIndex.at(0));
	}
	_currentColorSize = (_stage / 23) + 3;
	if(_currentColorSize > 10){
		_currentColorSize = 10;
	}
	_currentSize = (_stage%23) +3;
	_currentOtherSizeCount = 1+ ((int)(_currentSize*0.5f))*CCRANDOM_0_1();
	_currentColors = Utils::getDiffColor(_currentColorSize,startColor,isRandom);


}

void GameScene::restartGame()
{
	_stage = 0;
	resetWithStage();
	Constant::reset();
	_score->setString("0");
	_timeProgress->stopAllActions();
	float p = _timeProgress->getPercentage();
	_timeProgress->runAction(CCProgressFromTo::create(0.7f,p,Constant::getTimePercent()* 100.0f));
	resetStage();
}

void GameScene::keyBackClicked()
{
	CCDirector::sharedDirector()->end();
}
