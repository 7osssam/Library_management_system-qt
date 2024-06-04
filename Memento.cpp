// Memento.cpp
#include "Memento.h"
using json = nlohmann::json;

void Memento::save(const std::string& username, const std::string& password)
{
	json		  j = {{"username", username}, {"password", password}};
	std::ofstream outFile("memento.json");
	outFile << j.dump(4);
}

std::pair<std::string, std::string> Memento::restore()
{
	std::ifstream inFile("memento.json");
	if (inFile)
	{
		json j;
		inFile >> j;
		return {j["username"], j["password"]};
	}
	return {"", ""};
}
