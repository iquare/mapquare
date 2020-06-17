#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_extmappingtool.h"
#include "triggerdialog.h"
#include <fstream>
#include <ostream>
#include "diplomacy.h"
#include "factions.h"
#include "upgrade_menu.h"
#include "alliances.h"
#include "chk.h"
#include "upgreqmenu.h"
class ExtMappingTool : public QMainWindow
{
	Q_OBJECT

public:
	void handleIcons();
	triggerDialog trg;
	diplomacy diplomacy_dlg;
	factions factionDlg;
	upgrade_menu extendedUpg;
	Alliances allianceDlg;
	upgReqMenu upgReqMenu;
	chk Chk;

	ExtMappingTool(QWidget *parent = Q_NULLPTR);
	void ReadChunk(int chk_id, std::ifstream &temp, u32 header);


private:
	Ui::ExtMappingToolClass ui;
	int header_match(u32 str);
	void parse_chk(std::string name);
	void FillDataFromInterface();
	void ClearData();
	void DeleteTempFile();
	void WriteTempArchive();
	void ResetInterface(bool read_from_data);
	void ReadTempArchiveToData(std::string);
	void SetTempArchiveToInterface();
	void ClearEditorReadWriteData();
private slots:
	void on_saveButton_clicked();
	void on_loadButton_clicked();
	void on_triggersButton_clicked();
	void on_diplomacyButton_clicked();
	void on_factionsButton_clicked();
	void on_upgradesButton_clicked();
	void on_upgReqButton_clicked();
	void on_alliancesButton_clicked();
	//graftquare
	void on_saveUpgButton_clicked();
	void on_loadUpgButton_clicked();

};


