#include "stdafx.h"
#include "alliances.h"
#include "diplomacy.h"
#include "triggerdialog.h"
#include "TriggerSystem.h"

Alliances::Alliances(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	api.first_open[DialogMenu::Alliances] = true;
	setInterface(false);
	this->setWindowIcon(this->style()->standardIcon(QStyle::SP_DialogOkButton));
}

Alliances::~Alliances()
{
}

void Alliances::fillDataFromInterface(){
	alliances.clear();
	alliances.resize(8);
	for (int i = 0; i < 8; i++) {
		std::vector<u8> v;
		v.resize(8);
		alliances[i] = v;
	}
	for (auto a : alliance_buttons) {
		auto value = qvariant_cast<int>(a->property("status"));
		auto p1 = qvariant_cast<int>(a->property("i"));
		auto p2 = qvariant_cast<int>(a->property("j"));
		alliances[p1][p2] = value;
	}

}
void Alliances::ResetInterface(bool read_from_data){
	for (auto a : alliance_buttons) {
		a->deleteLater();
	}
	for (auto a : playerlabels) {
		a->deleteLater();
	}
	alliance_buttons.clear();
	playerlabels.clear();
	setInterface(read_from_data);
}

void allianceoffset(int &x) {
	x++;
	if (x > AllyStatus::SubordinateAlly) {
		x = 0;
	}
}
void Alliances::allianceButtonClicked() {
	QPushButton *button = qobject_cast<QPushButton*>(sender());
	Q_ASSERT(button);
	auto value = qvariant_cast<int>(button->property("status"));
	allianceoffset(value);
	setButtonAllyStatus(button, value);
}

#include <sstream>
void Alliances::colorize_button(QPushButton* button, u32 color) {
	u32 b = color & 0xff;
	u32 g = (color & 0xff00) >> 8;
	u32 r = (color & 0xff0000) >> 16;
	std::stringstream ss;
	ss << "background-color: rgb(";
	ss << WriteInt(r); ss << ", ";
	ss << WriteInt(g); ss << ", ";
	ss << WriteInt(b);
	ss << "); color: rgb(0, 0, 0)";
	button->setAutoFillBackground(true);
	button->setStyleSheet(QString::fromStdString(ss.str()));
}
QString allyStatusName(int status) {
	switch(status) {
	case AllyStatus::Enemy:
		return "E";
	case AllyStatus::Neutral:
		return "N";
	case AllyStatus::SubordinateAlly:
		return "A";
	default:
		break;
	}
	return "E";
}
void Alliances::setButtonAllyStatus(QPushButton* button, int status) {
	button->setProperty("status", QVariant::fromValue(status));
	button->setText(allyStatusName(status));
	u32 color = 0xffffff;
	switch(status) {
	case AllyStatus::Enemy:
		color = 0xff3c00;
		break;
	case AllyStatus::Neutral:
		color = 0x9ff6f7;
		break;
	case AllyStatus::SubordinateAlly:
		color = 0x00ff00;
		break;
	}
	colorize_button(button, color);
}
void Alliances::setInterface(bool read_from_data){
	for (int i = 0; i < 8; i++) {
		auto player_a = new QLabel(QString("Player %1").arg(i + 1), this);
		playerlabels.push_back(player_a);
		player_a->setGeometry(30, 65 + 40 * i, 47, 13);
		for (int j = 0; j < 8; j++) {
			if (i == 0) {
				auto player_b = new QLabel(QString("Player %1").arg(j + 1), this);
				playerlabels.push_back(player_b);
				player_b->setGeometry(80 + (45 * j), 40, 47, 13);
			}
			if (i <= j) {
				continue;
			}
			auto alliance_button = new QPushButton(this);
			alliance_buttons.push_back(alliance_button);
			connect(alliance_button, SIGNAL(clicked()),
				this, SLOT(allianceButtonClicked()));
			int x = 80;
			int y = 60;
			alliance_button->setGeometry(x + 45 * i, y + 40 * j, 40, 30);
			alliance_button->setProperty("i", QVariant::fromValue(i));
			alliance_button->setProperty("j", QVariant::fromValue(j));			
			setButtonAllyStatus(alliance_button, AllyStatus::Enemy);
			if (read_from_data) {
				setButtonAllyStatus(alliance_button,alliances[i][j]);
			}
		}
	}
}