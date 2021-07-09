#ifndef 	_TESTBALLINDICATOR_H_
#define 	_TESTBALLINDICATOR_H_


#include <cocos2d.h>
#include <modules/core/ExtensionMath.hpp>
#include <ui/CocosGUI.h>

typedef std::function<void(ps::ExtMath::vector)> BallIndicatorCallback;

class TestBallIndicator: public cocos2d::Node{
public:
	TestBallIndicator();
	bool init();
	void onEnter();
	void initBall();
	void initIndicator();
	void initListener();
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
	void setIndicatorPosition(cocos2d::Vec2 position);
	void setDelegate(BallIndicatorCallback &func);
	void initAngle();
	CREATE_FUNC(TestBallIndicator);


private:
	cocos2d::Sprite* ball;
	cocos2d::Rect ballRect;
	int rMax;
	int rSquare;
	int ballRSquare;
	cocos2d::Sprite* indicator;
	int touchListener;
	std::function<void(ps::ExtMath::vector)> delegate;
	cocos2d::ui::Text* angleText;
};
#endif