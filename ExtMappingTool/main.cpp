#include "stdafx.h"
#include "extmappingtool.h"
#include <QtWidgets/QApplication>
#include "string"
#include <iostream>
#include <fstream>
#include <ostream>
#include "SFmpqapi\SFmpqapi.h"

#include "serializer.h"
#include "deserializer.h"
#include "TriggerSystem.h"

#pragma comment (lib, "D:\\sfmpq\\SFmpq.lib" )
#include "MapScene.h"

int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	
	loadExternalDefinitions();
	ExtMappingTool w;
	for (int i = 0; i < 25; i++) {
		api.first_open[i] = false;
	}
	
	w.upgReqMenu.init();
	MPQHANDLE hMpq;
	auto id = qRegisterMetaType<TriggerSystemItem>();
	QVariant test{ id, nullptr };


	std::vector<ArgumentConvention> vtest;
	ArgumentConvention c;
	c.args.push_back(Argument(14));
	c.args.push_back(Argument(45));
	c.args.push_back(Argument(1042));
	c.args.push_back(Argument(257));
	vtest.push_back(c);
	vtest.push_back(c);

	trigger_setup.actions = vtest;


	std::ofstream test_stream;
	test_stream.open("test.bin", std::ios::binary | std::ios::out);
	
	/*
	test_stream << editor_info;
	test_stream << file_info;
	test_stream << trigger_setup;//is it required?

	test_stream << trigger_system;
	test_stream << disposition;
	test_stream << faction_list;
	test_stream << alliances;*/

	test_stream.close();

	std::string str = "D:/iquare/sc/maps/Project Carvalho/desert blends.scx";
	if (!SFileOpenArchive(str.c_str(), 0, 0, &hMpq)) {
	}
	w.handleIcons();
	w.show();

	/*
	QGraphicsScene scene(0, 0, 10000, 20000);
	MapScene* view = new MapScene(&scene);
	QGraphicsRectItem* pRect = new QGraphicsRectItem(0, 0, 100, 100);
	pRect->setPos(scene.width() / 2, scene.height() / 2);
	scene.addItem(pRect);
	QTimer* pTimer = new QTimer;
	pTimer->setSingleShot(true);

	QObject::connect(pTimer, &QTimer::timeout, [=]() {

		view->centerOn(pRect); // centre on the rectangle
		view->AnimateBy(100);
		pTimer->deleteLater();
	});

	pTimer->start(1000);
	view->show();
	*/


	return a.exec();
}
