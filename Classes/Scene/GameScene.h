#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__
#include "cocos2d.h"
class GameScene : public cocos2d::CCLayer
{
public:
	GameScene();
	~GameScene();
	bool init();
	CREATE_FUNC(GameScene);

	virtual void keyBackClicked();

	static cocos2d::CCScene* scene();
private:
	bool _isCanAction;
	int _currentColorSize;
	int _currentSize;
	int _currentOtherSizeCount;
	int _stage;
	bool _isStart;

	std::vector<cocos2d::ccColor3B> _currentColors;
	std::vector<int> _currentIndex;
	std::vector<int> _actionBalls;

	cocos2d::CCLayerColor* _bgColor;
	cocos2d::CCArray* _allBalls;
	cocos2d::CCSpriteBatchNode* _flagBn;
	cocos2d::CCArray* _colorflagBalls;
	cocos2d::CCProgressTimer* _timeProgress;
	cocos2d::CCLabelBMFont* _score;
	cocos2d::CCLayerColor* _gameOverLayer;
	cocos2d::CCLabelBMFont* _bestScore;
	cocos2d::CCSprite* _tips;

	void resetStage();
	void moveEnd(float dt);
	void ballSelectCallBack(cocos2d::CCObject* pSender);
	void checkAllColor();
	void update(float dt);
	void reStartCallBack(cocos2d::CCObject* pSender);
	void shareCallBack(cocos2d::CCObject* pSender);
	void resetWithStage(bool isRandom = false);
	void restartGame();
};
#endif //__GAME_SCENE_H__