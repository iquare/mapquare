#include "stdafx.h"
#include "serializer.h"

std::ostream& operator<<(std::ostream& out, const EditorInfo& obj) {
	out.write(reinterpret_cast<const char*>(&obj.index_count), sizeof(obj.index_count));
	return out;
}
std::ostream& operator<<(std::ostream& out, const EditorTreeElem& obj) {
	out.write(reinterpret_cast<const char*>(&obj.trigger_index), sizeof(obj.trigger_index));
	out << obj.name;
	out.write(reinterpret_cast<const char*>(&obj.type), sizeof(obj.type));
	return out;
}

std::ostream& operator<<(std::ostream& out, const FileInfo& obj) {
	out << obj.structure;
	out.write(reinterpret_cast<const char*>(&obj.null_root), sizeof(obj.null_root));

	return out;
}

std::ostream& operator<<(std::ostream& out, const Argument& obj) {
	out.write(reinterpret_cast<const char*>(&obj.id), sizeof(obj.id));
	return out;
}

std::ostream& operator<<(std::ostream& out, const ArgumentConvention& obj) {
	out << obj.args;
	return out;
}
std::ostream& operator<<(std::ostream& out, const EventConvention& obj) {
	out << obj.args;
	out << obj.name;
	out.write(reinterpret_cast<const char*>(&obj.event_class), sizeof(obj.event_class));
	return out;
}

std::ostream& operator<<(std::ostream& out, const ConditionLink& obj) {
	out << obj.conditions;
	out.write(reinterpret_cast<const char*>(&obj.is_link), sizeof(obj.is_link));
	out.write(reinterpret_cast<const char*>(&obj.condition_type), sizeof(obj.condition_type));
	return out;
}

std::ostream& operator<<(std::ostream& out, const Action& obj) {
	ConditionLink condition_base;//condition headers only
	out << obj.condition_base;
	out.write(reinterpret_cast<const char*>(&obj.action_core_type), sizeof(obj.action_core_type));
	out.write(reinterpret_cast<const char*>(&obj.jump), sizeof(obj.jump));
	return out;
}

std::ostream& operator<<(std::ostream& out, const Event& obj) {
	//add later
	return out;
}

std::ostream& operator<<(std::ostream& out, const TriggerSetup& obj) {
	out << obj.actions;
	out << obj.conditions;
	out << obj.events;
	return out;
}

std::ostream& operator<<(std::ostream& out, const UpgTreeDataElement& obj) {
	out << obj.data;
	out.write(reinterpret_cast<const char*>(&obj.upgrade_id), sizeof(obj.upgrade_id));
	out.write(reinterpret_cast<const char*>(&obj.used), sizeof(obj.used));
	return out;
}
std::ostream& operator<<(std::ostream& out, const UpgTreeData& obj) {
	out << obj.current;
	out << obj.nested;
	return out;
}

std::ostream& operator<<(std::ostream& out, const ReqOpcode& obj) {
	out.write(reinterpret_cast<const char*>(&obj.core_root), sizeof(obj.core_root));
	out.write(reinterpret_cast<const char*>(&obj.current_comparison_index), sizeof(obj.current_comparison_index));
	out.write(reinterpret_cast<const char*>(&obj.current_level_index), sizeof(obj.current_level_index));
	out.write(reinterpret_cast<const char*>(&obj.current_value_index), sizeof(obj.current_value_index));
	out.write(reinterpret_cast<const char*>(&obj.id), sizeof(obj.id));
	out << obj.item;
	return out;
}

std::ostream& operator<<(std::ostream& out, const ReqItem& obj) {
	out.write(reinterpret_cast<const char*>(&obj.comparison), sizeof(obj.comparison));
	out.write(reinterpret_cast<const char*>(&obj.id), sizeof(obj.id));
	out.write(reinterpret_cast<const char*>(&obj.setter_type), sizeof(obj.setter_type));
	out.write(reinterpret_cast<const char*>(&obj.value), sizeof(obj.value));
	return out;
}

std::ostream& operator<<(std::ostream& out, const TriggerSystemItem& obj) {
	out.write(reinterpret_cast<const char*>(&obj.is_folder), sizeof(obj.is_folder));
	out.write(reinterpret_cast<const char*>(&obj.is_comment), sizeof(obj.is_comment));
	out << obj.name;
	out << obj.comment;
	out << obj.events;
	out << obj.conditions;
	out << obj.action_list;
	return out;
}

std::ostream& operator<<(std::ostream& out, const Ext& obj) {
	out.write(reinterpret_cast<const char*>(&obj.upgrade_id), sizeof(obj.upgrade_id));
	out.write(reinterpret_cast<const char*>(&obj.initial), sizeof(obj.initial));
	out.write(reinterpret_cast<const char*>(&obj.max), sizeof(obj.max));
	out.write(reinterpret_cast<const char*>(&obj.escalation_status), sizeof(obj.escalation_status));
	return out;
}
#include <iostream>
#include <fstream>
#include <ostream>
#include "triggerdialog.h"
std::ostream& operator<<(std::ostream& out, const FItem& obj) {
	out.write(reinterpret_cast<const char*>(&obj.iterator), sizeof(obj.iterator));
	out.write(reinterpret_cast<const char*>(&obj.index), sizeof(obj.index));
	std::ofstream log;
	log.open("log.log", std::ios::app);
	log << WriteInt(obj.iterator) << " " << WriteInt(obj.index) << '\n';
	log.close();
	return out;
}
