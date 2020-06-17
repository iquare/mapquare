#pragma once
#include <vector>
#include "functions.h"
#include <cctype>
#include "factions.h"
class TriggerSystemItem;
class ConditionLink;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef double f64;
typedef float f32;
typedef unsigned long long u64;


class EditorApi {
public:
	bool first_open[25];
	std::string current_map_file;
	bool file_is_open;
//	HANDLE hMpq = NULL;
//	HANDLE hFile = NULL;
	QString fileName;
};

class Event {
public:
	//add body later
};

class ConditionLink {
public:
	std::vector<ConditionLink> conditions;
	bool is_link;
	int condition_type;//link only
	//condition body - add later
};


class Action {
public:
	ConditionLink condition_base;//condition headers only
	int action_core_type;//if condition list? or normal item
	int jump;//optional if condition is involved, defines length of the jump. Calculate all children in tree
	//action body - add later
	Action();
};
namespace TRG_ERROR {
	enum {
		SUCCESS,
		ERROR_BAD_ROW,
		ERROR_BAD_DEPTH,
		SKIP,
	};
}
class TriggerSystemItem {
public:
	bool is_folder;//only for tree
	bool is_comment;
	std::vector<char> name;
	std::vector<char> comment;//if comment
	//action only
	std::vector<Event> events;//header events
	std::vector<ConditionLink> conditions;//header conditions
	std::vector<Action> action_list;
	//action only
	TriggerSystemItem();
	~TriggerSystemItem();
	TriggerSystemItem(const TriggerSystemItem &n);
	std::string debug_string();

	
//	int add_folder_at(int depth, int row, std::string name);
};

Q_DECLARE_METATYPE(TriggerSystemItem);



struct Escalation {
	int id;
	std::string name;
};

struct UpgDef {
	std::string name;
	int max_level;
	bool escalation = false;
	Escalation lower;
	Escalation upper;
};



class Argument {
public:
	int id;
	//value stuff here
	Argument();
	Argument(int arg) : id(arg) {}
};

class ArgumentConvention {
public:
	std::vector<Argument> args;
};

namespace EventClass {
	enum {
		All,
		General,

		Custom,
		ByText,

		
	};
}

static std::string EventTypeName(int i) {
	switch (i) {
	case EventClass::All:
		return "All";
	case EventClass::Custom:
		return "--Custom--";
	case EventClass::ByText:
		return "--By Text--";
	case EventClass::General:
		return "General";
	default:
		break;
	}
	return "";
}

namespace DefaultFunc {
	enum {
		MapInitialization,
		PeriodicEvent,
	};
}

namespace ArgId {
	enum {
		Arithmetic,
		ArithmeticUintOnly,
		TimeType,
	};
}



class EventConvention : public ArgumentConvention {
public:
	std::vector<char> name;
	int event_class;
	u8 description_args;
	std::vector<char> description;//non-custom only
	EventConvention();
	EventConvention(std::string str, int event_class, int default_id, std::string description);
};

class TriggerSetup {
public:
	//variables - add later
	//local variables - add later
	//
	std::vector<ArgumentConvention> actions;//including macros
	std::vector<ArgumentConvention> conditions;//including macros
	std::vector<EventConvention> events;


	//presets - add later
	void clear();
	TriggerSetup();
};


class EditorInfo {
public:
	int index_count;
	void clear();
	EditorInfo();
};

class EditorTreeElem {
public:
	int trigger_index;
	std::vector<char> name;
	char type;//folder, trigger, comment
};

class FileInfo {
public:
	//add later
	std::vector<EditorTreeElem> structure;
	bool null_root;

	void clear();
};
//
// FOLDERS / TRIGGERS / COMMENTS PRIMARY SYSTEM
//
// SAVE TREES:
// 1. LOOP QT TREES
// 2.2. PUSH TRIGGERS TO TRIGGER VECTOR, USE INDEX
// 2.1. ALSO PUSH TREE ELEMENTS TO TREE, KEEP INDEX FROM UNIT NUMBER TO PRESERVE THE LINK
// 3. PUSH INDIVIDUAL TRIGGERS AS STANDARD VECTOR
// 4. SAVE

// LOAD TREES:
// 1. LOAD
// 2. LOOP TREE SYSTEM IN FILE
// 3. GET TRIGGER INFO BY INDEX, CONVERT TO TEXT
// 4. SORT TEXT TO TREES 
// 5. READ TRIGGERS
// 6. CREATE TREES DEPENDENT ON CONDITIONS, CONVERT TO RICH TEXT USING SAME ALGORITHM USED IN THE TOOL

//  AISE:
// 1. LOAD DATA
// 2. CLEAR FILE AND EDITOR INFO FROM MEMORY
// 3. READ TRIGGER DATA
// 4. SORT TRIGGERS BY EVENTS TO FXHASHMAPS


struct Ext {
	int upgrade_id;//only for reading/writing
	u8 initial;
	u8 max;
	u16 escalation_status = 0xe4;
};

struct FItem {
	u8 iterator;
	u16 index;
};

const int current_default_upg = 62;
extern EditorInfo editor_info;
extern FileInfo file_info;
extern TriggerSetup trigger_setup;
extern std::vector<TriggerSystemItem> trigger_system;
extern std::vector<std::vector<u8>> disposition;
extern std::vector<FItem> faction_list;
//extern std::vector<u8> faction_list;
extern std::vector<std::vector<Ext>> upgrades_data;
extern std::vector<std::vector<u8>> alliances;
extern u32 version;


//definitions: do not save/load
extern EditorApi api;
extern std::ifstream upgrade_def_config, ext_upgrades_data, escalations;
extern std::map<int, UpgDef> definitions, all_upg_definitions;
extern std::vector<FactionData> factions_info;
extern std::map<int, std::vector<std::string>> unit_name, tech_def;
std::pair<int, int> id_from_name(std::string name, std::map<int, std::vector<std::string>> &list);
void loadExternalDefinitions();
bool is_numeric(const std::string& s);
std::string get_unit_name(int id);
std::string get_tech_name(int id);
std::string get_upgrade_name(int id);
std::string get_faction_name(int id);
extern int NNN;