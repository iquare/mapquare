#include "stdafx.h"
#include "deserializer.h"
#include "triggerdialog.h"
std::istream& operator >> (std::istream& in, Ext& obj) {
	in.read(reinterpret_cast<char*>(&obj.upgrade_id), sizeof(obj.upgrade_id));
	in.read(reinterpret_cast<char*>(&obj.initial), sizeof(obj.initial));
	in.read(reinterpret_cast<char*>(&obj.max), sizeof(obj.max));
	in.read(reinterpret_cast<char*>(&obj.escalation_status), sizeof(obj.escalation_status));
	return in;
}



std::istream& operator >> (std::istream& in, FItem& obj) {
	in.read(reinterpret_cast<char*>(&obj.iterator), sizeof(obj.iterator));
	in.read(reinterpret_cast<char*>(&obj.index), sizeof(obj.index));
	return in;
}

std::istream& operator >> (std::istream& in, UpgTreeDataElement& obj) {
	in >> obj.data;
	in.read(reinterpret_cast<char*>(&obj.upgrade_id), sizeof(obj.upgrade_id));
	in.read(reinterpret_cast<char*>(&obj.used), sizeof(obj.used));
	return in;
}

std::istream& operator >> (std::istream& in, UpgTreeData& obj) {
	in >> obj.current;
	in >> obj.nested;
	return in;
}

std::istream& operator >> (std::istream& in, ReqOpcode& obj) {
	in.read(reinterpret_cast<char*>(&obj.core_root), sizeof(obj.core_root));
	in.read(reinterpret_cast<char*>(&obj.current_comparison_index), sizeof(obj.current_comparison_index));
	in.read(reinterpret_cast<char*>(&obj.current_level_index), sizeof(obj.current_level_index));
	in.read(reinterpret_cast<char*>(&obj.current_value_index), sizeof(obj.current_value_index));
	in.read(reinterpret_cast<char*>(&obj.id), sizeof(obj.id));
	in >> obj.item;
	return in;
}
std::istream& operator >> (std::istream& in, ReqItem& obj) {
	in.read(reinterpret_cast<char*>(&obj.comparison), sizeof(obj.comparison));
	in.read(reinterpret_cast<char*>(&obj.id), sizeof(obj.id));
	in.read(reinterpret_cast<char*>(&obj.setter_type), sizeof(obj.setter_type));
	in.read(reinterpret_cast<char*>(&obj.value), sizeof(obj.value));
	return in;
}


