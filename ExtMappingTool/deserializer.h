#pragma once

#include <QtWidgets/QMainWindow>
#include "TriggerSystem.h"
#include "upgreqmenu.h"
#include "chk.h"
/*
std::istream& operator>>(std::istream& in, int& obj);
std::istream& operator>>(std::istream& in, EditorInfo& obj);
std::istream& operator>>(std::istream& in, FileInfo& obj);
std::istream& operator>>(std::istream& in, Argument& obj);
std::istream& operator>>(std::istream& in, ArgumentConvention& obj);
std::istream& operator>>(std::istream& in, EventConvention& obj);
std::istream& operator>>(std::istream& in, TriggerSetup& obj);
std::istream& operator>>(std::istream& in, ConditionLink& obj);
std::istream& operator>>(std::istream& in, Action& obj);
std::istream& operator>>(std::istream& in, Event& obj);
std::istream& operator>>(std::istream& in, TriggerSystemItem& obj);
*/

std::istream& operator >> (std::istream& in, Ext& obj);
std::istream& operator >> (std::istream& in, FItem& obj);
std::istream& operator >> (std::istream& in, UpgTreeDataElement& obj);
std::istream& operator >> (std::istream& in, UpgTreeData& obj);
std::istream& operator >> (std::istream& in, ReqOpcode& obj);
std::istream& operator >> (std::istream& in, ReqItem& obj);
#include <iostream>
#include <fstream>
#include <ostream>
#include <type_traits>

template <typename T>
struct TypeName
{
	static const char* Get()
	{
		return typeid(T).name();
	}
};

template <>
struct TypeName<u32>
{
	static const char* Get()
	{
		return "u32";
	}
};
template <>
struct TypeName<int>
{
	static const char* Get()
	{
		return "int";
	}
};
template <>
struct TypeName<u16>
{
	static const char* Get()
	{
		return "u16";
	}
};
template <>
struct TypeName<short>
{
	static const char* Get()
	{
		return "short";
	}
};
template <>
struct TypeName<u8>
{
	static const char* Get()
	{
		return "u8";
	}
};
template <>
struct TypeName<Ext>
{
	static const char* Get()
	{
		return "Ext";
	}
};
template <>
struct TypeName<FItem>
{
	static const char* Get()
	{
		return "FItem";
	}
};

template<typename T>
std::istream& operator>>(std::istream& in, std::vector<T> &v) {
	unsigned long long len = 0;
	in.read(reinterpret_cast<char*>(&len), sizeof(len));
	//std::ofstream temp;
	//temp.open("log.log", std::ios::app);
	//temp << "Length: "+WriteInt(len);
	//temp << "\n";
	//temp << "Start vector\n";
	//temp << std::string(TypeName<unsigned long long>::Get()) << "   " << WriteInt(len) << " " << WriteInt(in.tellg()) <<'\n';

	//temp << "Start loop with length " + WriteInt(len) << "\n";
	//temp << "{\n";
	//temp.close();
	for (int i = 0; i < len; i++) {
		T val;		
		//temp.open("log.log", std::ios::app);
		//temp << "I: " << WriteInt(i) << " Before read: " << in.tellg() << '\n';
		//temp.close();
		in >> val;
		//temp.open("log.log", std::ios::app);
		//temp << "N "+WriteInt(i)+" global: "+WriteInt(global_i)+" "+std::string(TypeName<T>::Get());
		//temp << std::string(TypeName<T>::Get()) << " , " << in.tellg() << '\n';
		//temp << "\n";
		//temp.close();
		v.push_back(val);
	}
	//temp.open("log.log", std::ios::app);
	//temp << "}\n";
	//temp << "End loop\n";
	//temp.close();
	
	return in;
}
