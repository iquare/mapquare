#include "stdafx.h"
#include "upgrade_menu.h"
#include "triggerdialog.h"
#include "TriggerSystem.h"
#include "functions.h"

upgrade_menu::upgrade_menu(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	
	scroll_area = new QScrollArea(this);
	scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	QWidget *viewport = new QWidget(this);
	scroll_area->setWidget(viewport);
	scroll_area->setWidgetResizable(true);
	d_layout = new QVBoxLayout(this);
	viewport->setLayout(d_layout);
	d_layout->setSizeConstraint(QLayout::SetFixedSize);
	
	setInterface(false);
	api.first_open[DialogMenu::Upgrades] = true;

	
	dialog_layout = new QVBoxLayout(this);
	this->setLayout(dialog_layout);
	this->layout()->addWidget(scroll_area);
	this->setWindowIcon(this->style()->standardIcon(QStyle::SP_DialogOkButton));
}

upgrade_menu::~upgrade_menu()
{
}

void upgrade_menu::ResetInterface(bool read_from_data){


	if (!read_from_data) {
		scroll_area->deleteLater();
		dialog_layout->deleteLater();
		d_layout->deleteLater();
		for (auto a : initialLevelBoxes) {
			a->deleteLater();
		}
		for (auto a : maxLevelBoxes) {
			a->deleteLater();
		}
		for (auto a : upgradeHeaderLabels) {
			a->deleteLater();
		}
		for (auto a : upgradeNameLabels) {
			a->deleteLater();
		}
		for (auto a : lowerEscalationButtons) {
			a->deleteLater();
		}
		for (auto a : upperEscalationButtons) {
			a->deleteLater();
		}
		for (auto a : initialLevelLabels) {
			a->deleteLater();
		}
		for (auto a : maxLevelLabels) {
			a->deleteLater();
		}
		for (auto a : buttonGroups) {
			a->deleteLater();
		}
		for (auto a : dummy_labels) {
			a->deleteLater();
		}
		for (auto a : playerButtons) {
			a->deleteLater();
		}


		initialLevelBoxes.clear();
		maxLevelBoxes.clear();
		upgradeHeaderLabels.clear();
		upgradeNameLabels.clear();
		lowerEscalationButtons.clear();
		upperEscalationButtons.clear();
		buttonGroups.clear();
		dummy_labels.clear();
		playerButtons.clear();
		
	}
	setInterface(read_from_data);

}
void upgrade_menu::fillDataFromInterface() {
	//add later
	//unlike other menus, it fills from internal data instead of interface

	//upgrades_data.resize(255);
	upgrades_data.clear();
	int size = 0;
	for (auto a : internal_info) {
		std::vector<Ext> upg;
		upg.resize(256);
		for (auto b : a.second) {
			upg[b.second.upgrade_id] = b.second;
			size++;
		}
		upgrades_data.push_back(upg);
	}
	
	QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("Upgrades data size after filling: " + 
			WriteInt(upgrades_data.size())+" "+WriteInt(size)));
}

void upgrade_menu::maxBoxTextChanged(){
	QPlainTextEdit *edit = qobject_cast<QPlainTextEdit*>(sender());
	Q_ASSERT(edit);
	auto text = edit->toPlainText();
	auto string = text.toStdString();
	auto index = qvariant_cast<int>(edit->property("upgrade"));
	bool change = false;
	if (string.length() > 0) {
/*		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString(string));*/
		if (!is_numeric(string)) {
			string = WriteInt(definitions[index].max_level);
			change = true;
		}
		else {
			if (StringToInt(string) > 255) {
				string = "255";
				change = true;
			}
		}
	}
	else {
		string = "0";
		change = true;
	}
	if (internal_info[currentPlayer][index].max != StringToInt(string)) {
		change = true;
	}
	if (string == maxBoxRecursionCheck) {
		return;
	}
	maxBoxRecursionCheck = string;
	if (change) {
		edit->setPlainText(QString::fromStdString(string));
		internal_info[currentPlayer][index].max = StringToInt(string);
	}
	maxBoxRecursionCheck = "";
}
void upgrade_menu::initialBoxTextChanged(){
	QPlainTextEdit *edit = qobject_cast<QPlainTextEdit*>(sender());
	Q_ASSERT(edit);
	auto text = edit->toPlainText();
	auto string = text.toStdString();
	auto index = qvariant_cast<int>(edit->property("upgrade"));
	if (definitions[index].escalation) {
		return;
	}
	bool change = false;
	if (string.length() > 0) {
		if (!is_numeric(string)) {
			string = "0";
			change = true;
		}
		else {
			if (StringToInt(string) > 255) {
				string = "255";
				change = true;
			}
		}
	}
	else {
		string = "0";
		change = true;
	}
	if (internal_info[currentPlayer][index].initial != StringToInt(string)) {
		change = true;
	}
	if (string == iniBoxRecursionCheck) {
		return;
	}
	iniBoxRecursionCheck = string;
	
	if (change) {
		edit->setPlainText(QString::fromStdString(string));
	}
	if (change) {
		edit->setPlainText(QString::fromStdString(string));
		internal_info[currentPlayer][index].initial = StringToInt(string);

	}
	
	iniBoxRecursionCheck = "";
}

void upgrade_menu::escButtonLowClicked(){
	QRadioButton *button = qobject_cast<QRadioButton*>(sender());
	Q_ASSERT(button);
	auto index = qvariant_cast<int>(button->property("upgrade"));
	internal_info[currentPlayer][index].escalation_status = definitions[index].lower.id;
}
void upgrade_menu::escButtonHighClicked(){
	QRadioButton *button = qobject_cast<QRadioButton*>(sender());
	Q_ASSERT(button);
	auto index = qvariant_cast<int>(button->property("upgrade"));
	internal_info[currentPlayer][index].escalation_status = definitions[index].upper.id;
}

void upgrade_menu::playerButtonClicked() {
	QPushButton *button = qobject_cast<QPushButton*>(sender());
	Q_ASSERT(button);
	auto index = qvariant_cast<int>(button->property("player"));
	currentPlayer = index;
	/*QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("Set player to " + WriteInt(currentPlayer)));
	QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("Player 1 cyp max level: " + WriteInt(internal_info[0][62].max)));
	*/
	for(auto a : initialLevelBoxes){
		int index = qvariant_cast<int>(a->property("upgrade"));
		a->setPlainText(QString::fromStdString(WriteInt(internal_info[currentPlayer][index].initial)));
		/*
		if (internal_info[currentPlayer][index].escalation_status==0xe4) {
			a->setPlainText("");
		}
		else {
			a->setPlainText(QString::fromStdString(WriteInt(internal_info[currentPlayer][index].initial)));
		}*/
	}
	for (auto a : maxLevelBoxes) {
		int index = qvariant_cast<int>(a->property("upgrade"));
		a->setPlainText(QString::fromStdString(WriteInt(internal_info[currentPlayer][index].max)));
	}
	for(auto a : lowerEscalationButtons){
		int index = qvariant_cast<int>(a->property("upgrade"));
		if (internal_info[currentPlayer][index].escalation_status != 0xe4) {
			if (definitions[index].lower.id == internal_info[currentPlayer][index].escalation_status) {
				a->click();
			}
		}
	}
	for (auto a : upperEscalationButtons) {
		int index = qvariant_cast<int>(a->property("upgrade"));
		if (internal_info[currentPlayer][index].escalation_status != 0xe4) {
			if (definitions[index].upper.id == internal_info[currentPlayer][index].escalation_status) {
				a->click();
			}
		}
	}
	recolorButtons();
}

void upgrade_menu::prepareWidgetForLayout(QWidget *widget, int pos_x, int pos_y, int bound_x, int bound_y) {
	widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	widget->setGeometry(pos_x, pos_y,
		bound_x, bound_y);
	widget->updateGeometry();
	widget->setFixedSize(bound_x, bound_y);
}
void upgrade_menu::SetDataValuesFromArchive() {
	
	iter_enumerated(upgrades_data, [&](std::vector<Ext> a, int i) {
		int j = 0;
		for (auto b : a) {
			if (j >= 62) {
				if (i == 0) {
					/*					QMessageBox::information(this, tr("Unable to open file"),
					QString::fromStdString(WriteInt(j) + " " + WriteInt(b.upgrade_id) + " " + WriteInt(b.max) + " " + WriteInt(b.escalation_status)));
					*/
				}
				internal_info[i][b.upgrade_id] = b;
			}
			j++;
		}
	});

	for (int i = 0; i < initialLevelBoxes.size();i++) {
		auto index = qvariant_cast<int>(initialLevelBoxes[i]->property("upgrade"));
		initialLevelBoxes[i]->setPlainText(QString::fromStdString
		(WriteInt(upgrades_data[currentPlayer][index].initial)));
		maxLevelBoxes[i]->setPlainText(QString::fromStdString
		(WriteInt(upgrades_data[currentPlayer][index].max)));
	}
	for (int i = 0; i < lowerEscalationButtons.size(); i++) {

		auto index = qvariant_cast<int>(lowerEscalationButtons[i]->property("upgrade"));
		if (internal_info[currentPlayer][index].escalation_status != 0xe4) {
			
			if (upgrades_data[currentPlayer][index].escalation_status == definitions[index].lower.id) {
				lowerEscalationButtons[i]->click();
			}
			else {
				upperEscalationButtons[i]->click();
			}
		}
	}
	
}

void upgrade_menu::colorize_button(QPushButton* button, u32 color) {
	u32 r = color & 0xff;
	u32 g = (color & 0xff00)>>8;
	u32 b = (color & 0xff0000)>>16;
	std::stringstream ss;
	ss << "background-color: rgb(";
	ss << WriteInt(r); ss << ", ";
	ss << WriteInt(g); ss << ", ";
	ss << WriteInt(b); 
	ss << "); color: rgb(0, 0, 0)";
	button->setAutoFillBackground(true);
	button->setStyleSheet(QString::fromStdString(ss.str()));
}
void upgrade_menu::recolorButtons() {
	iter_enumerated(playerButtons, [&](QPushButton* button, int i) {
		if (currentPlayer == i) {
			colorize_button(button, 0xffffef);
		}
		else {
			colorize_button(button, 0xdddddd);
		}
	});
}
void upgrade_menu::setInterface(bool read_from_data){
	if (read_from_data) {
		SetDataValuesFromArchive();
		return;
	}

	currentPlayer = 0;
	ui.upgradeName->hide();
	ui.upgradeNameHeaderPlaceholder->hide();
	ui.initialLevelPlaceholder->hide();
	ui.maxLevelPlaceholder->hide();
	ui.initialLevelTextbox->hide();
	ui.maxLevelTextbox->hide();
	ui.lowerEscalationButton->hide();
	ui.upperEscalationButton->hide();
	/*std::ifstream upgrade_def_config, ext_upgrades_data, escalations;
	std::map<int, std::vector<std::string>> unit_name;
	*/
	

	auto dummy_layout = new QHBoxLayout(this);
	d_layout->addLayout(dummy_layout);
	internal_info.clear();
	for (auto a : definitions) {
		for (int i = 0; i < 8; i++) {
			
			internal_info[i][a.first].initial = 0;
			internal_info[i][a.first].max = a.second.max_level;
			internal_info[i][a.first].upgrade_id = a.first;
			if (a.second.escalation) {

				internal_info[i][a.first].escalation_status = a.second.lower.id;
			}
		}
	}
	auto button_layout = new QHBoxLayout(this);
	for (int i = 0; i < 8; i++) {
		auto player = new QPushButton(this);

		player->setProperty("player", QVariant::fromValue(i));
		connect(player, SIGNAL(clicked()),
			this, SLOT(playerButtonClicked()));
		playerButtons.push_back(player);
		

		player->setText(QString::fromStdString("Player "+WriteInt(i+1)));
		prepareWidgetForLayout(player, 0, 0, 75, 23);
		button_layout->addWidget(player);
	}



	recolorButtons();
	d_layout->addLayout(button_layout);
	
	for (auto a : definitions) {
		int i = a.first - 62;
		auto level_layout = new QHBoxLayout(this);
		auto head_layout = new QBoxLayout(QBoxLayout::Direction::LeftToRight,this);
		auto header = new QLabel("Upgrade", this);		
		upgradeHeaderLabels.push_back(header);
		prepareWidgetForLayout(header, ui.upgradeNameHeaderPlaceholder->x(),
									ui.upgradeNameHeaderPlaceholder->y() + 40 * i,
									101, 16);

		auto upgrade = new QLabel(QString::fromStdString(a.second.name), this);
		upgradeNameLabels.push_back(upgrade);
		prepareWidgetForLayout(upgrade, ui.upgradeName->x(),ui.upgradeName->y() + 40 * i,
			171, 16);

		head_layout->addWidget(header);
		head_layout->addWidget(upgrade);
		level_layout->addLayout(head_layout);
		
		auto initial_box_layout = new QHBoxLayout(this);
		initial_box_layout->setStretch(0, 0);
		auto initial = new QLabel("Initial Level", this);	
		initialLevelLabels.push_back(initial);
		//initial->setGeometry(ui.initialLevelPlaceholder->x(), ui.initialLevelPlaceholder->y() + 40 * i, 171, 16);
		prepareWidgetForLayout(initial, ui.initialLevelPlaceholder->x(),
			ui.initialLevelPlaceholder->y() + 40 * i,
			61, 16);
		initial_box_layout->addWidget(initial);

		auto initial_textbox = new QPlainTextEdit(this);
		
		initial_textbox->setProperty("upgrade", QVariant::fromValue(a.first));
		initial_textbox->setProperty("count", QVariant::fromValue(initialLevelBoxes.size()));
		connect(initial_textbox, SIGNAL(textChanged()),
			this, SLOT(initialBoxTextChanged()));
		initialLevelBoxes.push_back(initial_textbox);
		int x = ui.initialLevelTextbox->x();
		int y = ui.initialLevelTextbox->y();
//		initial_textbox->setGeometry(x, y + 40 * i, 41, 21);
		initial_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
		initial_textbox->setFrameShape(QFrame::StyledPanel);
		if (a.second.escalation) {
			initial_textbox->setDisabled(true);
			initial->setDisabled(true);/*
									   QMessageBox::information(this, tr("Unable to open file"),
									   QString::fromStdString("Disable: " + WriteInt(i) + " " + WriteInt(a.first)));*/
		}
		std::string str = "0";
		
		if (!a.second.escalation) {
			initial_textbox->setPlainText(QString::fromStdString(str));
		}
		prepareWidgetForLayout(initial_textbox, x, y, 41, 21);
		initial_box_layout->addWidget(initial_textbox);
		level_layout->addLayout(initial_box_layout);


		auto max_layout = new QHBoxLayout(this);
		
		auto max = new QLabel("Max Level", this);	
		maxLevelLabels.push_back(max);
		prepareWidgetForLayout(max, x, y, 61, 16);
		//max->setGeometry(ui.maxLevelPlaceholder->x(), ui.maxLevelPlaceholder->y() + 40 * i, 171, 16);
		max_layout->addWidget(max);

		auto max_textbox = new QPlainTextEdit(this);	
		max_textbox->setProperty("upgrade", QVariant::fromValue(a.first));
		max_textbox->setProperty("count", QVariant::fromValue(maxLevelBoxes.size()));
//		max_textbox->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
		connect(max_textbox, SIGNAL(textChanged()),
			this, SLOT(maxBoxTextChanged()));
		maxLevelBoxes.push_back(max_textbox);
		x = ui.maxLevelTextbox->x();
		y = ui.maxLevelTextbox->y();
		prepareWidgetForLayout(max_textbox, x, y, 41, 20);
		max_textbox->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
		max_textbox->setFrameShape(QFrame::StyledPanel);
		str = WriteInt(a.second.max_level);

		max_textbox->setPlainText(QString::fromStdString(str));
		max_layout->addWidget(max_textbox);
		
		level_layout->addLayout(max_layout);
		auto escalation_layout = new QHBoxLayout(this);
		if (definitions[a.first].escalation) {			
			
			auto esc_button_low = new QRadioButton(QString::fromStdString(a.second.lower.name),this);
			connect(esc_button_low, SIGNAL(clicked()),
				this, SLOT(escButtonLowClicked()));
			esc_button_low->setProperty("upgrade", QVariant::fromValue(a.first));
			esc_button_low->setProperty("escalation", QVariant::fromValue(a.first));
			lowerEscalationButtons.push_back(esc_button_low);
			x = ui.lowerEscalationButton->x();
			y = ui.lowerEscalationButton->y();
			//esc_button_low->setGeometry(x, y + 40 * i, 115, 17);
			prepareWidgetForLayout(esc_button_low, x,y+40*i,115, 17);
			

			escalation_layout->addWidget(esc_button_low);

			auto esc_button_high = new QRadioButton(QString::fromStdString(a.second.upper.name),this);		
			connect(esc_button_high, SIGNAL(clicked()),
				this, SLOT(escButtonHighClicked()));
			esc_button_high->setProperty("upgrade", QVariant::fromValue(a.first));
			esc_button_high->setProperty("escalation", QVariant::fromValue(a.second.escalation));
			upperEscalationButtons.push_back(esc_button_high);
			x = ui.upperEscalationButton->x();
			y = ui.upperEscalationButton->y();
			//esc_button_high->setGeometry(x, y + 40 * i, 115, 17);
			prepareWidgetForLayout(esc_button_high, x, y + 40 * i, 115, 17);
			escalation_layout->addWidget(esc_button_high);
			level_layout->addLayout(escalation_layout);
			//
			auto group = new QButtonGroup(this);
			group->addButton(esc_button_high);
			group->addButton(esc_button_low);
			buttonGroups.push_back(group);
			esc_button_low->click();

		}
		else {
			auto dummy = new QLabel(this);
			x = ui.lowerEscalationButton->x();
			y = ui.lowerEscalationButton->y();
			prepareWidgetForLayout(dummy, x, y + 40 * i, 115, 17);
			dummy_labels.push_back(dummy);
			escalation_layout->addWidget(dummy);
			//
			x = ui.upperEscalationButton->x();
			y = ui.upperEscalationButton->y();
			prepareWidgetForLayout(dummy, x, y + 40 * i, 115, 17);
			escalation_layout->addWidget(dummy);
		}
		level_layout->addLayout(escalation_layout);
		d_layout->addLayout(level_layout);
	}

	/*
	for (auto a : definitions) {
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Key: " + WriteInt(a.first) + " Max: " + WriteInt(a.second.max_level) + " Name: " + a.second.name));
	}*/
}