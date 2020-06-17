#pragma once
#include <QDialog>
#include "ui_upgreqmenu.h"
#include "TriggerSystem.h"
namespace ReqSetter {
	enum {

		Equal,//true
		Not,//false

		GreaterThan,
		GreaterThanOrEqual,
		LessThan,
		LessThanOrEqual,

	};
}
namespace ReqSetterRType {
	enum {
		NoSetters,
		TrueFalse,
		Comparisons,
	};
}

struct ReqItem {
	u8 comparison;
	u16 id;
	u8 setter_type;
	u32 value;
	
};
Q_DECLARE_METATYPE(ReqItem);

struct ReqOpcode {
	bool core_root;
	u16 current_comparison_index;
	u16 current_level_index;
	int current_value_index;
	u16 id;
	ReqItem item;

	ReqOpcode();
};
Q_DECLARE_METATYPE(ReqOpcode);

namespace Requirement {
	enum {
		And,//tree branching
		Or,//tree branching
		LevelReq,//tree branching
		//
		CurrentUnitIs,
		RequiresUpgradeLevel,
		RequiresTech,
		IsBusy,
		IsConstructingAddon,
		IsResearchingTech,
		IsUpgrading,
		HasAddonOfType,
		HasUnitOfType,
		Landed,
		//
		IsAddon,
		HasNuke,
		Burrowed,
		AIControlled,
		CanMove,
		CanAttack,
		IsWorker,
		IsTransport,
		IsPowerUp,
		IsSubunit,
		HasSpidermines,
		IsHero,
		HasRallyOrRightClick,
		RequiresFaction,
		//
		_LAST_,
	};
}

class UpgTreeData {
public:
	ReqOpcode current;
	std::vector<UpgTreeData> nested;	
};

class UpgTreeDataElement {
public:
	std::vector<UpgTreeData> data;
	int upgrade_id;//already using map, so it's not required but required for serialization in/from vector
	bool used;
	UpgTreeDataElement();
};

class upgReqMenu : public QDialog
{
	Q_OBJECT

public:
	upgReqMenu(QWidget *parent = Q_NULLPTR);
	~upgReqMenu();
	bool load_from_file_or_first_run;
	int currentUpgrade;
	bool code_list_change;
	int current_upgrade_id;
	void LoadEntry(int e);
	void SaveEntry(int e);
	void init();
	std::map<int, UpgTreeDataElement> req_tree_data;//for saving
private:
	Ui::upgReqMenu ui;
	QTreeWidgetItem* last_item;
	
	bool data_setup;
	void interfaceSetup();
	void handleIcons();
	void addItemToUpgradeList(QString name, int id, int i);
	void addItemToReqListRoot(int opcode, int cmp, int val, int lev, bool core);
	void PopulatePrimaryCombobox();
	void PopulateCurrentSecondaryBoxes();
	void UpdateComboboxesStatus();
	void writeIndexEntry(QTreeWidgetItem *i, std::vector<UpgTreeData> &destination);
	void readIndexEntry_parent(std::vector<UpgTreeData> &destination, QTreeWidget* parent);
	void readIndexEntry_child(std::vector<UpgTreeData> &destination, QTreeWidgetItem* parent);
	void ReadTreeItem(QTreeWidgetItem* item, std::vector<UpgTreeData> &destination, int itr);
	void clearFocus();
	std::string recursion_check;
	int refresh_string;
	bool value_combobox_status;
	bool comp_combobox_status;
	bool opcode_combobox_status;
	
	
	
	int tree_depth(QTreeWidgetItem *item);
	
	QString requirement_string(int opcode, int comparison, int value, int level);
	void RefreshReqString();
	void defaultSetup();
private slots:
	void on_opcodeCombobox_currentIndexChanged(int index);
	void on_opcodeComparisonCombobox_currentIndexChanged(int index);
	void on_opcodeValueCombobox_currentIndexChanged(int index);
	void on_opcodeNumericValueBox_textChanged();
	void on_reqList_itemSelectionChanged();
	void on_savetreetest_clicked();
	void on_loadtreetest_clicked();
	void on_reqAddButton_clicked();
	void on_reqRemoveButton_clicked();
	void on_upgradeList_itemSelectionChanged();
	void on_NButton_clicked();
	void on_LButton_clicked();
	void on_viewTreeButton_clicked();
};

std::string opcode_from_id(int opcode);


