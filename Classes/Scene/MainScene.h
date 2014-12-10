#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__
#include "cocos2d.h"
class MainScene : public cocos2d::CCLayer
{
public:
	MainScene();
	~MainScene();
	bool init();
	CREATE_FUNC(MainScene);

	virtual void keyBackClicked();

	static cocos2d::CCScene* scene();
private:

	void startCallBack(cocos2d::CCObject* pSender);

	void resetBalls(cocos2d::CCObject* ball);
};
#endif //__MAIN_SCENE_H__