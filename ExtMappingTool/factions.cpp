#include "stdafx.h"
#include "factions.h"
#include "functions.h"
#include "triggerdialog.h"
#include "TriggerSystem.h"

factions::factions(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	api.first_open[DialogMenu::Factions] = true;
	setInterface(false);
	this->setWindowIcon(this->style()->standardIcon(QStyle::SP_DialogOkButton));
}
bool sort_by_id(const FactionData &l, const FactionData &r) {
	return (l.index < r.index);
}
void factions::load_factions_from(std::string filename) {
	std::ifstream factions;
	factions.open(filename, std::ios::binary | std::ios::in);
	int itr = 0;
	if (factions.is_open()) {
		std::string line;
		while (std::getline(factions, line)) {
			trim(line);
			if (starts_with(line, "#") || starts_with(line, ";") || starts_with(line, "@") || line.length() == 0) {
				continue;
			}
			std::vector<std::string> strlist = split_by(line, '\t');
			if (strlist.size() != 5) {
				QMessageBox::information(this, tr("Unable to open file"),
					QString::fromStdString("FACTIONS: Incorrect faction line " + line));
			}
			else {

//				#index, name, color(bw index)
				FactionData f;
				f.name = strlist[1];
				f.color = StringToInt(strlist[2]);
				f.index = StringToInt(strlist[0]);
				f.icon = StringToInt(strlist[3]);
				f.stringId = StringToInt(strlist[4]);

				factions_info.push_back(f);
			}
		}
		factions.close();
	}
	std::sort(factions_info.begin(), factions_info.end(), sort_by_id);
}

void factions::FillFactionCombobox(QComboBox* box) {
	//
	std::vector<std::string> default_factions;
	factions_info.clear();
	load_factions_from("data\\factions.txt");
	/*
	default_factions.push_back("No Faction");
	default_factions.push_back("Kel-Morian Combine");
	default_factions.push_back("Tal Qirat Guild");
	default_factions.push_back("Alpha Squadron (Confederacy)");//confed
	default_factions.push_back("Gamma Squadron (Confederacy)");//confed
	default_factions.push_back("Nova Squadron (Confederacy)");//confed
	default_factions.push_back("Epsilon Squadron (Confederacy)");//confed
	default_factions.push_back("Omega Squadron (Confederacy)");//confed
	default_factions.push_back("Colonial Militia (Confederacy)");//confed
	default_factions.push_back("Umojans");
	default_factions.push_back("Pirates (Independents)");
	default_factions.push_back("Miners (Independents)");
	default_factions.push_back("Scavengers (Independents)");
	default_factions.push_back("Mercenaries (Independents)");*/
	iter_enumerated(factions_info, [&](FactionData f, int i) {
		box->addItem(QString::fromStdString(f.name), QVariant::fromValue(std::make_pair(i,f.index)));
		//box->addItem(QString::fromStdString(f.name), QVariant::fromValue(i));
	});
}

void factions::setInterface(bool read_from_data) {
	ui.playerPlaceholder->hide();
	ui.selectFactionComboboxPlaceholder->hide();
	for (int i = 0; i < 8; i++) {
		auto player_a = new QLabel(QString("Player %1").arg(i + 1), this);
		playerLabels.push_back(player_a);
		player_a->setGeometry(ui.playerPlaceholder->x(), ui.playerPlaceholder->y() + 40 * i, 47, 13);
		auto combobox = new QComboBox(this);
		factionBoxes.push_back(combobox);
		auto x = ui.selectFactionComboboxPlaceholder->x();
		auto y = ui.selectFactionComboboxPlaceholder->y();
		combobox->setGeometry(x, y+40*i, 200, 20);
		
		FillFactionCombobox(combobox);
		if (read_from_data) {
/*			QMessageBox::information(this, tr("Unable to open file"),
				QString::fromStdString("FACTIONS: Read Data "+WriteInt(faction_list[i])));*/
			combobox->setCurrentIndex(faction_list[i].iterator);
//			combobox->setCurrentIndex(faction_list[i]);
		}
		//combobox->setCurrentIndex(4);
	}
}


void factions::fillDataFromInterface() {
	faction_list.clear();
	faction_list.resize(8);
	int i = 0;
	QImage image(3, 3, QImage::Format_Indexed8);
	QRgb value;
	image.setPixel(1, 1, 0xffffff);
	value = qRgb(122, 163, 39); // 0xff7aa327
	image.setColor(0, value);

	value = qRgb(237, 187, 51); // 0xffedba31
	image.setColor(1, value);

	value = qRgb(189, 149, 39); // 0xffbd9527
	image.setColor(2, value);

	image.setPixel(0, 1, 0);
	image.setPixel(1, 0, 0);
	image.setPixel(1, 1, 2);
	image.setPixel(2, 1, 1);

	for (auto &a : factionBoxes) {
		auto value = a->itemData(a->currentIndex(), Qt::UserRole);
		auto itr = qvariant_cast<std::pair<int, int>>(value).first;
		auto index = qvariant_cast<std::pair<int, int>>(value).second;
//		auto i = qvariant_cast<int>(value);
//		faction_list[i] = i;
		faction_list[i].iterator = (u8)itr;
		faction_list[i].index = (u16)index;
		std::ofstream log;
		log.open("log.log", std::ios::app);
		log << "Fill data from boxes: " << WriteInt(itr) << " " << WriteInt(index) << '\n';
		log << "Check: " << WriteInt(faction_list[i].iterator) << " " << WriteInt(faction_list[i].index) <<
				" at [" << WriteInt(i) << "] " << '\n';
		log.close();
		i++;
	}
	std::ofstream log;
	log.open("log.log", std::ios::app);
	log << "Check again\n";
	int it = 0;
	for (auto a : faction_list) {
		log << "Data: " << WriteInt(a.index) << " " << WriteInt(a.iterator) << " at["<<WriteInt(it) << "] " << '\n';
		it++;
	}
	log.close();
}

void factions::ResetInterface(bool read_from_data) {
	for (auto a : playerLabels) {
		a->deleteLater();
	}
	for (auto a : factionBoxes) {
		a->deleteLater();
	}
	playerLabels.clear();
	factionBoxes.clear();
	setInterface(read_from_data);
}

factions::~factions()
{
}
