#pragma once
#include <QDialog>
#include <map>
#include "ui_add_event.h"
#include "TriggerSystem.h"

namespace ComboboxType {
	enum {
		EVENT,
		CONDITION,
		ACTION
	};
}

class ArgPreset;

namespace ArgValue {
	enum {
		Double,
		Integer,
		Uinteger,
		Name,
		Frames,
		Seconds,
	};
}
std::string argtypestringname(int arg);

namespace BrowsingType {
	enum {
		SetValue,
		SelectFromList,
	};
}

class ArgPreset {
public:
	bool is_numeric;
	f64 Double;
	u32 Uinteger;
	int Integer;
	std::string name;
	int control_id;
	bool is_variable;
	//
	int browsing_type;
	
	bool defined;
	//
	ArgPreset();
	std::vector<ArgPreset> composed;
	std::vector<int> used_values;
	
	int current_value;//list defining
};
class add_event : public QDialog
{
	Q_OBJECT

public:
	add_event(QWidget *parent = Q_NULLPTR);
	~add_event();

private:
	Ui::add_event ui;
	std::map<int, int> classes;//key is event class, value is events
	std::vector<ArgPreset> current_arguments;
	int selected_class;
	void PopulateCombobox(int box_type);
	void PopulateClassCombobox(int box_type);
	void ComposeRichText(int type_class);
	void ResetArguments();
	std::string current_argument_color(int i);
	std::string current_argument_string(int i);
private slots:
	void on_okButton_clicked();
	void on_cancelButton_clicked();
	void DrawRichTextWithArgs();

};

class ObjectSel {
public:
	int element_class;
	int i;
};
Q_DECLARE_METATYPE(ObjectSel);