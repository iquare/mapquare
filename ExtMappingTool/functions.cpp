#include "stdafx.h"
#include "functions.h"
#include <cctype>
#include <locale>

void trim(std::string &line) {
	line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](int ch) {
		return !std::isspace(ch);
	}));
	line.erase(std::find_if(line.rbegin(), line.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), line.end());
}

void trim(std::vector<std::string> &linevec) {
	for (auto &a : linevec) {
		trim(a);
	}
}

bool starts_with(std::string line, std::string str) {
	return line.rfind(str, 0)== 0;
}

std::vector<std::string> split_by(std::string line, char delimiter) {
	std::string tmp;
	std::stringstream ss(line);
	std::vector<std::string> result;
	while (std::getline(ss, tmp, delimiter)) {
		result.push_back(tmp);
	}
	return result;
}

