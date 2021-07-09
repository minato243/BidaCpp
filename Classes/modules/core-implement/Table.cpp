#include "Table.h"
#include <data/Resource.h>
#include <modules/core/PhysicsConstants.hpp>
#include <functional>
#include <modules/utils/CocosUtils.h>
#include <modules/core/GameMgr.h>

USING_NS_CC;
using namespace ps;

Color4B Table::GUILD_RED = Color4B(255, 50, 50, 150);
Color4B Table::GUILD_GREEN = Color4B(50, 255, 50, 255);
Color4B Table::GUILD_GRAY = Color4B(0, 0, 0, 40);
Color4B Table::GUILD_WHITE = Color4B(255, 255, 255, 40);

Table::Table()
{
}

Table::~Table()
{
}

bool Table::init()
{
	if (!Node::init())
		return false;

	this->initTable();
	this->initPhysicsWorld();
	return true;
}

void Table::initTable()
{
	auto table = Sprite3D::create(Res::TABLE_3D);
	table->setLightMask(0);
	table->setScale(100);
	// this->initShader(table);
	this->_table = table;

//
// const pEtc = cc.Properties.createNonRefCounted("res/material/3d_table.material#table");
//
// this->_matEtc = cc.Material.createWithProperties(pEtc);
// table.getChildByName('Table_Bida3D').getChildByName('ETC').setMaterial(this->_matEtc);
//
// const pMetal = cc.Properties.createNonRefCounted("res/material/3d_table.material#table");
//
// this->_matMetal = cc.Material.createWithProperties(pMetal);
// table.getChildByName('Table_Bida3D').getChildByName('Metal').setMaterial(this->_matMetal);

this->addChild(table);
/*this->drawKitchen();
this->initEffect();
this->initBillBoardOnPockets();
this->initPocketHighlights();*/

// this->showCueChallengePocket(0, 1);
// this->showCueChallengePocket(1, 2);
// this->showCueChallengePocket(2, 3);
// this->showCueChallengePocket(3, 4);
// this->showCueChallengePocket(4, 4);
// this->showCueChallengePocket(5, 2);
}

void Table::initPhysicsWorld()
{
	_world = new ps::PhysicsWorld();
	_world->getDispatcher()->subscribeWorldPaused(CC_CALLBACK_1(Table::onWorldPause, this));
}

void Table::addBall(Ball* ball, ps::ExtMath::vector position) {
	this->_balls.push_back(ball);
	this->addChild(ball);
	ball->setBallPosition(position);
	this->_world->addBall(ball->_body, position);
	if (ball->body()->id() == Ball::WHITE_BALL_ID) {
		_whiteBall = ball;
	}
}

void Table::addWhiteBall(Ball* ball)
{
	this->addBall(ball);
}

void Table::addCue(Cue* cue)
{
	this->addChild(cue);
	cue->setCuePosition(this->_whiteBall->body()->position());
	this->_cue = cue;
}

void Table::initGuildLine()
{
	this->_guildLine = TableGuildLine::create();
	this->addChild(_guildLine);
	_guildLine->setCameraMask(this->getCameraMask(), true);
	_guildLine->drawLine(Vec3(0,0,0), Vec3(100, 100, 0));
}

void Table::onEnter()
{
	Node::onEnter();
	this->schedule(CC_SCHEDULE_SELECTOR(Table::step));
}

void Table::step(float dt)
{
	this->_world->update(dt);
}

void Table::onWorldPause(double dt)
{
	this->updateCuePosition();
}

void Table::updateCuePosition()
{
	this->_cue->setCuePosition(this->_whiteBall->body()->position());
}

void Table::reset()
{
	for (auto i = 0; i < _balls.size(); i++) {
		auto ball = _balls[i];
		ball->body()->reset();
		if (ball->_number == 0) ball->setBallPosition(ps::PhysicsConstants::CUE_BALL_POSITION);
		else ball->setBallPosition(ps::PhysicsConstants::INIT_BALL_POSITIONS[ball->_number - 1]);
	}
}

void Table::setGuildLineVisible(bool visible)
{
	_guildLine->setVisible(visible);
}

void Table::updateGuildLines(std::vector<cocos2d::Vec3> points)
{
	auto sub = _guildLine->sub;
	sub->clear();
	sub->drawLine(points[0], points[1], Table::GUILD_WHITE);
}

void Table::drawFloatingTrajectory(ps::ExtMath::vector reflectDir, std::vector<ps::ExtMath::vector> points)
{
	if (points.size() ==0) {
		_guildLine->clearAll();
		this->getFakeCueBall()->setVisible(false);
		return;
	}
	auto len = points.size();

	auto guild = this->_guildLine;
	guild->clearAll();

	auto i = 1;
	auto prevPoint = points[0];
	vector point;
	auto hasFlying = false;

	while (i < len) {
		point = points[i];
		guild->drawLine(
			CocosUtils::transVecToCCVec(prevPoint),
			CocosUtils::transVecToCCVec(point),
			Table::GUILD_GREEN
		);

		hasFlying |= (point.z > PhysicsConstants::BALL_RADIUS);

		// if (prevPoint.z > gv.Physicsautoants.BALL_RADIUS || point.z > gv.Physicsautoants.BALL_RADIUS) {
		guild->sub->drawLine(
			Vec3(prevPoint.x, 0.02, -prevPoint.y),
			Vec3(point.x, 0.02, -point.y),
			Table::GUILD_GRAY
		);
		// }

		prevPoint = point;
		i = i + 1;
	}
	auto fakeBall = this->getFakeCueBall();
	fakeBall->setVisible(true);
	fakeBall->setPosition3D(CocosUtils::transVecToCCVec(prevPoint));
	this->getFakeCueBall()->setColor(Color3B(255, 255, 255));
	//auto match = gv.gameMgr.ebpCtrl._curMatch; todo
	auto ballIdx = reflectDir ? reflectDir.ballIdx : null;
	/*if (match && utils.isNumber(ballIdx)) { //todo
		auto suggestedBalls = match.getSuggestBall();
		if (suggestedBalls.indexOf(ballIdx) >= 0)
			this->getFakeCueBall()->setColor(Color3B(255, 255, 255));
		else
			this->getFakeCueBall()->setColor(Color3B(255, 150, 150));

		if (utils.isNumber(ballIdx)) {
			gv::gameMgr.gameUI.getIngameUI().setInteractionBall(ballIdx);
		}
		else {
			gv::gameMgr.gameUI.getIngameUI().setInteractionBall(-1);
		}
	}*/

	if (reflectDir && reflectDir.s1) {
		guild->drawLine(
			CocosUtils::transVecToCCVec(reflectDir.s1),
			CocosUtils::transVecToCCVec(reflectDir.e1),
			Table::GUILD_GREEN
		);

		guild->sub->drawLine(
			Vec3(reflectDir.s1.x, 0.02, -reflectDir.s1.y),
			Vec3(reflectDir.e1.x, 0.02, -reflectDir.e1.y),
			Table::GUILD_GRAY
		);

		guild->sub->drawCircle(
			Vec3(reflectDir.s1.x, 0.02, -reflectDir.s1.y),
			PhysicsConstants::BALL_RADIUS,
			Vec3(0, 1, 0),
			50,
			Table::GUILD_GRAY
		);
	}
	if (reflectDir && reflectDir.s2) {
		guild->drawLine(
			CocosUtils::transVecToCCVec(reflectDir.s2),
			CocosUtils::transVecToCCVec(reflectDir.e2),
			Table::GUILD_GREEN
		);

			guild->sub->drawLine(
				Vec3(reflectDir.s2.x, 0.02, -reflectDir.s2.y),
				Vec3(reflectDir.e2.x, 0.02, -reflectDir.e2.y),
				Table::GUILD_GRAY
			);

		guild->sub->drawCircle(
			Vec3(reflectDir.s2.x, 0.02, -reflectDir.s2.y),
			PhysicsConstants::BALL_RADIUS,
			Vec3(0, 1, 0),
			50,
			Table::GUILD_GRAY
		);
	}
}

ps::PhysicsWorld* Table::getWorld()
{
	return this->_world;
}


// Class TableGuildLine
TableGuildLine::TableGuildLine()
{
}

bool TableGuildLine::init()
{
	if (!ThiccDrawNode3D::init()){
		return false;
	}
	sub = ThiccDrawNode3D::create();
	this->addChild(sub);
	sub2 = ThiccDrawNode3D::create();
	this->addChild(sub2);
	sub3 = ThiccDrawNode3D::create();
	this->addChild(sub3);
	sub4 = ThiccDrawNode3D::create();
	this->addChild(sub4);
	sub5 = ThiccDrawNode3D::create();
	this->addChild(sub5);
	sub6 = ThiccDrawNode3D::create();
	this->addChild(sub6);
	return true;
}

void TableGuildLine::clearAll()
{
	this->clear();
	sub->clear();
	sub2->clear();
	sub3->clear();
	sub4->clear();
	sub5->clear();
	sub6->clear();
}
