#include "GameScene.h"
#include <data/Resource.h>
#include <modules/core-implement/Table.h>
#include <modules/core-implement/Ball.h>

USING_NS_CC;
using namespace ui;

cocos2d::Scene* GameScene::createScene()
{
	return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //auto sprite3D = Sprite3D::create(Res::BALL_HI_POLY_2);
    //sprite3D->setScale(1);
    //sprite3D->setTexture("res/3d/ball/Ball_No1.png");
    //this->addChild(sprite3D);
    //sprite3D->setPosition3D(Vec3(0.0f, -15.0f, -50.0f));
    //sprite3D->setCameraMask(2);

    auto ball = Ball::create();
    ball->setNumber(2);
    this->addChild(ball);
    ball->setPosition3D(Vec3(0.0f, -15.0f, -50.0f));
    ball->setCameraMask(2);

    auto table = Table::create();
    this->addChild(table);

    //use custom camera
    auto camera = Camera::createPerspective(60, visibleSize.width / visibleSize.height, 0.1f, 200);
    camera->setCameraFlag(CameraFlag::USER1);
    this->setCameraMask(2);
    this->addChild(camera);
    this->_camera = camera;

    auto button = ui::Button::create("button_blue_1.png");
    button->setTitleText("UP");
    this->addChild(button);
    button->setPosition(Vec2(100,200));
    button->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEvent, this));
    return true;
}

void GameScene::touchEvent(Ref* pSender, Widget::TouchEventType type)
{
    switch (type)
    {
    case Widget::TouchEventType::BEGAN:
    {
        CCLOG("BEGAN");
        auto pos = this->_camera->getPosition3D();
        pos.y += 1;
        this->_camera->setPosition3D(pos);
        break;
    }

    case Widget::TouchEventType::MOVED:
    {
        auto pos = this->_camera->getPosition3D();
        pos.y += 1;
        this->_camera->setPosition3D(pos);
        CCLOG("MOVED");
        break;
    }

    case Widget::TouchEventType::ENDED:
    {

        CCLOG("ENDED");
    }
    break;

    case Widget::TouchEventType::CANCELED:
        break;

    default:
        break;
    }
}
