#include "main.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "ScreenChanger/DebugResolution.h"
#include "ScreenChanger/ScreenChanger.h"

USING_NS_CC;

#define USE_WIN32_CONSOLE

int APIENTRY _tWinMain(HINSTANCE hInstance,
                       HINSTANCE hPrevInstance,
                       LPTSTR    lpCmdLine,
                       int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


#ifdef USE_WIN32_CONSOLE
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

    // create the application instance
    AppDelegate app;
    CCEGLView* eglView = CCEGLView::sharedOpenGLView();
    eglView->setViewName("colorwar");

	float tempWidth = 960;
	float tempHeight = 640;
#if RESOLUTION_DEBUG_ON
#if (NOW_RESOLUTION == RESOLUTION_IPAD)
	tempWidth = 1024;
	tempHeight = 768;
#elif (NOW_RESOLUTION == RESOLUTION_IPAD_RETAIN)
	tempWidth = 2048;
	tempHeight = 1536;
#elif (NOW_RESOLUTION == RESOLUTION_IPHONE5)
	tempWidth = 1136;
	tempHeight = 640;
#elif (NOW_RESOLUTION == RESOLUTION_IOS_NO_RETAIN)
	tempWidth = 480;
	tempHeight = 320;
#elif (NOW_RESOLUTION == RESOLUTION_ANDROID_UNKONW)
	tempWidth = RESOLUTION_ANDROID_UNKONW_WIDTH;
	tempHeight = RESOLUTION_ANDROID_UNKONW_HEIGHT;
#endif
#endif
	float zr = 1.0f;
#if SCREEN_ORIENTATION == SCREEN_LANDSCAPE
	eglView->setFrameSize(tempWidth, tempHeight);
	if(tempHeight > 960){
		zr = 960.0f / tempHeight;
	}
#else
	eglView->setFrameSize(tempHeight, tempWidth);
	if(tempWidth > 960){
		zr = 960.0f / tempWidth;
	}
#endif
	eglView->setFrameZoomFactor(zr);
	int ret = CCApplication::sharedApplication()->run();

#ifdef USE_WIN32_CONSOLE
	FreeConsole();
#endif

	return ret;
}
