#ifndef TABLE_H_
#define TABLE_H_

#include <cocos2d.h>
#include "Ball.h"
#include <modules/core/PhysicsWorld.hpp>
#include <modules/core-implement/Cue.h>
#include <modules/camera-view/utils/ThiccDrawNode3D.h>

class TableGuildLine : public ThiccDrawNode3D {
public:
	TableGuildLine();
	bool init();
	CREATE_FUNC(TableGuildLine);
	void clearAll();

	ThiccDrawNode3D* sub;
	ThiccDrawNode3D* sub2;
	ThiccDrawNode3D* sub3;
	ThiccDrawNode3D* sub4;
	ThiccDrawNode3D* sub5;
	ThiccDrawNode3D* sub6;
};

class Table: public cocos2d::Node {
public:
	Table();
	~Table();

	CREATE_FUNC(Table);
	virtual bool init();
	void initTable();
	void initPhysicsWorld();
	void addBall(Ball *ball, ps::ExtMath::vector position = ps::ExtMath::vector(0,0));
	void addWhiteBall(Ball *ball);
	void addCue(Cue *cue);
	void initGuildLine();
	
	virtual void onEnter();
	void step(float dt);
	void onWorldPause(double dt);
	void updateCuePosition();
	void reset();

	void setGuildLineVisible(bool visible);
	void updateGuildLines(std::vector<cocos2d::Vec3> points);
	void drawFloatingTrajectory(ps::ExtMath::vector reflectDir, std::vector<ps::ExtMath::vector> points);
	
	ps::PhysicsWorld* getWorld();
	Ball* getFakeCueBall();

private:
	cocos2d::Sprite3D* _table;
	std::vector<Ball*> _balls;
	Cue* _cue;
	Ball* _whiteBall;
	ps::PhysicsWorld* _world;
	TableGuildLine* _guildLine;

public:
	static cocos2d::Color4B GUILD_RED;
	static cocos2d::Color4B GUILD_GREEN;
	static cocos2d::Color4B GUILD_GRAY;
	static cocos2d::Color4B GUILD_WHITE;

};
#endif // !TABLE_H_
