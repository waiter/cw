#ifndef __UTILS_H__
#define __UTILS_H__
#include "cocos2d.h"
#include <vector>
class Utils
{
public:
	// size must smaller than 11
	static std::vector<cocos2d::ccColor3B> getDiffColor(int size , cocos2d::ccColor3B color  , bool isRandom = true);

	static void share(std::string imageUrl);
	static void showAd();
	static void toNoAd();
private:
	static float _diffColorDis[4];
};

extern "C"{
	extern long long getMillSec();
	extern int getGMTimeZone();
};
#endif //__UTILS_H__