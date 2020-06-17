#pragma once

#include <QDialog>
#include "ui_upgrade_menu.h"
#include "TriggerSystem.h"
#include <map>
#include <fstream>

class upgrade_menu : public QDialog
{
	Q_OBJECT

public:
	upgrade_menu(QWidget *parent = Q_NULLPTR);
	~upgrade_menu();
	void ResetInterface(bool read_from_data);
	void fillDataFromInterface();
private:
	Ui::upgrade_menu ui;
	QScrollArea* scroll_area;
	QVBoxLayout *dialog_layout;
	QVBoxLayout *d_layout;
	QVector<QLabel*> upgradeNameLabels;
	QVector<QLabel*> upgradeHeaderLabels;
	std::vector<QPlainTextEdit*> initialLevelBoxes;
	std::vector<QPlainTextEdit*> maxLevelBoxes;
	QVector<QLabel*> initialLevelLabels;
	QVector<QLabel*> maxLevelLabels;
	std::vector<QRadioButton*> lowerEscalationButtons;
	std::vector<QRadioButton*> upperEscalationButtons;
	QVector<QButtonGroup*> buttonGroups;
	QVector<QLabel*> dummy_labels;
	std::vector<QPushButton*> playerButtons;
	
	std::map<int, std::map<int, Ext>> internal_info;//first key - player, second key - upgrade id
	
	std::string maxBoxRecursionCheck;
	std::string iniBoxRecursionCheck;
	int currentPlayer = 0;
	//
	void setInterface(bool read_from_data);
	
	void prepareWidgetForLayout(QWidget *widget, int pos_x, int pos_y, int bound_x, int bound_y);
	void SetDataValuesFromArchive();
	void recolorButtons();
	void PopulatePrimaryCombobox();
	void colorize_button(QPushButton* player, u32 color);
private slots:
	void maxBoxTextChanged();
	void initialBoxTextChanged();
	void playerButtonClicked();
	void escButtonLowClicked();
	void escButtonHighClicked();
	
};

bool is_numeric(const std::string& s);
