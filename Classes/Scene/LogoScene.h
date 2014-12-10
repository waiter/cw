#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__
#include "cocos2d.h"
class LogoScene : public cocos2d::CCLayer
{
public:
	bool init();
	CREATE_FUNC(LogoScene);
	static cocos2d::CCScene* scene();
private:
	void toGameScene();
	void loadAllRes(float dt);
};
#endif //__LOGO_SCENE_H__