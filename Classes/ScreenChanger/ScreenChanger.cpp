#include "ScreenChanger.h"
#include "CCEGLView.h"
USING_NS_CC;


float ScreenChanger::offsetY = 0;
float ScreenChanger::offsetX = 0;
float ScreenChanger::screenWidth = 0;
float ScreenChanger::screenHeight = 0;

const CCPoint ScreenChanger::scp(float _width, float _height) {
	CCPoint ret;
	ret = ccp(_width+offsetX, _height+offsetY);
	
	return ret;
}

float ScreenChanger::scX(float c_value){
	CCPoint p = scp(c_value, 0);
	return p.x;
}

float ScreenChanger::scY(float c_value){
	CCPoint p = scp(0, c_value);
	return p.y;
}

void ScreenChanger::Init(){
	CCSize size = CCDirector::sharedDirector()->getWinSize();
#if SCREEN_ORIENTATION == SCREEN_LANDSCAPE
	float rate = 1.0f * size.width / size.height;
#else
	float rate = 1.0f * size.height / size.width;
#endif
	float tempWidth = 960;
	float tempHeight = 768;
	if(rate >= 1.775f - 0.00001f){
		tempWidth = 1136;
		tempHeight = 640;
	}else if(rate >= 1.5f - 0.00001f){
		tempWidth = 1136;
		tempHeight = 1136.0f / rate;
	}else if(rate >= 1.25f - 0.00001f){
		tempWidth = 768 * rate;
		tempHeight = 768;
	}
	float tempOffsetX = (tempWidth - 960.0f) / 2.0f;
	float tempOffsetY = (tempHeight - 640.0f) / 2.0f;
	tempOffsetX = tempOffsetX >= 0 ? tempOffsetX : 0;
	tempOffsetY = tempOffsetY >= 0 ? tempOffsetY : 0;

	float minSX = tempWidth / 960.0f;
	float minSY = tempWidth / 640.0f;
	minScale = minSX < minSY ? minSX : minSY;
	if(minScale < 1.0f)
		minScale = 1.0f;

#if SCREEN_ORIENTATION == SCREEN_LANDSCAPE
	screenWidth = tempWidth;
	screenHeight = tempHeight;
	offsetX = tempOffsetX;
	offsetY = tempOffsetY;
#else
	screenWidth = tempHeight;
	screenHeight = tempWidth;
	offsetX = tempOffsetY;
	offsetY = tempOffsetX;
#endif
	//CCLog("%f %f  %f  %f",screenWidth,screenHeight,offsetX,offsetY);
	CCEGLView::sharedOpenGLView()->setDesignResolutionSize(screenWidth,screenHeight,kResolutionExactFit);
}

float ScreenChanger::getOffsetX()
{
	return offsetX;
}

float ScreenChanger::getOffsetY()
{
	return offsetY;
}

float ScreenChanger::scPX( float rate )
{
	if(rate < 0){
		rate = 0;
	}
	if(rate > 1){
		rate = 1;
	}
	return screenWidth * rate;
}

float ScreenChanger::scPY( float rate )
{
	if(rate < 0){
		rate = 0;
	}
	if(rate > 1){
		rate = 1;
	}
	return screenHeight * rate;
}

const cocos2d::CCPoint ScreenChanger::scpp( float rateX , float rateY )
{
	return ccp(scPX(rateX) , scPY(rateY));
}

float ScreenChanger::getScreenWidth()
{
	return screenWidth;
}

float ScreenChanger::getScreenHeight()
{
	return screenHeight;
}

float ScreenChanger::getMinScale()
{
	return minScale;
}

float ScreenChanger::minScale = 1.0f;




