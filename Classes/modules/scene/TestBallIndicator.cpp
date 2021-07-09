#include "TestBallIndicator.h"
#include <modules/scene/UITestFactory.h>
#include <data/Resource.h>

USING_NS_CC;
using namespace ps;

TestBallIndicator::TestBallIndicator() {
		

}

bool TestBallIndicator::init() {
	if (!Node::init()) {
		return false;
	}

	this->initBall();
	this->initIndicator();
	this->initAngle();

	return true;
}

void TestBallIndicator::onEnter() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Node::onEnter();
	this->initListener();
	auto size = this->ball->getContentSize();
	this->setPosition(size.width / 2, visibleSize.height - size.height / 2);
}

void TestBallIndicator::initBall() {
	auto ball = UITestFactory::commonSprite(Res::INGAME_BALL_BUTTON.c_str());
	this->addChild(ball);
	this->ball = ball;
	this->ballRect = Rect(
		-this->ball->getContentSize().width / 2, -this->ball->getContentSize().height / 2,
		this->ball->getContentSize().width, this->ball->getContentSize().height
	);
	this->rMax = this->ball->getContentSize().width / 2 - 20;
	this->rSquare = this->rMax * this->rMax;
	this->ballRSquare = this->ball->getContentSize().width * this->ball->getContentSize().width;
}

void TestBallIndicator::initIndicator() {
	auto indicator = UITestFactory::commonSprite(Res::INGAME_BALL_TOUCH_INDICATOR.c_str());
	this->addChild(indicator);
	this->indicator = indicator;
}

void TestBallIndicator::initListener() {
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(TestBallIndicator::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(TestBallIndicator::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(TestBallIndicator::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

bool  TestBallIndicator::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	auto position = this->convertToNodeSpace(touch->getLocation());
	if (this->ballRect.containsPoint(position)) {
		auto dot = position.x * position.x + position.y * position.y;
		if (dot <= this->ballRSquare) {
			if (dot > this->rSquare) {
				auto length = sqrt(dot);
				position.x = position.x * this->rMax / length;
				position.y = position.y * this->rMax / length;
			}
			this->setIndicatorPosition(position);
		}
		return true;
	}
	return false;
}

void TestBallIndicator::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event) {
	auto position = this->convertToNodeSpace(touch->getLocation());
	auto dot = position.x * position.x + position.y * position.y;
	if (dot > this->rSquare) {
		auto length = sqrt(dot);
		position.x = position.x * this->rMax / length;
		position.y = position.y * this->rMax / length;
	}
	this->setIndicatorPosition(position);
}

void TestBallIndicator::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	auto position = this->convertToNodeSpace(touch->getLocation());
	auto dot = position.x * position.x + position.y * position.y;
	if (dot > this->rSquare) {
		auto length = sqrt(dot);
		position.x = position.x * this->rMax / length;
		position.y = position.y * this->rMax / length;
	}
	this->setIndicatorPosition(position);
}

void TestBallIndicator::setIndicatorPosition(cocos2d::Vec2 position) {
	this->indicator->setPosition(position);
	this->delegate(ps::ExtMath::vector(position.x/ this->rMax, position.y/ this->rMax));
}

void TestBallIndicator::setDelegate(BallIndicatorCallback &delegate) {
	this->delegate = delegate;
}

void TestBallIndicator::initAngle() {
	auto position = this->indicator->getPosition();

	auto text = UITestFactory::commonText("0d", position.x, position.y - this->indicator->getContentSize().height - 50, 20);
	this->addChild(text);
	this->angleText = text;
}
