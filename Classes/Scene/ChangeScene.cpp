#include "ChangeScene.h"
#include "Constant\Constant.h"
#include "MainScene.h"
USING_NS_CC;

ChangeScene::ChangeScene()
	:_bg(NULL)
	,_toLa(0)
{

}

ChangeScene::~ChangeScene()
{
	CC_SAFE_RELEASE_NULL(_bg);
}

bool ChangeScene::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		_bg = CCLayerColor::create(ccc4(0,0,0,255));
		addChild(_bg);
		_bg->retain();
		_color = ccBLACK;

		bRet = true;
	} while (0);
	return bRet;
}

void ChangeScene::setBgColor( cocos2d::ccColor3B color )
{
	_color = color;
	_bg->setColor(_color);
}

void ChangeScene::setChangeToLa( int la )
{
	_toLa = la;
}

void ChangeScene::startChange()
{
	this->scheduleOnce(schedule_selector(ChangeScene::change),0.1f);
}

void ChangeScene::change( float dt )
{
	char s[100] = {0};
	sprintf(s ,"la_%s.plist",Constant::getCurrentLaStr().c_str() );
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile(s);
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	Constant::setToNewLa(_toLa);
	sprintf(s ,"la_%s.plist",Constant::getCurrentLaStr().c_str() );
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(s);
	CCScene* scene = CCScene::create();
	MainScene* m = MainScene::create();
	scene->addChild(m);
	CCDirector::sharedDirector()->replaceScene(scene);
	m->showBigFrame(true,_color);
}
