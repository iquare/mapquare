#include "stdafx.h"
#include "stdafx.h"
#include "fstream"
#include "triggerdialog.h"
#include "ui_triggerdialog.h"
#include "TriggerSystem.h"
#include "QMessageBox"
#include <QDialog>
#include <QTranslator>
EditorInfo editor_info;
FileInfo file_info;
TriggerSetup trigger_setup;
std::vector<TriggerSystemItem> trigger_system;
std::vector<std::vector<u8>> disposition;
std::vector<FItem> faction_list;
//std::vector<u8> faction_list;
std::vector<std::vector<Ext>> upgrades_data;
std::vector<std::vector<u8>> alliances;
std::vector<FactionData> factions_info;
u32 version = 1000;

//editor only, do not save/load
int NNN = 0;
std::ifstream upgrade_def_config, ext_upgrades_data, escalations;
std::map<int, std::vector<std::string>> unit_name;
std::map<int, std::vector<std::string>> tech_def;
std::map<int, UpgDef> definitions;
std::map<int, UpgDef> all_upg_definitions;
//

Action::Action() {
	jump = -1;
}

TriggerSystemItem::TriggerSystemItem() {
//	null_root = true;
}

TriggerSystemItem::~TriggerSystemItem() {
	
}

std::string event_description(int func) {
	switch (func) {
	case DefaultFunc::MapInitialization:
		return "Executes on map's initialization";
	case DefaultFunc::PeriodicEvent:
		return "Fires each % %.";
	default:
		break;
	}
	return "";
}

TriggerSetup::TriggerSetup() {

	events.push_back(EventConvention("Periodic Event", EventClass::General, DefaultFunc::PeriodicEvent, 
			event_description(DefaultFunc::PeriodicEvent)));
	events.push_back(EventConvention("Map Initialization", EventClass::General, DefaultFunc::MapInitialization,
		event_description(DefaultFunc::MapInitialization)));
}

EventConvention::EventConvention(){}
EventConvention::EventConvention(std::string str, int event_class, int def, std::string descr) {
	this->event_class = event_class;
	std::copy(descr.begin(), descr.end(), std::back_inserter(description));
	std::copy(str.begin(), str.end(), std::back_inserter(name));
	switch (def) {
	case DefaultFunc::MapInitialization:
		break;
	case DefaultFunc::PeriodicEvent:
		this->args.push_back(ArgId::ArithmeticUintOnly);
		this->args.push_back(ArgId::TimeType);
		break;
	default:
		break;
	}
}

EditorInfo::EditorInfo() {
	index_count = 0;
	

}
void TriggerSetup::clear() {
	events.clear();
	conditions.clear();
	actions.clear();
}

void FileInfo::clear() {
	structure.clear();
}

void EditorInfo::clear() {
	//add later
}

TriggerSystemItem::TriggerSystemItem(const TriggerSystemItem &n) {
	this->is_folder = n.is_folder;
	this->is_comment = n.is_comment;
	this->name = n.name;
	this->comment = n.comment;
	this->events = n.events;
	this->conditions = n.conditions;
	this->action_list = n.action_list;
}

void loadExternalDefinitions() {
	definitions.clear();
	upgrade_def_config.open("data\\unitdef.txt", std::ios::binary | std::ios::in);

	std::ofstream debugtest;
	debugtest.open("test.txt", std::ios::out);
	if (upgrade_def_config.is_open()) {
		std::string line = "";
		
		while (std::getline(upgrade_def_config, line)) {
			if (line == "") {
				continue;
			}
			debugtest << line << '\n';
			trim(line);
			if (starts_with(line, "#")) {
				if (!starts_with(line, "#upgrade")) {
					continue;
				}
			}
			if (starts_with(line, ";") || starts_with(line, "@") || line.length() == 0) {
				continue;
			}
			if (starts_with(line, "upgrade")) {
				std::vector<std::string> strlist = split_by(line, '=');
				if (strlist.size() == 2) {
					std::string header = strlist[0];
					std::vector<std::string> header_break = split_by(header, ' ');
					if (header_break.size() == 2) {
						trim(strlist[1]);
						trim(header_break[1]);
						NNN++;
						all_upg_definitions[StringToInt(strlist[1])].name = (header_break[1]);
					}
				}
				continue;
				//temporary, must be removed later, upgrades.dat reading is required
			}
			if (starts_with(line, "military") || starts_with(line, "building")) {
				std::vector<std::string> strlist = split_by(line, '=');
				if (strlist.size() == 2) {
					std::string header = strlist[0];
					std::vector<std::string> header_break = split_by(header, ' ');
					if (header_break.size() == 2) {
						trim(strlist[1]);
						trim(header_break[1]);
						unit_name[StringToInt(strlist[1])].push_back(header_break[1]);
						/*
						QMessageBox::information(this, tr("Unable to open file"),
						QString::fromStdString(strlist[1] + " " + header_break[1]));*/
					}
				}
				continue;
			}
			if (starts_with(line, "technology")) {
				std::vector<std::string> strlist = split_by(line, '=');
				if (strlist.size() == 2) {
					std::string header = strlist[0];
					std::vector<std::string> header_break = split_by(header, ' ');
					if (header_break.size() == 2) {
						trim(strlist[1]);
						trim(header_break[1]);
						tech_def[StringToInt(strlist[1])].push_back(header_break[1]);
					}
				}
				continue;
			}
			if (!(starts_with(line, "upgrade") || starts_with(line, "#upgrade"))) {
				continue;
			}

			std::vector<std::string> strlist = split_by(line, '=');
			if (strlist.size() == 2) {
				std::string header = strlist[0];
				std::vector<std::string> header_break = split_by(header, ' ');
				if (header_break.size() == 2) {
					trim(strlist[1]);
					trim(header_break[1]);
					definitions[StringToInt(strlist[1])].name = header_break[1];
					all_upg_definitions[StringToInt(strlist[1])].name = header_break[1];
				}
			}
		}
		upgrade_def_config.close();
	}
	ext_upgrades_data.open("data\\upgrades_ext.txt", std::ios::binary | std::ios::in);
	int count = 62;
	if (ext_upgrades_data.is_open()) {
		std::string line;
		while (std::getline(ext_upgrades_data, line)) {
			trim(line);
			if (starts_with(line, "#") || starts_with(line, ";") || starts_with(line, "@") || line.length() == 0) {
				continue;
			}
			std::vector<std::string> command_list = split_by(line, ':');
			if (command_list.size() == 2) {
				std::string name = command_list[0];
				trim(command_list[1]);
				int value = StringToInt(command_list[1]);
				if (name == "max_repeats") {
					definitions[count].max_level = value;
					count++;
				}
			}
		}
		ext_upgrades_data.close();
	}
	escalations.open("data\\escalations.ini", std::ios::binary | std::ios::in);
	if (escalations.is_open()) {

		std::string line;
		while (std::getline(escalations, line)) {
			trim(line);
			if (starts_with(line, "#") || starts_with(line, ";") || starts_with(line, "@") || line.length() == 0) {
				continue;
			}
			std::vector<std::string> command_list = split_by(line, ',');
			trim(command_list);
			/*
			QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("Read escalation"));*/
			if (command_list.size() == 3) {

				std::pair<int, int> lower_id = id_from_name(command_list[1], unit_name);
				std::pair<int, int> upper_id = id_from_name(command_list[2], unit_name);

				int size = unit_name.size();
				definitions[StringToInt(command_list[0])].lower.id = lower_id.first;
				definitions[StringToInt(command_list[0])].lower.name = unit_name[lower_id.first][lower_id.second];

				definitions[StringToInt(command_list[0])].upper.id = upper_id.first;
				definitions[StringToInt(command_list[0])].upper.name = unit_name[upper_id.first][lower_id.second];
				definitions[StringToInt(command_list[0])].escalation = true;
			}
		}
		escalations.close();
	}
}

std::pair<int, int> id_from_name(std::string name, std::map<int, std::vector<std::string>> &list) {
	for (auto a : list) {
		int i = 0;
		for (auto b : a.second) {

			if (name == b) {
				return std::make_pair(a.first, i);
			}
			i++;
		}
	}
	if (is_numeric(name)) {
		return std::make_pair(StringToInt(name), 0);
	}
	return std::make_pair(0, 0);
}

bool is_numeric(const std::string& s)
{
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}


std::string get_unit_name(int id) {
	if (unit_name[id].size() == 0) {
		return "UnitId "+WriteInt(id);
	}
	return unit_name[id][0];
}

std::string get_faction_name(int id) {
	return factions_info[id].name;
}

std::string get_tech_name(int id) {
	if (tech_def[id].size() == 0) {
		return "TechId "+WriteInt(id);
	}
	return tech_def[id][0];
}
std::string get_upgrade_name(int id) {

	std::string result = all_upg_definitions[id].name;
	if (result == "") {
		return "Upgrade " + WriteInt(id);	
	}
	return result;
}