#ifndef GAMESCENE_H_
#define GAMESCENE_H_

#include <cocos2d.h>
#include "ui/CocosGUI.h"
#include <modules/core-implement/Table.h>
#include <modules/core-implement/Ball.h>
#include <modules/core-implement/Cue.h>
#include <modules/camera-view/SphericalCamera.h>
#include "UITestLayer.h"


class GameScene: public cocos2d::Scene {
public:
	Table* _table;
	//cocos2d::Camera* _camera;
	Ball* _cueBall;
	Cue* _cue;
	SphericalCamera* _camera;

	static cocos2d::Scene* createScene();

	virtual bool init();
	void initBall();
	void initTable();
	void initCue();
	void initCamera();
	void initUI();
	void initListener();
	CREATE_FUNC(GameScene);
	void touchEvent(cocos2d::Ref* pSender, cocos2d::ui::Widget::TouchEventType type);
	void clickEvent(cocos2d::Ref* pSender);
	void resetEvent(cocos2d::Ref* pSender);
	void up(cocos2d::Ref* pSender);
	void down(cocos2d::Ref* pSender);
	void left(cocos2d::Ref* pSender);
	void right(cocos2d::Ref* pSender);
	void doShoot();
	void addButton(const char *text, cocos2d::Vec2 position, cocos2d::Size size, cocos2d::ui::Widget::ccWidgetClickCallback callback);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void rotationUpdate(float dt);
	void cueRotationUpdate(float dt);
	void updateCueOffset(ps::ExtMath::vector offset);

private:
	int rotateCount;
	int cueRotateCount;
	cocos2d::Vec3 rotateSign;
	cocos2d::Vec3 cueSign;
	double yaw = 0;
	double pitch = 0;
	UITestLayer* ui;

public:
	static cocos2d::Vec3 ROTATE_SIGN[250];
	static const double MAX_POWER;
};

#endif // !GAMESCENE_H_
