#ifndef __CHANGE_SCENE_H__
#define __CHANGE_SCENE_H__
#include "cocos2d.h"
class ChangeScene : public cocos2d::CCLayer
{
public:
	ChangeScene();
	~ChangeScene();
	bool init();

	void setBgColor(cocos2d::ccColor3B color);
	void setChangeToLa(int la);
	void startChange();

	CREATE_FUNC(ChangeScene);
private:
	cocos2d::CCLayerColor* _bg;
	cocos2d::ccColor3B _color;
	int _toLa;

	void change(float dt);
};
#endif //__CHANGE_SCENE_H__