#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__
#include "cocos2d.h"
class Button;
class MainScene : public cocos2d::CCLayer
{
public:
	MainScene();
	~MainScene();
	bool init();
	CREATE_FUNC(MainScene);

	virtual void keyBackClicked();
	void showBigFrame(bool isOpen , cocos2d::ccColor3B color);

	static cocos2d::CCScene* scene();
private:
	int _needChangeToLa;
	cocos2d::ccColor3B _color;
	Button* _noAd;
	void startCallBack(cocos2d::CCObject* pSender);
	void noAdCallBack(cocos2d::CCObject* pSender);
	void flagCallBack(cocos2d::CCObject* pSender);

	void resetBalls(cocos2d::CCObject* ball);
	void changeLa();

	void notiNoAdCallBack(cocos2d::CCObject* pSender);
};
#endif //__MAIN_SCENE_H__