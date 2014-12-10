#ifndef __SCREEN_CHANGER_H__
#define __SCREEN_CHANGER_H__
#include "cocos2d.h"

// like 960 * 640
#define SCREEN_LANDSCAPE	1
// like 640 * 960
#define SCREEN_PORTRAIT		2
#define SCREEN_ORIENTATION	SCREEN_PORTRAIT

#define SC_SCP(__X__,__Y__)			ScreenChanger::scp((float)(__X__), (float)(__Y__))
#define SC_SCPP(__X__,__Y__)		ScreenChanger::scpp((float)(__X__), (float)(__Y__))
#define SC_GETOFFSET_X()			ScreenChanger::getOffsetX()
#define SC_GETOFFSET_Y()			ScreenChanger::getOffsetY()
#define SC_WIDTH()					ScreenChanger::getScreenWidth()
#define SC_HEIGHT()					ScreenChanger::getScreenHeight()
#define SC_MIN_SCALE()				ScreenChanger::getMinScale()

class ScreenChanger
{
private:
	static float offsetX,offsetY;
	static float screenWidth,screenHeight;
	static float minScale;
public:
	static const cocos2d::CCPoint scp(float _width, float _height);
	static float scX(float c_value);
	static float scY(float c_value);
	// rate 0 - 1 return for ccp not scp
	static float scPX(float rate);
	// rate 0 - 1 return for ccp not scp
	static float scPY(float rate);
	// rateX 0 - 1 rateY 0 - 1
	static const cocos2d::CCPoint scpp(float rateX , float rateY);

    static void Init();
	static float getOffsetX();
	static float getOffsetY();
	static float getScreenWidth();
	static float getScreenHeight();
	static float getMinScale();
};

#endif
