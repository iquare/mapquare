#pragma once

#include <QDialog>
#include "ui_factions.h"
#include "fstream"


struct FactionData {
	int index;
	std::string name;
	int color;
	int iterator;
	int icon;
	int stringId;
};

class factions : public QDialog
{
	Q_OBJECT

public:
	factions(QWidget *parent = Q_NULLPTR);
	~factions();
	void ResetInterface(bool read_from_data);
	void fillDataFromInterface();
private:
	Ui::factions ui;
	QVector<QLabel*> playerLabels;
	QVector<QComboBox*> factionBoxes;
	void setInterface(bool read_from_data);
	
	void FillFactionCombobox(QComboBox* box);
	void load_factions_from(std::string filename);
	
};

