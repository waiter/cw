#include "Utils.h"
#include "GlobleDefine.h"
#include "Constant/Constant.h"
USING_NS_CC;
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#define  LOG_TAG    "UtilJNI"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#define  CLASS_NAME "com/colortheworld/colorwar/j"
#else
#endif

std::vector<cocos2d::ccColor3B> Utils::getDiffColor( int size , cocos2d::ccColor3B color , bool isRandom /*= true*/ )
{
	std::vector<cocos2d::ccColor3B> re;
	if(size > 0){
		ccColor3B beginColor = color;
		if(isRandom){
			beginColor = ccc3(CCRANDOM_0_1()*255.0f,CCRANDOM_0_1()*255.0f,CCRANDOM_0_1()*255.0f);
		}
		size--;
		re.push_back(beginColor);
		int diff3[3] = {
			_diffColorDis[RAND_INT_0_B1(4)]*255.0f,
			_diffColorDis[RAND_INT_0_B1(4)]*255.0f,
			_diffColorDis[RAND_INT_0_B1(4)]*255.0f
		};
		ccColor3B nowC = beginColor;
		while(size > 0){
			nowC = ccc3(
				(nowC.r+diff3[0])%256,
				(nowC.g+diff3[1])%256,
				(nowC.b+diff3[2])%256
				);
			re.push_back(nowC);
			size--;
		}
	}
	return re;
}

float Utils::_diffColorDis[4] = {3.0f/11.0f,5.0f/11.0f,7.0f/11.0f,9.0f/11.0f};

void Utils::share( std::string imageUrl )
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo,CLASS_NAME, "share", "(Ljava/lang/String;)V"))
	{
		return;
	}
	jstring stringArg1 = methodInfo.env->NewStringUTF(imageUrl.c_str());
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID,stringArg1);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#else
	CCLOG("share: %s",imageUrl.c_str());
#endif
}

void Utils::showAd()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo,CLASS_NAME, "showAd", "()V"))
	{
		return;
	}
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#else
	CCLOG("showAd");
#endif
}

void Utils::toNoAd()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS

#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	JniMethodInfo methodInfo;
	if (! JniHelper::getStaticMethodInfo(methodInfo,CLASS_NAME, "toPaid", "()V"))
	{
		return;
	}
	methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
	methodInfo.env->DeleteLocalRef(methodInfo.classID);
#else
	CCLOG("toNoAd");
	Constant::noAd();
#endif
}

extern "C"
{
	long long getMillSec()
	{
		long long nMillSec = 0;  

		struct timeval tv;
		gettimeofday(&tv,NULL);

		nMillSec = (long long)tv.tv_sec * 1000; 
		nMillSec += tv.tv_usec / 1000;    

		return nMillSec; 
	}

	int getGMTimeZone()
	{
		struct timeval tv;
		struct timezone tz;
		gettimeofday (&tv , &tz);
		int timeZ = (tz.tz_minuteswest/60);
		if (timeZ*60<tz.tz_minuteswest)
		{
			timeZ++;
		}else if (timeZ*60>tz.tz_minuteswest)
		{
			timeZ--;
		}
		return timeZ;
	}
};

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
extern "C"
{
	void Java_com_colortheworld_colorwar_j_nsp1(JNIEnv*  env, jobject thiz , jstring path){
		std::string stemp = JniHelper::jstring2string(path);

		Constant::savePath = stemp;
	}
	void Java_com_colortheworld_colorwar_j_nsa(JNIEnv*  env, jobject thiz , jboolean s){
		Constant::isShowAd = s;
	}
	void Java_com_colortheworld_colorwar_j_nna(JNIEnv*  env, jobject thiz){
		Constant::noAd();
	}
}
#endif