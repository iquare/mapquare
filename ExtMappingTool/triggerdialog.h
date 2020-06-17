#pragma once

#include <QDialog>
#include "ui_triggerdialog.h"

std::string WriteInt(int x);
std::string WriteDouble(double x);
int StringToInt(std::string s);

namespace DialogMenu {
	enum {
		ExtendedTriggers,
		Diplomacy,
		Upgrades,
		Factions,
		Alliances,

		UpgReqEditor,
	};
}

class triggerDialog : public QDialog
{
	Q_OBJECT

public:
	triggerDialog(QWidget *parent = Q_NULLPTR);
	~triggerDialog();
	void fillDataFromInterface();
	void ResetInterface();
private:
	Ui::triggerDialog ui;
	void addTreeChild(QTreeWidgetItem *parent, QString name, QString description);
	void addTreeRoot(QString name, QString description);
	void addTreeRawRoot(QString name, QString description);
	void addTreeCoreRoot(QString name, QStyle::StandardPixmap icon, int id);
	void linkClick(QString qstr);
	std::vector<QLabel*> label_list;
	void interfaceSetup();
	void handleIcons();
	void updateIcons();
	void defineSelection(QTreeWidgetItem *treeItem, bool selected);
	void addTriggerObject(bool is_comment);
	void categorySelectionUpdate();
	void triggerSystemItemSetup(QTreeWidgetItem *treeItem, QStyle::StandardPixmap icon, std::string name);
	bool is_folder(QTreeWidgetItem *treeItem);
	bool is_comment(QTreeWidgetItem *treeItem);
	

private slots:
	void on_triggerMembersWidget_itemDoubleClicked();
	void on_testLabel_linkActivated();
	void on_testButton_clicked();
	void onHyperlinkActivated(const QString & link);
	//
	void on_triggerListWidget_itemClicked();
	void on_triggerListWidget_itemSelectionChanged();
	void on_triggersNewCategoryButton_clicked();
	void on_triggersNewTriggerButton_clicked();
	void on_triggersNewTriggerComment_clicked();
	void on_triggersNewActionButton_clicked();
	void on_triggersNewConditionButton_clicked();
	void on_triggersNewEventButton_clicked();
	//
};
extern triggerDialog* trigger_dialog;