#include "Table.h"
#include <data/Resource.h>
#include <modules/core/PhysicsConstants.hpp>

USING_NS_CC;
using namespace ps;

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
}

void Table::addBall(Ball *ball, ps::ExtMath::vector position) {
	this->_balls.push_back(ball);
	this->addChild(ball);
	ball->setBallPosition(position);
	this->_world->addBall(ball->_body, position);
}

void Table::addWhiteBall(Ball*)
{
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

void Table::reset()
{
	for (auto i = 0; i < _balls.size(); i++) {
		auto ball = _balls[i];
		ball->body()->reset();
		if (ball->_number == 0) ball->setBallPosition(ps::PhysicsConstants::CUE_BALL_POSITION);
		else ball->setBallPosition(ps::PhysicsConstants::INIT_BALL_POSITIONS[ball->_number-1]);
	}
}
