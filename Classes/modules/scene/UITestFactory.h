#ifndef 	_UITestFactory_H_
#define 	_UITestFactory_H_

#include <cocos2d.h>
#include <cocos/ui/UIslider.h>
#include <cocos/ui/UIText.h>

class UITestFactory {
public:

	static cocos2d::ui::Slider *createSlider();
	static cocos2d::ui::Text* commonText(const char *text, double x = 0, double y = 0, int size = 30);
private:
};
#endif