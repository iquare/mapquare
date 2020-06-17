#pragma once

#include <QDialog>
#include "ui_alliances.h"
#include "TriggerSystem.h"
namespace AllyStatus {
	enum {
		Enemy,
		Neutral,
		SubordinateAlly,
	};
}
class Alliances : public QDialog
{
	Q_OBJECT

public:
	Alliances(QWidget *parent = Q_NULLPTR);
	~Alliances();
	void fillDataFromInterface();
	void ResetInterface(bool read_from_data);

	void setButtonAllyStatus(QPushButton* button, int status);
private:
	Ui::Alliances ui;
	QVector<QLabel*> playerlabels;
	QVector<QPushButton*> alliance_buttons;
//	std::string recursion_check;
	void setInterface(bool read_from);

	void colorize_button(QPushButton* button, u32 color);
private slots:
	//void boxTextChanged();

	void allianceButtonClicked();
};