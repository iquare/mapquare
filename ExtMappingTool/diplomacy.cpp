#include "stdafx.h"
#include "diplomacy.h"
#include "triggerdialog.h"
#include "TriggerSystem.h"

void diplomacy::fillDataFromInterface() {
	int i = 0;
	disposition.clear();
	disposition.resize(8);
	for (int i = 0; i < 8; i++) {
		std::vector<u8> v;
		v.resize(8);
		disposition[i] = v;
	}
	for (auto a : dispositionEdits) {
		int player_a = i % 8;
		int player_b = i / 8;
		disposition[player_a][player_b] = std::atoi(a->toPlainText().toStdString().c_str());
		i++;
	}
}

void diplomacy::boxTextChanged() {
	QPlainTextEdit *edit = qobject_cast<QPlainTextEdit*>(sender());
	Q_ASSERT(edit);
	auto text = edit->toPlainText();
	auto string = text.toStdString();	
	bool change = false;
	if (string.length() > 0) {
		for (int i = 0; i < string.size(); i++) {
			if (!(string[i]>='0' && string[i]<='9')) {
				string.erase(string.begin() + i);
				i--;
				change = true;
			}
		}
		while (string.length() > 3) {
			string = "999";
			change = true;
		}
	}
	else {
		string = "0";
	}
	if (string == recursion_check) {
		return;
	}
	recursion_check = string;
	if (change) {
		edit->setPlainText(QString::fromStdString(string));
	}
	recursion_check = "";
}
void diplomacy::ResetInterface(bool read_from_data) {
	for (auto a : disposition_playerlabels) {
		a->deleteLater();
	}
	for (auto a : dispositionEdits) {
		a->deleteLater();
	}
	disposition_playerlabels.clear();
	dispositionEdits.clear();
	setInterface(read_from_data);
}

void diplomacy::setInterface(bool read_from_data) {
	ui.textBoxPlaceholder->hide();
	for (int i = 0; i < 8; i++) {
		auto player_a = new QLabel(QString("Player %1").arg(i + 1), this);
		disposition_playerlabels.push_back(player_a);
		player_a->setGeometry(ui.player_a_mockup->x(), ui.player_a_mockup->y() + 40 * i, 47, 13);
		for (int j = 0; j < 8; j++) {
			auto disposition_textbox = new QPlainTextEdit(this);
			connect(disposition_textbox, SIGNAL(textChanged()),
				this, SLOT(boxTextChanged()));
			int x = ui.textBoxPlaceholder->x();
			int y = ui.textBoxPlaceholder->y();
			disposition_textbox->setGeometry(x + 45 * i, y + 40 * j, 40, 30);
			disposition_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
			disposition_textbox->setFrameShape(QFrame::StyledPanel);
			std::string str = "50";
			if (i <= j) {
				disposition_textbox->setDisabled(true);
				str = "";
			}		
			if (read_from_data) {
				str = WriteInt(disposition[j][i]);
				if (i == 0 && j == 0) {
					QMessageBox::information(this, tr("0-0 disposition"),
						QString::fromStdString(str));
				}
			}
			disposition_textbox->setPlainText(QString::fromStdString(str));
			if (i == 0) {
				auto player_b = new QLabel(QString("Player %1").arg(j + 1), this);
				disposition_playerlabels.push_back(player_b);
				player_b->setGeometry(ui.player_b_mockup->x() + (45 * j), ui.player_b_mockup->y(), 47, 13);
			}
			dispositionEdits.push_back(disposition_textbox);
		}
	}

}

diplomacy::diplomacy(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	api.first_open[DialogMenu::Diplomacy] = true;
	setInterface(false);
	this->setWindowIcon(this->style()->standardIcon(QStyle::SP_DialogOkButton));
}

diplomacy::~diplomacy()
{
}
