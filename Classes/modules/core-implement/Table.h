#ifndef TABLE_H_
#define TABLE_H_

#include <cocos2d.h>
#include "Ball.h"
#include <modules/core/PhysicsWorld.hpp>

class Table: public cocos2d::Node {
public:

	cocos2d::Sprite3D* _table;
	std::vector<Ball*> _balls;
	ps::PhysicsWorld* _world;

	Table();
	~Table();

	CREATE_FUNC(Table);
	virtual bool init();
	void initTable();
	void initPhysicsWorld();
	void addBall(Ball *ball, ps::ExtMath::vector position = ps::ExtMath::vector(0,0));
	void addWhiteBall(Ball *);
	
	virtual void onEnter();

	void step(float dt);

	void reset();

};
#endif // !TABLE_H_
