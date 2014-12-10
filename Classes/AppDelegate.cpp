#include "AppDelegate.h"
#include "ScreenChanger/ScreenChanger.h"
#include "SimpleAudioEngine.h"
#include "Scene/LogoScene.h"
#include "Constant/Constant.h"
USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
	
}

bool AppDelegate::applicationDidFinishLaunching() {
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();
	pDirector->setOpenGLView(pEGLView);
	ScreenChanger::Init();

	if(Constant::savePath.empty()){
		Constant::savePath = CCFileUtils::sharedFileUtils()->getWritablePath();
	}

//#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
//	if(GlobalData::packageName.compare("com.feelingtouch.quizgame") != 0){
//		CCLog("Invaild Package!!");
//		CCDirector::sharedDirector()->end();
//	}
//#endif
//	pDirector->setDisplayStats(true);
   CCScene *pScene = LogoScene::scene();
	pDirector->runWithScene(pScene);

	//std::string pk = "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAhKYPyzK8gVvLY1g2JivITArKxaZZnijCOmsfppu+EoFFTv8MezHMm37QCtZmgHAlngexw2q9Q3KLxhc3buHU9lxWOWWwVFqOn1X7fsalkGKVo9zJ04ArcZ+Grt1sSn5vb6OaDi2xyyFNULFHsbt7jzdnjPkXpy4SguBHF30bJQGMUopzbaox3yQsFh7V5VROIWHeafJAcChpEnRhtpBxElVJ3kl0eSIy/pQlGh10E1So37a89ZUX3pVSwW65DNZ0mIN+19+wOokjPFli0aNBAU4/6URX+jU1K/dV5GmwR/PkJCQ4o10LlkSDbx+rqMCiGASdWOEodDI3zLIBu5h5GQIDAQAB";
	//Util::encodePublicKey(pk);
	//std::vector<std::string> tem;
	//for(int i = 0 ; i < 5 ; i++){
	//	tem.push_back(GlobalData::pkey[i]);
	//}
	//std::string dpk = Util::decodePublicKey(tem);
	//CCLog(dpk.c_str());
	//if(dpk.compare(pk) == 0){
	//	CCLog("true");
	//}else{
	//	CCLog("false");
	//}

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();

}
