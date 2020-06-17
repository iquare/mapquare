#pragma once

#include <QWidget>
#include <QMetaType>
#include "ui_triggerMenu.h"

class triggerMenu : public QWidget
{
	Q_OBJECT

public:
	triggerMenu(QWidget *parent = Q_NULLPTR);
	~triggerMenu();

private:
	Ui::triggerMenu ui;
};
