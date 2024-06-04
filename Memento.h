// Memento.h
#ifndef MEMENTO_H
#define MEMENTO_H

#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

class Memento
{
public:
	void								save(const std::string& username, const std::string& password);
	std::pair<std::string, std::string> restore();
};

#endif // MEMENTO_H
