#include "UITestFactory.h"

USING_NS_CC;

cocos2d::ui::Slider* UITestFactory::createSlider()
{
	auto slider = ui::Slider::create();
	slider->setRotation(-90);
	slider->setTouchEnabled(true);
	slider->loadBarTexture("res/ui/sliderTrack.png");
	slider->loadSlidBallTextures("res/ui/sliderThumb.png", "res/ui/sliderThumb.png", "");
	slider->loadProgressBarTexture("res/ui/sliderProgress.png");
	return slider;
}

cocos2d::ui::Text* UITestFactory::commonText(const char* text, double x, double y, int size)
{
	auto _lb = ui::Text::create();
	_lb->setString(text);
	_lb->setFontSize(size);
	_lb->setPosition(Vec2(x,y));
    _lb->setColor(Color3B(220, 220, 220));
    return _lb;
}
