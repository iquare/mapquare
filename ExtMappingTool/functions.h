#pragma once
#include <sstream>

template<typename T, typename Func>
inline void iter_enumerated(std::vector<T> &v, Func f) {
	for (int i = 0; i < v.size(); i++) {
		f(v[i], i);
	}
}

//filter/retain function for vector
template<typename T, typename Filter>
inline void retain(std::vector<T> &test, Filter f) {
	for (int i = 0; i < test.size(); i++) {
		if (!f(test[i])) {
			test.erase(test.begin() + i);
			i--;
		}
	}
}

void trim(std::string &line);
void trim(std::vector<std::string> &linevec);
bool starts_with(std::string line, std::string str);
std::vector<std::string> split_by(std::string line, char delimiter);