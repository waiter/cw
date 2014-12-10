#include "Constant.h"
USING_NS_CC;

void Constant::init()
{
	_bestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey(BEST_SCORE,-3);
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

int Constant::playTimes = 0;

std::string Constant::savePath = "";

bool Constant::isNeedTip = true;

int Constant::_currentScore = 0;

int Constant::_bestScore = 0;

float Constant::_currentTime = 0;
