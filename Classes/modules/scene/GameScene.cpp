#include "GameScene.h"
#include <data/Resource.h>
#include <modules/core-implement/Table.h>
#include <modules/core-implement/Ball.h>
#include <modules/core/PhysicsConstants.hpp>
#include <modules/core-implement/Cue.h>
#include <data/GlobalVar.h>
#include <modules/core/utils/LerpUtils.h>
#include <modules/camera-view/SphericalCamera.h>
#include <cmath>

USING_NS_CC;
using namespace ui;
using namespace ps;

cocos2d::Vec3 GameScene::ROTATE_SIGN[250];

cocos2d::Scene* GameScene::createScene()
{
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_A] = Vec3(1, 0, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_D] = Vec3(-1, 0, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_W] = Vec3(0, 1, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_S] = Vec3(0, -1, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_UP_ARROW] = Vec3(0, 1, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_DOWN_ARROW] = Vec3(0, -1, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_LEFT_ARROW] = Vec3(1, 0, 0);
    GameScene::ROTATE_SIGN[(int)EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = Vec3(-1, 0, 0);
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

    this->initTable();
    this->initBall();
    this->initCamera();
    this->initListener();
    this->initCue();
    this->initUI();

    this->addButton("Shoot", Vec2(300, 200), Size(100, 54), CC_CALLBACK_1(GameScene::clickEvent, this));
    this->addButton("Reset", Vec2(500, 200), Size(100, 54), CC_CALLBACK_1(GameScene::resetEvent, this));
    return true;
}

void GameScene::initTable() {
    auto table = Table::create();
    table->setCameraMask(2);
    this->addChild(table);
    this->_table = table;
}

void GameScene::initCue()
{
    auto cue = Cue::create();
    cue->setCameraMask(2);
    this->_table->addChild(cue);
    cue->updateCueRotate(vector(1, 1, 0));
    this->_cue = cue;
}

void GameScene::initCamera()
{
    //auto visibleSize = Director::getInstance()->getVisibleSize();
    ////use custom camera
    //auto camera = Camera::createPerspective(35, visibleSize.width / visibleSize.height, 0.1f, 10000);
    //camera->setCameraFlag(CameraFlag::USER1);
    //this->setCameraMask(2);
    //this->addChild(camera);
    //this->_camera = camera;
    //auto position = this->_camera->getPosition3D();
    //camera->setPosition3D(Vec3(0, 350, 350));
    //camera->lookAt(Vec3(0, 0, 0));
    this->setCameraMask(2);
    auto camera = SphericalCamera::create();
    camera->setPosition3D(Vec3(0, 350, 350));
    this->addChild(camera);
    this->_camera = camera;
}

void GameScene::initUI()
{
    this->ui = UITestLayer::create();
    this->addChild(this->ui);
}

void GameScene::initListener()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::initBall() {
    auto cueBall = Ball::createBall(0);
    cueBall->setCameraMask(2);
    this->_table->addBall(cueBall, PhysicsConstants::CUE_BALL_POSITION);
    this->_cueBall = cueBall;

    for (int i = 0; i < NUMBER_BALL; i++) {
        auto ball = Ball::createBall(i + 1);
        ball->setNumber(i + 1);
        auto pos = ps::PhysicsConstants::INIT_BALL_POSITIONS[i];
        ball->setCameraMask(2);
        this->_table->addBall(ball, pos);
    }
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

void GameScene::clickEvent(cocos2d::Ref* pSender)
{
    CCLOG("clickEvent");

    auto force = rand() % 500;
    auto direction = vector::normalize(vector(rand(), rand()));
    auto offset = vector::normalize(vector(rand(), rand()));
    this->_table->_world->shoot(0, 0, 300, direction, offset);
    std::function<void()> callback = std::bind(&GameScene::doShoot, this);
    //this->_cue->doShootAnimation(300., callback);
}

void GameScene::resetEvent(cocos2d::Ref* pSender)
{
    this->_table->reset();
    CCLOG("clickEvent");

}

void GameScene::up(cocos2d::Ref* pSender)
{
    auto pos = this->_camera->getPosition3D();
    pos.y += 1;
    this->_camera->setPosition3D(pos);
}

void GameScene::down(cocos2d::Ref* pSender)
{
    auto pos = this->_camera->getPosition3D();
    pos.y += -1;
    this->_camera->setPosition3D(pos);
}

void GameScene::left(cocos2d::Ref* pSender)
{
    auto pos = this->_camera->getPosition3D();
    pos.x -= 1;
    this->_camera->setPosition3D(pos);
}

void GameScene::right(cocos2d::Ref* pSender)
{
    auto pos = this->_camera->getPosition3D();
    pos.x += 1;
    this->_camera->setPosition3D(pos);
}

void GameScene::doShoot()
{
    CCLOG("doShoot");
    auto force =PhysicsConstants::MAX_CUE_OFFSET * this->ui->getPowerValue() /100;
    auto direction = vector::normalize(vector(rand(), rand()));
    auto offset = vector::normalize(vector(rand(), rand()));
    this->_table->_world->shoot(0, 0, force, direction, offset);
}

void GameScene::addButton(const char* text, cocos2d::Vec2 position, cocos2d::Size size, cocos2d::ui::Widget::ccWidgetClickCallback  callback)
{
    auto button = ui::Button::create("button_blue_1.png");
    auto rect = Rect(0, 0, 200, 80);
    button->setTitleText(text);
    this->addChild(button);
    button->setPosition(position);
    button->addClickEventListener(callback);
    button->setScale9Enabled(true);
    button->setCapInsets(rect);
    button->setContentSize(size);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_D:
    case EventKeyboard::KeyCode::KEY_W:
    case EventKeyboard::KeyCode::KEY_S:
        this->rotateCount = this->rotateCount + 1;
        this->rotateSign = GameScene::ROTATE_SIGN[(int)keyCode];
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::rotationUpdate));
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->cueRotateCount = this->cueRotateCount + 1;
        this->cueSign = GameScene::ROTATE_SIGN[(int)keyCode];
        this->schedule(CC_SCHEDULE_SELECTOR(GameScene::cueRotationUpdate));
        break;
        case EventKeyboard::KeyCode::KEY_SPACE:
            this->doShoot();
    }
}

void GameScene::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_A:
    case EventKeyboard::KeyCode::KEY_D:
    case EventKeyboard::KeyCode::KEY_W:
    case EventKeyboard::KeyCode::KEY_S:
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::rotationUpdate));
        break;
    case EventKeyboard::KeyCode::KEY_UP_ARROW:
    case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
    case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
    case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
        this->unschedule(CC_SCHEDULE_SELECTOR(GameScene::cueRotationUpdate));
        break;
    }
}

void GameScene::rotationUpdate(float dt) {
    auto deltaPhi = this->rotateSign.x * dt;
    auto deltaTheta = this->rotateSign.y * dt;
    this->_camera->moveSpherical(deltaPhi, deltaTheta);
}


void GameScene::cueRotationUpdate(float dt) {
    auto speed = this->ui->getCueSpeedValue() / 50;
    this->yaw = this->yaw + dt * this->cueSign.x * speed;
    this->pitch = this->pitch + dt * this->cueSign.y * speed;
    this->pitch = std::min(0.499 * ExtMath::PI, std::max(0., this->pitch));
    auto angle = this->pitch * 180 / ExtMath::PI;
    angle = floor(angle * 100) / 100;

    //this->ui.ballIndicator.angleText.setString(angle + "d");
    auto cosPitch = std::cos(this->pitch);
    this->_cue->updateDirection(ps::ExtMath::vector(
        cosPitch * std::cos(this->yaw),
        cosPitch * std::sin(this->yaw),
        -std::sin(this->pitch)
        ));
    //this->updateTableDirection(this->ui.powerSlider.getPercent() * this->ui.maxForce / 100);
}
