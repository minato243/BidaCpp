#include "GameScene.h"
#include <data/Resource.h>
#include <modules/core-implement/Table.h>

USING_NS_CC;

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

    auto sprite3D = Sprite3D::create(Res::BALL_HI_POLY_2);
    sprite3D->setScale(1);
    sprite3D->setTexture("res/3d/ball/Ball_No1.png");
    this->addChild(sprite3D);
    sprite3D->setPosition3D(Vec3(0.0f, -15.0f, -50.0f));
    sprite3D->setCameraMask(2);

    auto table = Table::create();
    this->addChild(table);

    //use custom camera
    auto camera = Camera::createPerspective(60, visibleSize.width / visibleSize.height, 0.1f, 200);
    camera->setCameraFlag(CameraFlag::USER1);
    this->setCameraMask(2);
    this->addChild(camera);
    return true;
}
