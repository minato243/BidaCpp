#include "Table.h"
#include <data/Resource.h>

USING_NS_CC;

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
	return true;
}

void Table::initTable()
{
	auto table = Sprite3D::create(Res::TABLE_3D);
	table->setLightMask(0);
	table->setScale(100);
	table->setPosition3D(Vec3(0, 0, 0));
	// this.initShader(table);
	this->_table = table;

	//
	// const pEtc = cc.Properties.createNonRefCounted("res/material/3d_table.material#table");
	//
	// this._matEtc = cc.Material.createWithProperties(pEtc);
	// table.getChildByName('Table_Bida3D').getChildByName('ETC').setMaterial(this._matEtc);
	//
	// const pMetal = cc.Properties.createNonRefCounted("res/material/3d_table.material#table");
	//
	// this._matMetal = cc.Material.createWithProperties(pMetal);
	// table.getChildByName('Table_Bida3D').getChildByName('Metal').setMaterial(this._matMetal);

	this->addChild(table);
	/*this.drawKitchen();
	this.initEffect();
	this.initBillBoardOnPockets();
	this.initPocketHighlights();*/

	// this.showCueChallengePocket(0, 1);
	// this.showCueChallengePocket(1, 2);
	// this.showCueChallengePocket(2, 3);
	// this.showCueChallengePocket(3, 4);
	// this.showCueChallengePocket(4, 4);
	// this.showCueChallengePocket(5, 2);
}
