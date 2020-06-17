#include "stdafx.h"
#include "upgreqmenu.h"
#include "triggerdialog.h"
#include "TriggerSystem.h"
#include <QSettings>
upgReqMenu::upgReqMenu(QWidget *parent)
	: QDialog(parent)
{
	init();
	
	ui.setupUi(this);
	

	//ui.addUpgradeButton
	ui.addUpgradeButton->hide();
	ui.removeUpgradeButton->hide();
//	ui.savetreetest->hide();
//	ui.loadtreetest->hide();

	ui.NButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogSaveButton));
	ui.LButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogOpenButton));
	


	interfaceSetup();
	data_setup = true;
	opcode_combobox_status = false;
	load_from_file_or_first_run = false;


}
#include "serializer.h"
#include "deserializer.h"
#include "TriggerSystem.h"
void upgReqMenu::on_LButton_clicked() {
	ui.upgradeList->setCurrentItem(ui.upgradeList->topLevelItem(0));
	api.fileName = QFileDialog::getOpenFileName(this,
		tr("Open Graft Extension File"), "",
		tr("Brood War extension data plugin file (*.ext);;All Files (*)"));
	if (api.fileName.isEmpty())
	{
		return;
	}
	else {

		QFile file(api.fileName);
		if (!file.open(QIODevice::ReadOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return;
		}
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Try open, clear data [0]"));
		req_tree_data.clear();

		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Clear"));

		std::ifstream temp;
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Open file..."));
		temp.open(api.fileName.toStdString().c_str(), std::ios::binary | std::ios::in);
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("While good, open..."));
		//std::vector<UpgTreeDataElement> lister;
		while (temp.good()) {
			std::vector<UpgTreeDataElement> lister;
			temp >> lister;
			QMessageBox::information(this, tr("Unable to open file"),
				QString::fromStdString("Lister:"));
			for (auto a : lister) {
				int i = 0;
				if (i < 5) {
					/*
					QMessageBox::information(this, tr("Unable to open file"),
						QString::fromStdString("l1: " + WriteInt(a.upgrade_id) + " " + WriteInt(a.used) + " " + WriteInt(a.data.size())));*/
					i++;
				}

				/*for (int i = 0; i < a.data.size(); i++) {
					QMessageBox::information(this, tr("Unable to open file"),
						QString::fromStdString("l2: " + WriteInt(a.data[i].nested.size())));
				}*/
			}
			for (auto a : lister) {
				req_tree_data[a.upgrade_id] = a;
			}
		}
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Finish read, it was good"));
		currentUpgrade = current_default_upg;
		current_upgrade_id = current_default_upg;
		LoadEntry(current_default_upg);
		temp.close();
	}
}

void upgReqMenu::on_viewTreeButton_clicked(){
	int i = 0;
	for (auto a : req_tree_data) {
		
		if (i < 10) {
			QMessageBox::information(this, tr(""),
				"Entry: " + QString::number(a.second.upgrade_id));
		}
		i++;
	}

}
void upgReqMenu::on_NButton_clicked() {
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save Graft Extension File"), "",
		tr("Brood War extension data plugin file (*.ext);;All Files (*)"));
	if (fileName.isEmpty())
		return;
	else {
		QFile file(fileName);
		if (!file.open(QIODevice::WriteOnly)) {
			QMessageBox::information(this, tr("Unable to open file"),
				file.errorString());
			return;
		}
		QMessageBox::information(this, tr(""),
			"Entry: " + QString::number(currentUpgrade));
		SaveEntry(currentUpgrade);
		std::ofstream temp;
		temp.open(fileName.toStdString().c_str(), std::ios::binary | std::ios::out);
		std::vector<UpgTreeDataElement> lister;




		for (auto a : req_tree_data) {
			lister.push_back(a.second);
		}
		/*
		int i = 0;
		for (auto a : lister) {
			if (i < 5) {
				QMessageBox::information(this, tr("Unable to open file"),
					QString::fromStdString("l1->: " + WriteInt(a.upgrade_id) + " " + WriteInt(a.used) + " " + WriteInt(a.data.size())));
			}


			i++;
		}*/
		temp << std::noskipws << lister;
		/*QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Save Lister:"));
		*/
		temp.close();
	}
}
void upgReqMenu::init() {
	data_setup = false;
	current_upgrade_id = 62;
	currentUpgrade = 62;
	load_from_file_or_first_run = true;
	opcode_combobox_status = true;
}

upgReqMenu::~upgReqMenu()
{
}
void upgReqMenu::handleIcons() {
	this->setWindowIcon(this->style()->standardIcon(QStyle::SP_DirIcon));
}

bool branching_opcode(int opcode) {
	switch (opcode) {
	case Requirement::And:
	case Requirement::Or:
	case Requirement::LevelReq:
		return true;
	default:
		break;
	};
	return false;
}
std::string opcode_from_id(int opcode) {
	switch (opcode) {
	case Requirement::And:
		return "And";
	case Requirement::Or:
		return "Or";
	case Requirement::LevelReq:
		return "For Upgrade Level";
	case Requirement::CurrentUnitIs:
		return "Current Unit Is";
	case Requirement::RequiresUpgradeLevel:
		return "Requires Upgrade Level";
	case Requirement::RequiresTech:
		return "Requires Tech";
	case Requirement::IsBusy:
		return "Busy";
	case Requirement::IsConstructingAddon:
		return "Constructing Addon";
	case Requirement::IsResearchingTech:
		return "Researching Tech";
	case Requirement::IsUpgrading:
		return "Upgrading";
	case Requirement::HasAddonOfType:
		return "Addon Of Type";
	case Requirement::HasUnitOfType:
		return "Unit of Type";
	case Requirement::Landed:
		return "Landed";
	case Requirement::IsAddon:
		return "Is Addon";
	case Requirement::HasNuke:
		return "Has Nuke";
	case Requirement::Burrowed:
		return "Burrowed";
	case Requirement::AIControlled:
		return "AI-Controlled";
	case Requirement::CanMove:
		return "Move";
	case Requirement::CanAttack:
		return "Attack";
	case Requirement::IsWorker:
		return "Worker";
	case Requirement::IsTransport:
		return "Transport";
	case Requirement::IsPowerUp:
		return "Powerup";
	case Requirement::IsSubunit:
		return "Subunit";
	case Requirement::HasSpidermines:
		return "Spidermines";
	case Requirement::IsHero:
		return "Hero";
	case Requirement::HasRallyOrRightClick:
		return "Rally Or Rightclick";
	case Requirement::RequiresFaction:
		return "Requires Faction";
	default:
		break;
	};
	return "";
}
std::string opcode_from_id_raw(int opcode) {
	switch (opcode) {
	case Requirement::And:
		return "And";
	case Requirement::Or:
		return "Or";
	case Requirement::LevelReq:
		return "For Upgrade Level";
	case Requirement::CurrentUnitIs:
		return "Current Unit Is";
	case Requirement::RequiresUpgradeLevel:
		return "Requires Upgrade Level";
	case Requirement::RequiresTech:
		return "Requires Tech";
	case Requirement::IsBusy:
		return "Is Busy";
	case Requirement::IsConstructingAddon:
		return "Is Constructing Addon";
	case Requirement::IsResearchingTech:
		return "Is Researching Tech";
	case Requirement::IsUpgrading:
		return "Is Upgrading";
	case Requirement::HasAddonOfType:
		return "Has Addon Of Type";
	case Requirement::HasUnitOfType:
		return "Has Unit of Type";
	case Requirement::Landed:
		return "Landed";
	case Requirement::IsAddon:
		return "Has Addon";
	case Requirement::HasNuke:
		return "Has Nuke";
	case Requirement::Burrowed:
		return "Is Burrowed";
	case Requirement::AIControlled:
		return "Is AI-Controlled";
	case Requirement::CanMove:
		return "Can Move";
	case Requirement::CanAttack:
		return "Can Attack";
	case Requirement::IsWorker:
		return "Is Worker";
	case Requirement::IsTransport:
		return "Is Transport";
	case Requirement::IsPowerUp:
		return "Is Powerup";
	case Requirement::IsSubunit:
		return "Is Subunit";
	case Requirement::HasSpidermines:
		return "Has Spidermines";
	case Requirement::IsHero:
		return "Is Hero";
	case Requirement::HasRallyOrRightClick:
		return "Has Rally Or Rightclick";
	case Requirement::RequiresFaction:
		return "Current Faction Is";
	default:
		break;
	};
	return "";
}
std::string simple_cmpv(int comparison) {
	if (comparison == ReqSetter::Not || comparison==1) {
		return "Is Not";
	}
	return "Is";
}
std::string simple_can(int comparison) {
	if (comparison == ReqSetter::Not) {
		return "Can't";
	}
	return "Can";
}
std::string has_dont_cmpv(int comparison) {
	if (comparison == ReqSetter::Not) {
		return "Don't have";
	}
	return "Has";
}
std::string has_cmpv(int comparison) {
	if (comparison == ReqSetter::Not) {
		return "Has Not";
	}
	return "Has";
}

std::string comp_char(int comparison) {

	switch (comparison) {
	case ReqSetter::GreaterThan:
		return ">";
	case ReqSetter::GreaterThanOrEqual:
		return ">=";
	case ReqSetter::LessThan:
		return "<";
	case ReqSetter::LessThanOrEqual:
		return "<=";
	case ReqSetter::Not:
		return "!=";
	case ReqSetter::Equal:
		return "=";
	default:
		break;
	}
	return "";
}

void upgReqMenu::RefreshReqString() {
	
	if (ui.reqList->currentItem() == NULL) {
		return;
	}
	
	auto data = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));
	ui.reqList->currentItem()->setText(0, requirement_string(ui.opcodeCombobox->currentIndex(), ui.opcodeComparisonCombobox->currentIndex(), 
		ui.opcodeValueCombobox->currentIndex(),
		refresh_string));
	

	if (branching_opcode(ui.opcodeCombobox->currentIndex())) {
		ui.reqList->currentItem()->setIcon(0, this->style()->standardIcon(QStyle::SP_DialogApplyButton));
	}
	else {
		ui.reqList->currentItem()->setIcon(0, QIcon());
	}
	

	/*QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("Refresh? i"+WriteInt(data.id)+" c"+WriteInt(data.current_comparison_index)+" v"
						+	WriteInt(data.current_value_index)+" l"+WriteInt(data.current_level_index)));
	*/
}

QString upgReqMenu::requirement_string(int opcode, int comparison, int value, int level) {

	std::string result;
	std::string str = opcode_from_id(opcode);
	switch (opcode) {
	case Requirement::And:
	case Requirement::Or:
		result = str;
		break;
	case Requirement::LevelReq:
		result = str + " " + WriteInt(level);
		break;
	case Requirement::CurrentUnitIs:
		result = "Current Unit " + simple_cmpv(comparison) + " " + get_unit_name(value);
		break;
	case Requirement::RequiresUpgradeLevel:
		result = "Requires Upgrade " + get_upgrade_name(value) + " " + comp_char(comparison)+WriteInt(level);
		break;
	case Requirement::RequiresTech:
		result = "Tech " + get_tech_name(value) + " " + simple_cmpv(comparison) + " researched";
		break;
	case Requirement::RequiresFaction:
		result = "Faction " + simple_cmpv(comparison) + " " + get_faction_name(value);
		break;
	case Requirement::IsBusy:
	case Requirement::IsConstructingAddon:
	case Requirement::IsResearchingTech:
	case Requirement::IsUpgrading:
	case Requirement::Burrowed:
	case Requirement::AIControlled:
	case Requirement::IsWorker:
	case Requirement::IsTransport:
	case Requirement::IsPowerUp:
	case Requirement::IsSubunit:
	case Requirement::IsHero:
		result = simple_cmpv(comparison) + " " + str;
		break;	
	case Requirement::HasAddonOfType:
	case Requirement::HasUnitOfType:
		result = has_dont_cmpv(comparison)+" "+str + " " + get_unit_name(value);
		break;
	case Requirement::IsAddon:
	case Requirement::HasNuke:
	case Requirement::HasSpidermines:
	case Requirement::HasRallyOrRightClick:
		result = has_dont_cmpv(comparison) + " " + str;
		break;
	case Requirement::CanMove:
	case Requirement::CanAttack:
		result = simple_can(comparison) + " " + str;
		break;
	default:
		break;
	}
	return QString::fromStdString(result);
//	sprintf(buf, )
}

bool has_value_fields(int opcode) {
	switch (opcode) {
	case Requirement::CurrentUnitIs:
	case Requirement::RequiresTech:
	case Requirement::HasAddonOfType:
	case Requirement::HasUnitOfType:
	case Requirement::RequiresUpgradeLevel:
	case Requirement::RequiresFaction:
	case Requirement::LevelReq:
		return true;
	default:
		break;
	};
	return false;
}
u8 get_setting_type(int opcode) {
	switch (opcode) {
	case Requirement::And:
	case Requirement::Or:
	case Requirement::LevelReq:
		return ReqSetterRType::NoSetters;
	case Requirement::CurrentUnitIs:
	case Requirement::RequiresFaction:
	case Requirement::RequiresTech:
	case Requirement::IsBusy:
	case Requirement::IsConstructingAddon:
	case Requirement::IsResearchingTech:
	case Requirement::IsUpgrading:
	case Requirement::HasAddonOfType:
	case Requirement::HasUnitOfType:
	case Requirement::Landed:
	case Requirement::IsAddon:
	case Requirement::HasNuke:
	case Requirement::Burrowed:
	case Requirement::AIControlled:
	case Requirement::CanMove:
	case Requirement::CanAttack:
	case Requirement::IsWorker:
	case Requirement::IsTransport:
	case Requirement::IsPowerUp:
	case Requirement::IsSubunit:
	case Requirement::HasSpidermines:
	case Requirement::IsHero:
	case Requirement::HasRallyOrRightClick:
		return ReqSetterRType::TrueFalse;
	case Requirement::RequiresUpgradeLevel:
		return ReqSetterRType::Comparisons;
	default:
		break;
	};
	return 0;
}

//FRAMEWORK
/*
1 - ADD BUTTON
if and/or -> add inside, else add in current level (check triggerdialog for reference)
if other -> add in same level
all buttons must have qvariants/setdata to keep track of values [done]
(1-2h)

1.5 - GENERIC TEXT WRITE SETTING FROM VALUES

SWITCHING OF UPGRADE IDS MUST CHANGE (SAVE/LOAD) TO

3 - DELETE BUTTON
if not and/or -> just delete
if and or or -> delete with all children
(~1h)

3.5 - add/delete buttons for upgrades (max 255-62)

4 - save:
read opcodes to vector system
serialize it

load:
clear vector
clear everything in tree
load opcodes to vector system (deserialize)
turn vector system to tree/child system

(<4h)

*/

void upgReqMenu::addItemToReqListRoot(int opcode,  int cmp, int val, int lev, bool core) {
	QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui.reqList);
	
	last_item = treeItem;
	
	treeItem->setText(0, requirement_string(opcode, cmp, val, lev));
	ReqOpcode rq;
	rq.id = opcode;
	rq.core_root = core;
	rq.current_comparison_index = cmp;
	rq.current_level_index = lev;
	rq.current_value_index = val;

	if (core) {
		ReqItem req;
		req.id = Requirement::And;
		req.comparison = ReqSetter::Equal;
		req.value = 0;
		req.setter_type = get_setting_type(opcode);
		rq.item = req;
		
		

		for (int i = 0; i < 2; i++) {
			auto N = new QTreeWidgetItem();
			//last_item = N;
			N->setText(0, requirement_string(Requirement::CanAttack,ReqSetter::Equal,0,0));
			ReqOpcode custom;
			custom.id = Requirement::CanAttack;
			custom.core_root = false;
			custom.current_comparison_index = 0;
			custom.current_value_index = 0;
			custom.current_level_index = 0;
			N->setData(0, Qt::UserRole, QVariant::fromValue(custom));
			treeItem->addChild(N);
			treeItem->setExpanded(true);
		}

/*		if (is_folder(ui.triggerListWidget->currentItem())) {
			ui.triggerListWidget->currentItem()->addChild(treeItem);*/
	}
	treeItem->setData(0, Qt::UserRole, QVariant::fromValue(rq));
	auto value = treeItem->data(0, Qt::UserRole);
	auto item = qvariant_cast<ReqOpcode>(value);
	
	if (branching_opcode(opcode)) {
		treeItem->setIcon(0, this->style()->standardIcon(QStyle::SP_DialogApplyButton));
	}
	ui.reqList->resizeColumnToContents(0);
}

void upgReqMenu::PopulateCurrentSecondaryBoxes() {
	comp_combobox_status = true;
	value_combobox_status = true;
	opcode_combobox_status = true;
	data_setup = false;
	ui.opcodeComparisonCombobox->clear();
	ui.opcodeValueCombobox->clear();
	
	
	auto index = ui.opcodeCombobox->currentIndex();
	auto value = ui.opcodeCombobox->itemData(index, Qt::UserRole);
	auto item = qvariant_cast<ReqItem>(value);
	switch (item.setter_type) {
	case ReqSetterRType::NoSetters:
		break;
	case ReqSetterRType::Comparisons:
		ui.opcodeComparisonCombobox->addItem("Equal", ReqSetter::Equal);
		ui.opcodeComparisonCombobox->addItem("Not Equal", ReqSetter::Not);
		ui.opcodeComparisonCombobox->addItem("Greater Than", ReqSetter::GreaterThan);
		ui.opcodeComparisonCombobox->addItem("Greater Than Or Equal", ReqSetter::GreaterThanOrEqual);
		ui.opcodeComparisonCombobox->addItem("Less Than", ReqSetter::LessThan);
		ui.opcodeComparisonCombobox->addItem("Less Than Or Equal", ReqSetter::LessThanOrEqual);
		break;
	case ReqSetterRType::TrueFalse:
/*		QMessageBox::information(this, tr(""),
			QString::fromStdString("truefalse"));*/
		ui.opcodeComparisonCombobox->addItem("True", ReqSetter::Equal);
		ui.opcodeComparisonCombobox->addItem("False", ReqSetter::Not);
		break;
	default:
		break;
	};
	switch (item.id) {
		case Requirement::RequiresFaction:
			//requires faction - overlord-specific
			for (int i = 0; i < factions_info.size(); i++) {
				ui.opcodeValueCombobox->addItem(QString::fromStdString(get_faction_name(factions_info[i].index)), factions_info[i].index);
			}
			break;
		case Requirement::RequiresTech:
			for (int i = 0; i < 44; i++) {
				ui.opcodeValueCombobox->addItem(QString::fromStdString(get_tech_name(i)), i);
			}
			break;
		case Requirement::HasAddonOfType:
		case Requirement::CurrentUnitIs:
		case Requirement::HasUnitOfType:
			for (int i = 0; i < 228; i++) {
				ui.opcodeValueCombobox->addItem(QString::fromStdString(get_unit_name(i)), i);
			}
			break;
		case Requirement::RequiresUpgradeLevel:
		case Requirement::LevelReq:
			for (int i = 0; i < 255; i++) {
				ui.opcodeValueCombobox->addItem(QString::fromStdString(get_upgrade_name(i)), i + 1);
			}
			break;
			
		default:
			break;
	}
	ui.opcodeNumericValueBox->setDisabled(true);
	/*
	QMessageBox::information(this, tr(""),
		QString::fromStdString("Disable valuebox")); */
	if (item.id == Requirement::RequiresUpgradeLevel || item.id == Requirement::LevelReq) {
		ui.opcodeNumericValueBox->setDisabled(false);/*
		QMessageBox::information(this, tr(""),
			QString::fromStdString("Enable valuebox"));*/
	}

	if (ui.opcodeNumericValueBox->isEnabled()) {
		ui.opcodeNumericValueBox->setPlainText("1");
	}
	else {
		ui.opcodeNumericValueBox->setPlainText("");
	}
	opcode_combobox_status = false;
	comp_combobox_status = false;
	value_combobox_status = false;
	data_setup = true;
}


void upgReqMenu::UpdateComboboxesStatus() {
	comp_combobox_status = true;
	value_combobox_status = true;
	data_setup = false;
	auto index = ui.opcodeCombobox->currentIndex();
	auto value = ui.opcodeCombobox->itemData(index, Qt::UserRole);
	auto item = qvariant_cast<ReqItem>(value);
	if (ui.reqList->currentItem() == NULL) {
		ui.opcodeComparisonCombobox->setDisabled(true);
		ui.opcodeValueCombobox->setDisabled(true);
		ui.opcodeNumericValueBox->setDisabled(true);
		ui.opcodeCombobox->setDisabled(true);
		return;
	}
	else {
		ui.opcodeComparisonCombobox->setDisabled(false);
		ui.opcodeValueCombobox->setDisabled(false);
		ui.opcodeNumericValueBox->setDisabled(false);
		ui.opcodeCombobox->setDisabled(false);

		ui.opcodeNumericValueBox->setPlainText("");
	}
	if (!has_value_fields(index)) {
		ui.opcodeNumericValueBox->setDisabled(true);
	}
	

	auto data = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));
	/*if (data.core_root) {
						 QMessageBox::information(this, tr(""),
						 QString::fromStdString("Core root"));
		return;
	}*/
	/*
	QMessageBox::information(this, tr(""),
		QString::fromStdString("Setter type: " + WriteInt(item.setter_type)));*/
	switch (item.setter_type) {
	case ReqSetterRType::NoSetters:
		ui.opcodeComparisonCombobox->setDisabled(true);
		ui.opcodeValueCombobox->setDisabled(true);
		break;
	case ReqSetterRType::Comparisons:
		ui.opcodeComparisonCombobox->setDisabled(false);
		ui.opcodeValueCombobox->setDisabled(false);
		break;
	case ReqSetterRType::TrueFalse:
		ui.opcodeComparisonCombobox->setDisabled(false);
		break;
	default:
		break;
	};
	if (item.id == Requirement::RequiresUpgradeLevel || item.id == Requirement::LevelReq) {
		ui.opcodeNumericValueBox->setDisabled(false);
	}
	else {
		ui.opcodeNumericValueBox->setDisabled(true);
	}
	//
	// SET INDICES DEPENDENT ON TREE ITEM
	//
	if (has_value_fields(item.id)) {
		ui.opcodeValueCombobox->setDisabled(false);
	}
	else {
		ui.opcodeValueCombobox->setDisabled(true);
	}
	comp_combobox_status = false;
	value_combobox_status = false;
	data_setup = true;
	if (data.core_root) {
		ui.opcodeCombobox->setDisabled(true);
	}
}
void upgReqMenu::on_reqList_itemSelectionChanged() {
	opcode_combobox_status = true;
	if (ui.reqList->currentItem() == NULL || last_item == NULL) {
		return;
	}
	auto data_old = qvariant_cast<ReqOpcode>(last_item->data(0, Qt::UserRole));
	auto data_new = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));
	int current_combobox_i = ui.opcodeCombobox->currentIndex();
	int current_combobox_comp_i = ui.opcodeComparisonCombobox->currentIndex();
	int current_combobox_val_i = ui.opcodeValueCombobox->currentIndex();
	QString current_textbox_str = ui.opcodeNumericValueBox->toPlainText();
	ui.opcodeCombobox->setCurrentIndex(data_new.id);
	PopulateCurrentSecondaryBoxes();
	UpdateComboboxesStatus();
	comp_combobox_status = true;
	value_combobox_status = true;
	ui.opcodeComparisonCombobox->setCurrentIndex(data_new.current_comparison_index);
	ui.opcodeValueCombobox->setCurrentIndex(data_new.current_value_index);
	comp_combobox_status = false;
	value_combobox_status = false;
	if (ui.opcodeNumericValueBox->isEnabled()) {

		ui.opcodeNumericValueBox->setPlainText(QString::number(data_new.current_level_index));
	}
	

	if (last_item != ui.reqList->currentItem() && last_item != NULL) {
		data_old.current_comparison_index = current_combobox_comp_i;
		data_old.current_value_index = current_combobox_val_i;
		data_old.current_level_index = StringToInt(current_textbox_str.toStdString());
		data_old.id = current_combobox_i;

		last_item->setData(0, Qt::UserRole, QVariant::fromValue(data_old));
		//QMessageBox::information(this, tr(""), "Set item data");
		//QMessageBox::information(this, tr("Unable to open file"), "Set item data");
	}
	auto data_old1 = qvariant_cast<ReqOpcode>(last_item->data(0, Qt::UserRole));
	auto data_new1 = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));
	/*	QMessageBox::information(this, tr(""), QString::fromStdString("Compare Result: " + WriteInt(data_old1.current_comparison_index) + " " + WriteInt(data_new1.current_comparison_index)
	+" ["+WriteInt(current_combobox_comp_i )+"]"));*/
	last_item = ui.reqList->currentItem();
	opcode_combobox_status = false;
}
void upgReqMenu::PopulatePrimaryCombobox() {
	ui.opcodeCombobox->clear();
	for (int i = 0; i < Requirement::_LAST_; i++) {
		QString Text = QString::fromStdString(opcode_from_id_raw(i));
		ReqItem req;
		req.id = i;
		req.comparison = ReqSetter::Equal;
		req.value = 0;
		req.setter_type = get_setting_type(i);

		ui.opcodeCombobox->addItem(Text, QVariant::fromValue(req));
	}
}
void upgReqMenu::defaultSetup() {
	for (int i = 62; i < 256; i++) {
		req_tree_data[i].upgrade_id = i;
		ReqOpcode o;
		o.id = Requirement::And;
		o.core_root = true;
		o.current_comparison_index = 0;
		o.current_level_index = 0;
		o.current_value_index = 0;
		UpgTreeData e;
		e.current = o;
		req_tree_data[i].data.push_back(e);
	}
}

UpgTreeDataElement::UpgTreeDataElement() {
	used = false;
}
void upgReqMenu::clearFocus() {
	ui.opcodeValueCombobox->clearFocus();
	ui.opcodeComparisonCombobox->clearFocus();
	ui.opcodeCombobox->clearFocus();
	ui.opcodeNumericValueBox->clearFocus();
	ui.opcodeCombobox->hidePopup();
	ui.opcodeComparisonCombobox->hidePopup();
	ui.opcodeValueCombobox->hidePopup();
	
}
void upgReqMenu::on_upgradeList_itemSelectionChanged() {
	//QMessageBox::information(this, tr(""), QString::fromStdString("Itemselch" + WriteInt(code_list_change)));
	if (!code_list_change) {
		return;
	}
	
	auto data = qvariant_cast<int>(ui.upgradeList->currentItem()->data(0, Qt::UserRole));
	clearFocus();
	SaveEntry(currentUpgrade);
	LoadEntry(data);
	currentUpgrade = data;

	

}
void upgReqMenu::interfaceSetup() {
	ui.upgradeList->setHeaderHidden(true);
	ui.upgradeList->setColumnCount(2);
	ui.upgradeList->setRootIsDecorated(false);

	ui.reqList->setHeaderHidden(true);
	ui.reqList->setColumnCount(1);
	ui.reqList->setRootIsDecorated(false);
	//ui.upgradeList->setHorizontalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
	handleIcons();
	code_list_change = false;

	
	for (int i = 62; i <256; i++) {
		addItemToUpgradeList(QString::fromStdString(WriteInt(i)), -1, i);
		
	}
	code_list_change = true;
	defaultSetup();
	on_loadtreetest_clicked();
//	PopulatePrimaryCombobox();
//	PopulateCurrentSecondaryBoxes();
//	addItemToReqListRoot(Requirement::And, 0,0,0,true);

//	addItemToReqListRoot(Requirement::And, 0, 0, 0, true);
/*	ReqOpcode rq;
	rq.id = Requirement::And;
	rq.core_root = true;
	rq.current_comparison_index = 0;
	rq.current_level_index = 0;
	rq.current_value_index = 0;
	
	QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui.reqList);
	treeItem->setText(0, requirement_string(Requirement::And, 0, 0, 0));
	
	treeItem->setData(0, Qt::UserRole, QVariant::fromValue(rq));
	treeItem->setIcon(0, this->style()->standardIcon(QStyle::SP_DialogApplyButton));
	ui.reqList->resizeColumnToContents(0);

	*/

	//	addItemToReqListRoot(Requirement::CurrentUnitIs,ReqSetter::Equal,0,0, false);
//	addItemToReqListRoot(Requirement::RequiresUpgradeLevel, ReqSetter::GreaterThanOrEqual,0,2,false);
//	addItemToReqListRoot(Requirement::RequiresTech,ReqSetter::Equal,0,0, false);
	api.first_open[DialogMenu::UpgReqEditor] = true;
	PopulatePrimaryCombobox();
	PopulateCurrentSecondaryBoxes();
	UpdateComboboxesStatus();
//	QMessageBox::information(this, tr(""), QString::fromStdString("Code list: " + WriteInt(code_list_change)));
}
void upgReqMenu::on_opcodeCombobox_currentIndexChanged(int index) {
	if (opcode_combobox_status) {
		return;
	}

	if (ui.reqList->currentItem() == NULL) {
		return;
	}
	auto data = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));
	
	if (branching_opcode(data.id) && !branching_opcode(index) && ui.reqList->currentItem()->childCount()>0) {
		QMessageBox::information(this, tr("Opcode error"), "Can't change opcode: all subopcodes must be removed first");
		opcode_combobox_status = true;
		ui.opcodeCombobox->setCurrentIndex(data.id);
		opcode_combobox_status = false;
		return;
	}
	PopulateCurrentSecondaryBoxes();
	UpdateComboboxesStatus();
	if (ui.reqList->currentItem() != NULL) {
		
		//data was here originally
		data.id = index;
		ui.reqList->currentItem()->setData(0, Qt::UserRole, QVariant::fromValue(data));
	}

	RefreshReqString();//?
	
}
void upgReqMenu::on_opcodeNumericValueBox_textChanged() {
	if (!data_setup) {
		return;
	}

	QPlainTextEdit *edit = qobject_cast<QPlainTextEdit*>(sender());
	Q_ASSERT(edit);
	auto text = edit->toPlainText();
	auto string = text.toStdString();
	bool change = false;
	if (string.length() > 0) {
		for (int i = 0; i < string.size(); i++) {
			if (!(string[i] >= '0' && string[i] <= '9')) {
				string.erase(string.begin() + i);
				i--;
				change = true;
			}
		}
	
		if (string.length() > 3 || StringToInt(string)>255) {
			string = "255";
			change = true;
		}
	}
	else {
		string = "0";
	}
	
	auto data = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));
	data.current_level_index = StringToInt(string);
	ui.reqList->currentItem()->setData(0, Qt::UserRole, QVariant::fromValue(ReqOpcode()));

	refresh_string = StringToInt(string);

	RefreshReqString();
	if (string == recursion_check) {
		return;
	}
	recursion_check = string;
	if (change) {
		edit->setPlainText(QString::fromStdString(string));
	}
	recursion_check.clear();
	
}

void upgReqMenu::on_opcodeComparisonCombobox_currentIndexChanged(int index) {

	if (value_combobox_status) {
		return;
	}
	if (ui.reqList->currentItem() != NULL) {
		auto data = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));
		data.current_comparison_index = index;
		ui.reqList->currentItem()->setData(0, Qt::UserRole, QVariant::fromValue(data));
	}

	RefreshReqString();

}
void upgReqMenu::on_opcodeValueCombobox_currentIndexChanged(int index) {
	if (value_combobox_status) {
		return;
	}
	if (ui.reqList->currentItem() != NULL) {
		auto data = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));
		data.current_value_index = index;
		ui.reqList->currentItem()->setData(0, Qt::UserRole, QVariant::fromValue(data));
		/*ui.reqList->currentItem()->setText(0, requirement_string(data.id, data.current_comparison_index, data.current_value_index,
			data.current_level_index));*/
//		QMessageBox::information(this, tr(""), "Change val");
	}

	/*
	QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("Refresh4"));*/
	RefreshReqString();
}

ReqOpcode::ReqOpcode() {
	current_comparison_index = 0;
	current_value_index = 0;
}

void upgReqMenu::writeIndexEntry(QTreeWidgetItem *item, std::vector<UpgTreeData> &destination) {

	auto data = qvariant_cast<ReqOpcode>(item->data(0, Qt::UserRole));
	UpgTreeData u;
	/*QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("Write index..."));*/
	u.current = data;
	/*QMessageBox::information(this, tr("Unable to open file"),
		"Push [" + QString::number(data.id) + " " + QString::number(data.core_root) + " " + QString::number(data.current_comparison_index) +
		" " + QString::number(data.current_value_index) + "]  " + QString::number(u.nested.size()));*/
	for (int i = 0; i < item->childCount(); i++) {
	/*
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Write child..."));*/
		writeIndexEntry(item->child(i), u.nested);
	}

	/*
	*/
	destination.push_back(u);
	//correct
	//push [0 1 65535 -1] 2

	//bad
	//push [3 1 0 -1] 2

}

int upgReqMenu::tree_depth(QTreeWidgetItem *item) {
	int d = 0;
	while (item != NULL) {
		d++;
		item = item->parent();
	}
	return d;
}
void upgReqMenu::on_reqAddButton_clicked(){
	QTreeWidgetItem *treeItem;
	bool selected = true;
	if (ui.reqList->currentItem() != NULL) {
		if (!ui.reqList->currentItem()->isSelected()) {
			selected = false;
		}
	}	
	int depth = tree_depth(ui.reqList->currentItem());
	std::string name = "Unknown category";
	
	if (ui.reqList->currentItem() != NULL) {
		treeItem = new QTreeWidgetItem();
		int opcode = Requirement::CurrentUnitIs;
		int cmp = ReqSetter::Equal;
		int val = 0;
		int lev = 0;
		treeItem->setText(0, requirement_string(opcode, cmp, val, lev));
		ReqOpcode rq;
		rq.id = opcode;
		rq.core_root = false;
		rq.current_comparison_index = cmp;
		rq.current_level_index = lev;
		rq.current_value_index = val;
		treeItem->setData(0, Qt::UserRole, QVariant::fromValue(rq));
		ui.reqList->resizeColumnToContents(0);
		auto data = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));

		int current_opcode = data.id;
		if (branching_opcode(current_opcode)) {
			ui.reqList->currentItem()->addChild(treeItem);
		}
		else {
			if (ui.reqList->currentItem()->parent() == NULL) {
				QMessageBox::information(this, tr("Unable to open file"),
					QString::fromStdString("No parent"));
			}
			ui.reqList->currentItem()->parent()->addChild(treeItem);
		}
		/*
		triggerSystemItemSetup(treeItem, QStyle::SP_FileDialogNewFolder, name);
		if (is_folder(ui.triggerListWidget->currentItem())) {
			ui.triggerListWidget->currentItem()->addChild(treeItem);
		}
		else if (ui.triggerListWidget->currentItem()->parent() != NULL) {
			ui.triggerListWidget->currentItem()->parent()->addChild(treeItem);
		}*/
		if (branching_opcode(current_opcode)) {
			ui.reqList->currentItem()->setExpanded(true);
		}
		
	}
}
void upgReqMenu::on_reqRemoveButton_clicked(){
	
	if (ui.reqList->currentItem() == NULL) {
		return;
	}
	auto data = qvariant_cast<ReqOpcode>(ui.reqList->currentItem()->data(0, Qt::UserRole));
	if (data.core_root) {
		return;
	}
	QList<QTreeWidgetItem *> items = ui.reqList->selectedItems();
	QTreeWidgetItem *pp = nullptr;
	if (!items.isEmpty())
	{
		foreach(QTreeWidgetItem *item, items)
		{
			pp = item->parent();
			if (pp != nullptr) {
				pp->removeChild(item);
				delete item;
			}
		}
	}

}

void upgReqMenu::SaveEntry(int e){
	ui.reqList->setCurrentItem(ui.reqList->topLevelItem(0), 0, QItemSelectionModel::Select);//to fix bug with using wrong core root
	req_tree_data[e].data.clear();
	req_tree_data[e].used = ui.usedBox->isChecked();
	for (int i = 0; i < ui.reqList->topLevelItemCount(); i++) {
		writeIndexEntry(ui.reqList->topLevelItem(i), req_tree_data[e].data);
	}
	
}
void upgReqMenu::on_savetreetest_clicked() {
	if (ui.reqList != NULL)
	{
		clearFocus();
		SaveEntry(current_upgrade_id);


	}
}

void upgReqMenu::ReadTreeItem(QTreeWidgetItem* treeItem, std::vector<UpgTreeData> &destination, int i) {
	auto data = destination[i].current;
/*	QMessageBox::information(this, tr("Unable to open file"),
		"Pop " + QString::number(data.id) + " " + QString::number(destination[i].nested.size()));*/
	if (last_item == NULL) {
		last_item = treeItem;
	}	
	treeItem->setText(0, requirement_string(data.id, data.current_comparison_index, 
		data.current_value_index, data.current_level_index));
	if (branching_opcode(data.id)) {
		treeItem->setExpanded(true);
	}
	treeItem->setData(0, Qt::UserRole, QVariant::fromValue(data));
	if (branching_opcode(data.id)) {
		treeItem->setIcon(0, this->style()->standardIcon(QStyle::SP_DialogApplyButton));
	}
	ui.upgradeList->resizeColumnToContents(0);
}
void upgReqMenu::readIndexEntry_parent(std::vector<UpgTreeData> &destination, QTreeWidget* parent) {
	
	for (int i=0;i<destination.size();i++){
		QTreeWidgetItem *treeItem;
		treeItem = new QTreeWidgetItem(parent);
		ReadTreeItem(treeItem, destination,i);
		readIndexEntry_child(destination[i].nested, treeItem);
	}
}

void upgReqMenu::readIndexEntry_child(std::vector<UpgTreeData> &destination, QTreeWidgetItem* parent) {

	for (int i = 0; i<destination.size(); i++) {
		QTreeWidgetItem *treeItem;
		treeItem = new QTreeWidgetItem(parent);
		ReadTreeItem(treeItem, destination, i);
		readIndexEntry_child(destination[i].nested, treeItem);
	}
}

void upgReqMenu::LoadEntry(int e) {
	ui.reqList->clear();
	ui.usedBox->setChecked(req_tree_data[e].used);

	if (req_tree_data.find(e) != req_tree_data.end()) {
		last_item = NULL;
		readIndexEntry_parent(req_tree_data[e].data, ui.reqList);
	}
}

void upgReqMenu::on_loadtreetest_clicked() {

	LoadEntry(current_upgrade_id);
}

void upgReqMenu::addItemToUpgradeList(QString name, int id, int i) {
	QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui.upgradeList);
	treeItem->setText(0, name);
	treeItem->setText(1, QString::fromStdString(get_upgrade_name(i)));
	treeItem->setData(0, Qt::UserRole, QVariant::fromValue(i));
	ui.upgradeList->resizeColumnToContents(0);
	if (i == 62) {
		ui.upgradeList->setCurrentItem(treeItem);
		QMessageBox::information(this, tr(""), "[2] Set CU to " + QString::number(i));
		currentUpgrade = i;
	}
	
	//	ui.reqList->setItemSelected(treeItem, true);
	//	treeItem->setIcon(0, this->style()->standardIcon(icon));
	//	addTreeChild(treeItem, name, "Child_first");

}

