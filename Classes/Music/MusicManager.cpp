#include "MusicManager.h"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

void MusicManager::initAll()
{
	SimpleAudioEngine::sharedEngine()->preloadEffect(SOUND_CHANGE);
	SimpleAudioEngine::sharedEngine()->preloadEffect(SOUND_CLICK);
}

void MusicManager::playSound( std::string sound )
{
	SimpleAudioEngine::sharedEngine()->playEffect(sound.c_str());
}
