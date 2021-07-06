#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include <cocos2d.h>
#include "ui/CocosGUI.h"


class GameScene: public cocos2d::Scene {
public:
	cocos2d::Camera* _camera;

	static cocos2d::Scene* createScene();

	virtual bool init();
	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
	void touchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif // !GAMESCENE_H_
