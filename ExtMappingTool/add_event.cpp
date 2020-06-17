#include "stdafx.h"
#include "add_event.h"
#include "TriggerSystem.h"
#include "triggerdialog.h"
add_event::add_event(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
//	ui.eventTypeComboBoxClasses->addItem(listItem);
//	auto trigger = qvariant_cast<TriggerSystemItem>(listItem->data(Qt::UserRole));
//	listItem->setData(Qt::UserRole, QVariant::fromValue(trigger));
	/*
	for (int i = 0; i < 4; i++) {
		int n = pow(2, i + 1);
		QString richText;
		richText = "N";
		ui.eventTypeComboBoxClasses->addItem(richText, QVariant::fromValue(n));
	}*/
	selected_class = EventClass::All;
	classes.clear();
	if (true) {
		//if this is event box, not condition or action
		int ii = 0;
		classes[EventClass::All] = -1;
		classes[EventClass::ByText] = -1;

		for (auto a : trigger_setup.events) {
			classes[a.event_class] = ii;
			ii++;
		}/*
		QMessageBox::information(this, tr("Unable to open file"),
			QString::fromStdString("N? " + WriteInt(trigger_setup.events.size())));
		*/
		PopulateClassCombobox(ComboboxType::EVENT);
		PopulateCombobox(ComboboxType::EVENT);
		ResetArguments();
		DrawRichTextWithArgs();
	}	
	/*
	ui.eventTypeComboBoxClasses->addItem(richText, );*/
}

void add_event::PopulateClassCombobox(int box_type) {
	if (true) {
		ui.eventTypeComboBoxClasses->clear();
		for (auto a : classes) {
			QString Text = QString::fromStdString(EventTypeName(a.first));
			ObjectSel sel;
			sel.element_class = a.first;
			ui.eventTypeComboBoxClasses->addItem(Text, QVariant::fromValue(sel));
		}
	}
}

void add_event::ComposeRichText(int type_class) {

}

void add_event::PopulateCombobox(int box_type) {
	auto index = ui.eventTypeComboBoxClasses->currentIndex();
	auto value = ui.eventTypeComboBoxClasses->itemData(index, Qt::UserRole);
	auto item_class = qvariant_cast<ObjectSel>(value);
	if (true) {
		int i = 0;
		for (auto a : trigger_setup.events) {
			if ((item_class.element_class == EventClass::All || a.event_class == item_class.element_class)
				&& true /*text match*/) {
				QString Text = QString::fromStdString(std::string(a.name.begin(), a.name.end()));
				ObjectSel sel;
				sel.i = i;
				ui.eventListBox->addItem(Text, QVariant::fromValue(sel));
			}
			i++;
		}
	}
}
std::string VToString(std::vector<char> v_string) {
	std::string result = std::string(v_string.begin(), v_string.end());
	return result;
}
std::string format_arg_string(std::string raw_string, std::vector<std::string> args) {
	std::string result;
	int a = 0;
	for (int i = 0; i < raw_string.size(); i++) {
		if(raw_string[i]=='%'){
			result += args[a];
			a++;
		}
		else {
			result.push_back(raw_string[i]);
		}
	}
	return result;
}

#include "sstream"

ArgPreset::ArgPreset() {
	defined = false;
	current_value = 0;//for lists
	Double = 0.0;
	Integer = 0.0;
	Uinteger = 0.0;
	is_variable = false;
}

void add_event::ResetArguments() {
	current_arguments.clear();
	auto index = ui.eventListBox->currentIndex();
	auto value = ui.eventListBox->itemData(index, Qt::UserRole);
	auto item = qvariant_cast<ObjectSel>(value);
	auto object = trigger_setup.events[item.i];
	for (auto a : object.args) {
		ArgPreset p;
		p.control_id = a.id;
		switch (a.id) {
		case ArgId::Arithmetic:
			p.is_numeric = true;
			p.used_values.push_back(ArgValue::Double);
			p.used_values.push_back(ArgValue::Uinteger);
			p.used_values.push_back(ArgValue::Integer);
			p.browsing_type = BrowsingType::SetValue;
			break;
		case ArgId::ArithmeticUintOnly:
			p.is_numeric = true;
			p.used_values.push_back(ArgValue::Uinteger);
			p.browsing_type = BrowsingType::SetValue;
			break;
		case ArgId::TimeType:
			p.is_numeric = false;
			p.used_values.push_back(ArgValue::Frames);
			p.used_values.push_back(ArgValue::Seconds);
			p.browsing_type = BrowsingType::SelectFromList;
			p.defined = true;

		default:
			break;
		}
		current_arguments.push_back(p);
	}
}
std::string argtypestringname(int arg) {
	switch (arg) {
	case ArgValue::Frames:
		return "frames";
		break;
	case ArgValue::Seconds:
		return "seconds";
		break;
	default:
		break;
	}
	return "";
}

std::string add_event::current_argument_color(int i) {
	auto arg = current_arguments[i];
	if (!arg.defined) {
		return "orangered";
	}
	return "royalblue";
}

std::string add_event::current_argument_string(int i) {
	auto arg = current_arguments[i];
	if (!arg.defined) {
		return "Define";
	}
	else {
		
		auto current = arg.used_values[arg.current_value];
		if (arg.is_variable) {
			//define later
			//add variable support to events/triggers
		}
		switch (arg.control_id) {
		default:
		case ArgId::Arithmetic:
		case ArgId::ArithmeticUintOnly:
			if (current== ArgValue::Double) {
				return WriteDouble(arg.Double);
			}
			else if (current == ArgValue::Uinteger) {
				return WriteInt(arg.Uinteger);
			}
			else if (current == ArgValue::Integer) {
				return WriteInt(arg.Integer);
			}
			break;
		case ArgId::TimeType:
			return argtypestringname(arg.used_values[arg.current_value]);
			break;
		}
	}
	return "";
}
void add_event::DrawRichTextWithArgs() {
	auto index = ui.eventListBox->currentIndex();
	auto value = ui.eventListBox->itemData(index, Qt::UserRole);
	auto item = qvariant_cast<ObjectSel>(value);
	if (true) {
		if (trigger_setup.events[item.i].event_class == EventClass::Custom) {
			//implement later
			return;
		}
		else {
			std::string converted = VToString(trigger_setup.events[item.i].description);
			auto object = trigger_setup.events[item.i];
			int arguments = object.description_args;
			int itr = 0;
			std::string check = VToString(object.name);
			/*
			QMessageBox::information(this, tr("Unable to open file"),
				QString::fromStdString("N?" + WriteInt(arguments) + " " + WriteInt(item.i) + " "+ check));*/
			std::vector<std::string> arg_strings;
			while (arguments > 0) {
				std::stringstream ss;			
//				ss << "<a href =\ n.\" style=\"color: orangered;\">";
				ss << "<a href =\ n.\" style=\"color: ";
				ss << current_argument_color(itr);
				ss << ";\">";

				//ss << "Argument";
				ss << current_argument_string(itr);
				ss << "</a>";
				arg_strings.push_back(ss.str());
				arguments--;
				itr++;
			}
			std::string result = format_arg_string(VToString(object.description), arg_strings);
			QString richText(QString::fromStdString(result));
			ui.clickableSetEventSection->setText(richText);
		}
	}
}

add_event::~add_event()
{
}




void add_event::on_okButton_clicked(){

}
void add_event::on_cancelButton_clicked(){

	this->close();
}