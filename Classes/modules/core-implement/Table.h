#ifndef TABLE_H_
#define TABLE_H_

#include <cocos2d.h>

class Table: public cocos2d::Node {
public:

	cocos2d::Sprite3D* _table;

	Table();
	~Table();

	CREATE_FUNC(Table);
	virtual bool init();
	void initTable();
};
#endif // !TABLE_H_
