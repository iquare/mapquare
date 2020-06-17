#pragma once
#include "TriggerSystem.h"
#include "upgreqmenu.h"

std::ostream& operator<<(std::ostream& out, const int& obj);
std::ostream& operator<<(std::ostream& out, const EditorInfo& obj);
std::ostream& operator<<(std::ostream& out, const FileInfo& obj);
std::ostream& operator<<(std::ostream& out, const Argument& obj);
std::ostream& operator<<(std::ostream& out, const ArgumentConvention& obj);
std::ostream& operator<<(std::ostream& out, const EventConvention& obj);
std::ostream& operator<<(std::ostream& out, const TriggerSetup& obj);
std::ostream& operator<<(std::ostream& out, const ConditionLink& obj);
std::ostream& operator<<(std::ostream& out, const Action& obj);
std::ostream& operator<<(std::ostream& out, const Event& obj);
std::ostream& operator<<(std::ostream& out, const TriggerSystemItem& obj);
std::ostream& operator<<(std::ostream& out, const Ext& obj);
std::ostream& operator<<(std::ostream& out, const FItem& obj);
std::ostream& operator<<(std::ostream& out, const UpgTreeDataElement& obj);
std::ostream& operator<<(std::ostream& out, const UpgTreeData& obj);
std::ostream& operator<<(std::ostream& out, const ReqOpcode& obj);
std::ostream& operator<<(std::ostream& out, const ReqItem& obj);

template<typename T>
std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
	unsigned long long len = v.size();
	out.write(reinterpret_cast<const char*>(&len), sizeof(len));
	for (auto a : v) {
		out << a;
	}
	return out;
}