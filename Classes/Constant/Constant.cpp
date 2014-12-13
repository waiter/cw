#include "Constant.h"
USING_NS_CC;

void Constant::init()
{
	_bestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(BEST_SCORE,-3);
	_currentLanguage = CCUserDefault::sharedUserDefault()->getIntegerForKey(CUR_LANGUAGE,0);
	if(_currentLanguage == laNone){
		_currentLanguage = laEnglish;
		ccLanguageType lt = CCApplication::sharedApplication()->getCurrentLanguage();
		switch(lt){
		case kLanguageChinese:
			_currentLanguage = laChinese;
			break;
		case kLanguageFrench:
			_currentLanguage = laFrench;
			break;
		case kLanguageItalian:
			_currentLanguage = laItalian;
			break;
		case kLanguageGerman:
			_currentLanguage = laGerman;
			break;
		case kLanguageRussian:
			_currentLanguage = laRussian;
			break;
		case kLanguageKorean:
			_currentLanguage = laKorean;
			break;
		case kLanguageJapanese:
			_currentLanguage = laJapanese;
			break;
		}
		CCUserDefault::sharedUserDefault()->setIntegerForKey(CUR_LANGUAGE,_currentLanguage);
		CCUserDefault::sharedUserDefault()->flush();
	}
}

void Constant::reset()
{
	_currentTime = T_START_TIME;
	_currentScore = -5;
}

int Constant::addScore( int score )
{
	_currentScore += score*3;
	return getScore();
}

int Constant::getScore()
{
	return (_currentScore + 5)/3;
}

int Constant::getBestScore()
{
	return (_bestScore+3)/5;
}

bool Constant::passTime( float dt )
{
	_currentTime -= dt;
	if(_currentTime < 0.0001f){
		_currentTime = 0;
		return false;
	}
	return true;
}

float Constant::getTimePercent()
{
	return _currentTime / T_MAX_TIME;
}

float Constant::addTime( float dt )
{
	_currentTime += dt;
	if(_currentTime >= T_MAX_TIME){
		_currentTime = T_MAX_TIME;
	}
	return getTimePercent();
}

void Constant::gameover()
{
	if(getBestScore() < getScore()){
		_bestScore = getScore() * 5 -3;
		saveScore();
	}
}

void Constant::saveScore()
{
	CCUserDefault::sharedUserDefault()->setIntegerForKey(BEST_SCORE,_bestScore);
	CCUserDefault::sharedUserDefault()->flush();
}

std::string Constant::getCurrentLaStr()
{
	if(_currentLanguage == laNone){
		_currentLanguage = laEnglish;
		CCUserDefault::sharedUserDefault()->setIntegerForKey(CUR_LANGUAGE,_currentLanguage);
		CCUserDefault::sharedUserDefault()->flush();
	}
	return getLaStr(_currentLanguage);
}

int Constant::getCurrentLa()
{
	return _currentLanguage;
}

std::string Constant::getLaStr( int la )
{
	switch(la){
	case laChinese:
		return "zh";
	case laJapanese:
		return "ja";
	case laKorean:
		return "ko";
	case laFrench:
		return "fr";
	case laGerman:
		return "de";
	case laItalian:
		return "it";
	case laRussian:
		return "ru";
	}
	return "en";
}

void Constant::setToNewLa( int la )
{
	if(la > 0 && la < 9){
		_currentLanguage = la;
		CCUserDefault::sharedUserDefault()->setIntegerForKey(CUR_LANGUAGE,_currentLanguage);
		CCUserDefault::sharedUserDefault()->flush();
	}
}

int Constant::_currentLanguage = laEnglish;

bool Constant::isShowAd = true;

int Constant::playTimes = 0;

std::string Constant::savePath = "";

bool Constant::isNeedTip = true;

int Constant::_currentScore = 0;

int Constant::_bestScore = 0;

float Constant::_currentTime = 0;
