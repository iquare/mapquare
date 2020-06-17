#pragma once

#include <QDialog>
#include "ui_diplomacy.h"


class test_button : QPushButton {


};

class diplomacy : public QDialog
{
	Q_OBJECT

public:
	void fillDataFromInterface();
	void ResetInterface(bool read_from_data);
	diplomacy(QWidget *parent = Q_NULLPTR);
	~diplomacy();
private:
	Ui::diplomacy ui;
	QVector<QLabel*> disposition_playerlabels;
	QVector<QPlainTextEdit*> dispositionEdits;
	std::string recursion_check;
	void setInterface(bool read_from);
private slots:
	void boxTextChanged();
};
