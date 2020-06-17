#include "stdafx.h"
#include "triggerdialog.h"
#include "TriggerSystem.h"
#include <QSettings>

EditorApi api;

void triggerDialog::ResetInterface() {
	ui.triggerListWidget->clear();
	ui.triggerMembersWidget->clear();

	interfaceSetup();
}

void triggerDialog::interfaceSetup() {
	ui.triggerMembersWidget->setHeaderHidden(true);
	ui.triggerListWidget->setHeaderHidden(true);
	ui.triggerMembersWidget->setColumnCount(1);
	ui.triggerListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
	handleIcons();
	addTreeCoreRoot("Events", QStyle::SP_ToolBarVerticalExtensionButton, -1);
	addTreeCoreRoot("Conditions", QStyle::SP_DialogOkButton, -1);
	addTreeCoreRoot("Actions", QStyle::SP_MediaPlay, -1);
	api.first_open[DialogMenu::ExtendedTriggers] = true;
}
triggerDialog::triggerDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	interfaceSetup();
}
#include "extmappingtool.h"

void triggerDialog::fillDataFromInterface() {
	
}

triggerDialog::~triggerDialog()
{
	
	/*
	QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("N? "));*/
	
//	trig_settings.setValue("triggerWindow", QVariant::fromValue(this));

}

void triggerDialog::categorySelectionUpdate() {
	ui.triggersNewTriggerButton->setDisabled(false);
	ui.triggersNewTriggerComment->setDisabled(false);
	auto folder = qvariant_cast<TriggerSystemItem>(ui.triggerListWidget->currentItem()->data(0, Qt::UserRole));
	if (!folder.is_folder) {
		ui.triggersNewEventButton->setDisabled(false);
		ui.triggersNewConditionButton->setDisabled(false);
		ui.triggersNewActionButton->setDisabled(false);
	}
	else {
		ui.triggersNewEventButton->setDisabled(true);
		ui.triggersNewConditionButton->setDisabled(true);
		ui.triggersNewActionButton->setDisabled(true);
	}
}

void triggerDialog::on_triggerListWidget_itemClicked() {
	categorySelectionUpdate();
}

void triggerDialog::on_triggerListWidget_itemSelectionChanged() {
	if (ui.triggerListWidget->children().size()==0 == NULL) {
		ui.triggersNewTriggerButton->setDisabled(true);
		ui.triggersNewTriggerComment->setDisabled(true);
		//
		ui.triggersNewActionButton->setDisabled(true);
		ui.triggersNewConditionButton->setDisabled(true);
		ui.triggersNewEventButton->setDisabled(true);
	}
	
}


void triggerDialog::on_triggersNewActionButton_clicked() {

}
void triggerDialog::on_triggersNewConditionButton_clicked() {

}
#include "add_event.h"
void triggerDialog::on_triggersNewEventButton_clicked() {
	add_event dialog(this);
	/*
	QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("N!?"));*/
	
	if (dialog.exec()) {

	}
	


	/*
	QTreeWidgetItem *treeItem;
	bool selected = true;
	defineSelection(treeItem, selected);
	std::string name = "Map Initialization";
	treeItem = new QTreeWidgetItem();
	treeItem->setIcon(0, (this->style()->standardIcon(QStyle::SP_DriveHDIcon)));
	treeItem->setText(0, QString::fromStdString(name));
//	treeItem->setData(0, Qt::UserRole, QVariant(qRegisterMetaType<TriggerSystemItem>(), nullptr));
//	remove event list from data structure, replace it with setdata stuff
	QTreeWidgetItemIterator it(ui.triggerMembersWidget);
	while (*it) {
		if ((*it)->text(0) == "Events") {
			(*it)->addChild(treeItem);
			(*it)->setExpanded(true);
		}
		++it;
	}*/
}

void triggerDialog::updateIcons() {
	if (true) {
		ui.triggersNewTriggerButton->setDisabled(true);
		ui.triggersNewTriggerComment->setDisabled(true);
		//
		ui.triggersNewEventButton->setDisabled(true);
		ui.triggersNewConditionButton->setDisabled(true);
		ui.triggersNewActionButton->setDisabled(true);
	}
}
void triggerDialog::handleIcons() {

	this->setWindowIcon(this->style()->standardIcon(QStyle::SP_DirIcon));

	ui.triggersNewCategoryButton->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogNewFolder));
	ui.triggersNewTriggerButton->setIcon(this->style()->standardIcon(QStyle::SP_FileIcon));
	ui.triggersNewTriggerComment->setIcon(this->style()->standardIcon(QStyle::SP_FileDialogDetailedView));
	//
	ui.triggersNewEventButton->setIcon(this->style()->standardIcon(QStyle::SP_ToolBarVerticalExtensionButton));
	ui.triggersNewConditionButton->setIcon(this->style()->standardIcon(QStyle::SP_DialogOkButton));
	ui.triggersNewActionButton->setIcon(this->style()->standardIcon(QStyle::SP_MediaPlay));
	//disable comment+add trigger if there are no folders
	updateIcons();


}


void triggerDialog::linkClick(QString qstr) {
	QMessageBox::information(this, tr("Unable to open file"),
		"Swine");
}
#include <sstream>
std::string WriteInt(int x)
{
	std::ostringstream y;
	y << x;
	const std::string xstring(y.str());
	return xstring;
}

int StringToInt(std::string s) {
	return std::atoi(s.c_str());
}

std::string WriteDouble(double x)
{
	std::ostringstream y;
	y << x;
	const std::string xstring(y.str());
	return xstring;
}
void triggerDialog::on_testLabel_linkActivated() {
	QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("Pig"+WriteInt(this->label_list.size())));
}


void triggerDialog::on_testButton_clicked() {
	addTreeRawRoot("N", "hren");
}
void triggerDialog::addTreeCoreRoot(QString name, QStyle::StandardPixmap icon, int id) {
	QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui.triggerMembersWidget);
	
	treeItem->setText(0, name);
	treeItem->setIcon(0,this->style()->standardIcon(icon));
//	addTreeChild(treeItem, name, "Child_first");

}


void triggerDialog::addTreeRawRoot(QString name, QString description) {
	
	QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui.triggerMembersWidget);
	QLabel *label = new QLabel("<i>italics</i>, <b>bold</b>, normal", ui.triggerMembersWidget);
	QString richText("Trigger swine of <a href =\ https://pr0nogo.wikidot.com\" style=\"color: orangered;\">Visit pr0nogo wikidot</a> a");
	label->setText(richText);


	
	ui.triggerMembersWidget->setItemWidget(treeItem, 0, label);
	label_list.push_back(label);
	connect(label, SIGNAL(linkActivated(const QString&)), this, SLOT(onHyperlinkActivated(const QString&)));

	
	//treeItem->setText(0, name);
}

void triggerDialog::onHyperlinkActivated(const QString & link) {
	QMessageBox::information(this, tr("Unable to open file"),
		QString::fromStdString("N?" + WriteInt(this->label_list.size())));

}

void triggerDialog::addTreeRoot(QString name, QString description)
{
	// QTreeWidgetItem(QTreeWidget * parent, int type = Type)
	QTreeWidgetItem *treeItem = new QTreeWidgetItem(ui.triggerMembersWidget);

	// QTreeWidgetItem::setText(int column, const QString & text)
	treeItem->setText(0, name);
//	treeItem->setText(1, description);
	addTreeChild(treeItem, name + "A", "Child_first");
//	addTreeChild(treeItem, name + "B", "Child_second");
}

void triggerDialog::addTreeChild(QTreeWidgetItem *parent,
	QString name, QString description)
{
	// QTreeWidgetItem(QTreeWidget * parent, int type = Type)
	QTreeWidgetItem *treeItem = new QTreeWidgetItem();
//	treeItem->setIcon(0, QIcon("qttest2.ico"));

	// QTreeWidgetItem::setText(int column, const QString & text)
	treeItem->setText(0, name);
//	treeItem->setText(1, description);

	// QTreeWidgetItem::addChild(QTreeWidgetItem * child)
	parent->addChild(treeItem);
	parent->setExpanded(true);
}

void triggerDialog::on_triggerMembersWidget_itemDoubleClicked() {
	
}
int tree_depth(QTreeWidgetItem *item) {
	int d = 0;
	while (item != 0) {
		d++;
		item = item->parent();
	}
	return d;
}


void triggerDialog::defineSelection(QTreeWidgetItem *treeItem, bool selected) {
	if (ui.triggerListWidget->currentItem() != NULL) {
		if (!ui.triggerListWidget->currentItem()->isSelected()) {
			selected = false;
		}
	}
}
//SP_FileDialogDetailedView

bool triggerDialog::is_folder(QTreeWidgetItem *treeItem) {
	auto object = qvariant_cast<TriggerSystemItem>(treeItem->data(0, Qt::UserRole));
	return object.is_folder;
}

bool triggerDialog::is_comment(QTreeWidgetItem *treeItem) {
	auto object = qvariant_cast<TriggerSystemItem>(treeItem->data(0, Qt::UserRole));
	return object.is_comment;
}

void triggerDialog::addTriggerObject(bool is_comment) {
	auto a = &ui.triggerListWidget->currentIndex();
	QTreeWidgetItem *treeItem;
	bool selected = true;
	defineSelection(treeItem, selected);
	int depth = tree_depth(ui.triggerListWidget->currentItem());
	std::string name;
	treeItem = new QTreeWidgetItem();
	QStyle::StandardPixmap icon;
	if (is_comment) {
		icon = QStyle::SP_FileDialogDetailedView;
		name = "Unknown comment";
	}
	else {
		icon = QStyle::SP_FileIcon;
		name = "Unknown trigger";
	}
	triggerSystemItemSetup(treeItem, icon, name);
	auto trigger = qvariant_cast<TriggerSystemItem>(treeItem->data(0, Qt::UserRole));
	trigger.is_folder = false;
	trigger.is_comment = is_comment;
	treeItem->setData(0, Qt::UserRole, QVariant::fromValue(trigger));
	if (ui.triggerListWidget->currentItem() != NULL) {
		if (is_folder(ui.triggerListWidget->currentItem())) {
			ui.triggerListWidget->currentItem()->addChild(treeItem);
			ui.triggerListWidget->currentItem()->setExpanded(true);
		}
		else {
			ui.triggerListWidget->currentItem()->parent()->addChild(treeItem);
		}
	}
}

void triggerDialog::on_triggersNewTriggerComment_clicked() {
	addTriggerObject(true);
}

void triggerDialog::on_triggersNewTriggerButton_clicked() {
	addTriggerObject(false);
}
void triggerDialog::triggerSystemItemSetup(QTreeWidgetItem *treeItem, QStyle::StandardPixmap icon, std::string name) {
	treeItem->setIcon(0, (this->style()->standardIcon(icon)));
	treeItem->setText(0, QString::fromStdString(name));
	treeItem->setData(0, Qt::UserRole, QVariant(qRegisterMetaType<TriggerSystemItem>(), nullptr));
}
void triggerDialog::on_triggersNewCategoryButton_clicked() {
	QTreeWidgetItem *treeItem = nullptr;
	bool selected = true;
	defineSelection(treeItem, selected);
	int depth = tree_depth(ui.triggerListWidget->currentItem());
	std::string name = "Unknown category";	
	if (depth == 0 || !selected) {
		treeItem = new QTreeWidgetItem(ui.triggerListWidget);
		triggerSystemItemSetup(treeItem, QStyle::SP_FileDialogNewFolder, name);
		auto folder = qvariant_cast<TriggerSystemItem>(treeItem->data(0, Qt::UserRole));
		folder.is_folder = true;
		ui.triggerListWidget->setCurrentItem(treeItem);
		categorySelectionUpdate();
	}
	else if(ui.triggerListWidget->currentItem()!=NULL){
		treeItem = new QTreeWidgetItem();
		triggerSystemItemSetup(treeItem, QStyle::SP_FileDialogNewFolder, name);
		if (is_folder(ui.triggerListWidget->currentItem())) {
			ui.triggerListWidget->currentItem()->addChild(treeItem);
		}
		else if(ui.triggerListWidget->currentItem()->parent()!=NULL) {
			ui.triggerListWidget->currentItem()->parent()->addChild(treeItem);
		}
		ui.triggerListWidget->currentItem()->setExpanded(true);
	}
}

