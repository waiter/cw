#ifndef __CONSTANT_H__
#define __CONSTANT_H__
#include "cocos2d.h"
#define T_MAX_TIME			10.0f
#define CLICK_ADD_TIME		0.1f
#define LEVEL_ADD_TIME		1.0f
#define T_START_TIME			5.0f

#define BEST_SCORE			"bs"
class Constant
{
public:
	static void init();

	static void reset();

	static int addScore(int score);
	static int getScore();

	static int getBestScore();

	static bool passTime(float dt);
	static float getTimePercent();
	static float addTime(float dt);

	static void gameover();

	static void saveScore();

	static bool isNeedTip;

	static std::string savePath;
	static int playTimes;
private:
	static int _bestScore;
	static float _currentTime;
	static int _currentScore;
};
#endif //__CONSTANT_H__