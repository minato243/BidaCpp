#include "UITestLayer.h"
#include <modules/scene/UITestFactory.h>

USING_NS_CC;
using namespace cocos2d::ui;

UITestLayer::UITestLayer() {

}

bool UITestLayer::init()
{
	CCLOG("UITestLayer::init");
	if (!Layer::init()) {
		return false;
	}

	this->maxForce = 400;
	this->initPowerSlider();
	this->initcueMovingSpeedSlider();
	this->initBallIndicator();
	this->initTimescaleSlider();

	return true;
}

void UITestLayer::initPowerSlider() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto slider = UITestFactory::createSlider();
	slider->setTag(UITestLayer::POWER_SLIDER_TAG);
	auto pos = Vec2(visibleSize.width - 30, visibleSize.height / 2 - 20);
	slider->setPosition(pos);
	slider->setPercent(65);
	slider->addEventListener(CC_CALLBACK_2(UITestLayer::sliderEvent, this));
	this->addChild(slider);
	this->powerSlider = slider;

	char percentStr[50];
	sprintf(percentStr, "%d%%", slider->getPercent());
	auto textPercent = UITestFactory::commonText(percentStr, slider->getPosition().x, slider->getPosition().y - 140, 20);
	this->addChild(textPercent);
	this->powerPercentText = textPercent;
	sprintf(percentStr, "%d", slider->getPercent() * this->maxForce / 100);
	auto text = UITestFactory::commonText( percentStr,  slider->getPosition().x,  slider->getPosition().y + 140, 20);
	this->addChild(text);
	this->powerText = text;

	auto label = UITestFactory::commonText("Force",  slider->getPosition().x, textPercent->getPosition().y - 20, 15);
	this->addChild(label);
}

void UITestLayer::initTimescaleSlider() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto slider = UITestFactory::createSlider();
	slider->setRotation(0);
	slider->setTag(UITestLayer::TIMESCALE_SLIDER_TAG);
	slider->setPosition(Vec2(visibleSize.width / 2, 20));
	slider->setPercent(100);
	slider->addEventListener(CC_CALLBACK_2(UITestLayer::sliderEvent, this));
	this->addChild(slider);
	this->timescaleSlider = slider;

	char str[50];
	sprintf(str, "Timescale: %d%%", slider->getPercent());
	auto text = UITestFactory::commonText(str,  slider->getPosition().x,  slider->getPosition().y + 20, 15);
	this->addChild(text);
	this->timescaleText = text;
}

void UITestLayer::initcueMovingSpeedSlider() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto slider = UITestFactory::createSlider();
	slider->setTag(UITestLayer::CUE_SPEED_SLIDER_TAG);
	slider->setPosition(Vec2(30, visibleSize.height / 2 - 20));
	slider->setPercent(50);
	slider->addEventListener(CC_CALLBACK_2(UITestLayer::sliderEvent, this));
	this->addChild(slider);
	this->cueSlider = slider;

	char str[20];
	sprintf(str, "%d%%", slider->getPercent());
	auto text = UITestFactory::commonText( str,  slider->getPosition().x,  slider->getPosition().y - 140, 20);
	this->addChild(text);
	this->cueText = text;

	auto label = UITestFactory::commonText("Cue\nSpeed",  slider->getPosition().x, text->getPositionY() - 30, 15);
	label->setTextHorizontalAlignment(TextHAlignment::CENTER);
	this->addChild(label);
}

void UITestLayer::initBallIndicator() {
	//auto ballIndicator = new TestBallIndicator(); // todo update later
	//ballIndicator.setDelegate({
	//	updateCueOffsetUnit: offsetUnit => this->delegate && this->delegate.updateCueOffsetUnit(offsetUnit)
	//})
	//this->addChild(ballIndicator);
	//this->ballIndicator = ballIndicator;
}

float UITestLayer::getPowerValue() {
	return this->powerSlider->getPercent();
}

float UITestLayer::getCueSpeedValue() {
	return this->cueSlider->getPercent();
}

void UITestLayer::setMaxPower(int force) {
	this->maxForce = force;
	this->powerText->setString(this->powerSlider->getPercent() * this->maxForce / 100 + "");
}

void UITestLayer::sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	int tag = ((Slider*)sender)->getTag();
	Slider* slider = (Slider*)sender;
	char str[50];
	switch (type) {
	case ui::Slider::EventType::ON_PERCENTAGE_CHANGED:
			switch (tag) {
				case UITestLayer::POWER_SLIDER_TAG:
					sprintf(str, "%d %%", slider->getPercent());
					this->powerPercentText->setString(str);
					sprintf(str, "%d", slider->getPercent() * this->maxForce / 100);
					this->powerText->setString(str);
					//this->parent->updateTableDirection(sender->getPercent() * this->maxForce / 100); todo later
					break;
				case UITestLayer::CUE_SPEED_SLIDER_TAG:
					sprintf(str, "%d %%", slider->getPercent());
					this->cueText->setString(str);
					break;
				case UITestLayer::TIMESCALE_SLIDER_TAG:
					//this->delegate && this->delegate->onTimeScaleChanged(slider->getPercent());
					sprintf(str, "Timescale: %d %%", slider->getPercent());
					this->timescaleText->setString(str);
					break;
				case UITestLayer::FOV_SLIDER:
					this->updateFoVValues();
					break;
			}
			break;
	}
}

void UITestLayer::setDelegate(int delegate) {
	this->delegate = delegate;
}

void UITestLayer::initFoVSliders() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto slider = UITestFactory::createSlider();
	slider->setTag(UITestLayer::FOV_SLIDER);
	slider->setRotation(0);
	slider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 10));
	slider->setPercent(45);
	slider->addEventListener(CC_CALLBACK_2(UITestLayer::sliderEvent, this));
	this->addChild(slider);
	this->fovSlider_1 = slider;

	slider = UITestFactory::createSlider();
	slider->setTag(UITestLayer::FOV_SLIDER);
	slider->setRotation(0);
	slider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 30));
	slider->setPercent(0);
	slider->addEventListener(CC_CALLBACK_2(UITestLayer::sliderEvent, this));
	this->addChild(slider);
	this->fovSlider_2 = slider;

	slider = UITestFactory::createSlider();
	slider->setTag(UITestLayer::FOV_SLIDER);
	slider->setRotation(0);
	slider->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 50));
	slider->setPercent(0);
	slider->addEventListener(CC_CALLBACK_2(UITestLayer::sliderEvent, this));
	this->addChild(slider);
	this->fovSlider_3 = slider;
}

void UITestLayer::updateFoVValues() {
	auto val_1 = this->fovSlider_1->getPercent();
	auto val_2 = this->fovSlider_2->getPercent();
	auto val_3 = this->fovSlider_3->getPercent();

	auto fov = val_1 + val_2 / 10 + val_3 / 100;
	//this->delegate->onChangeFoV(fov);
}
