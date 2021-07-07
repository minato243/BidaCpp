#ifndef GAMEMGR_H_
#define GAMEMGR_H_

#include <modules/core-implement/Table.h>

class GameMgr {

private:
public:
	Table* table;

	GameMgr();
	~GameMgr();


};

namespace gv {
	extern GameMgr* gameMgr;
};

#endif // !GAMEMGR_H_

