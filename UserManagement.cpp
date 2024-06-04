// UserManagement.cpp
#include "UserManagement.h"
#include <fstream>
#include <nlohmann/json.hpp>
#include <QDir>
#include <QFile>

#include "logger.h"

#define USERS_FILE "../data/users.json"

using json = nlohmann::json;

UserManagement::UserManagement()
{
	loadUsers();
}

void UserManagement::loadUsers()
{
	// std::ifstream inFile("users.json");
	QString usersFilePath = QDir(QCoreApplication::applicationDirPath()).filePath(USERS_FILE);
	QFile usersFile(usersFilePath);

	std::ifstream inFile(usersFile.fileName().toStdString());
	if (inFile)
	{
		json j;
		inFile >> j;
		for (const auto &item : j)
		{
			users[item["username"]] = {item["password"], item["role"]};
		}
	}
}

void UserManagement::saveUsers()
{
	// std::ofstream outFile("users.json");
	QString usersFilePath = QDir(QCoreApplication::applicationDirPath()).filePath(USERS_FILE);
	QFile usersFile(usersFilePath);

	json j = json::array();
	for (const auto &user : users)
	{
		j.push_back(
			{{"username", user.first}, {"password", user.second.password}, {"role", user.second.role}});
	}
	std::ofstream outFile(usersFile.fileName().toStdString());
	outFile << j.dump(4);
}

bool UserManagement::authenticate(const std::string &username, const std::string &password)
{
	loadUsers();

	auto it = users.find(username);
	if (it != users.end())
	{
		if (it->second.password == password)
		{
			qInfo() << "Authentication successful";
			return true;
		}
	}

	qInfo() << "Authentication failed";
	return false;
}

bool UserManagement::isAdmin(const std::string &username)
{
	auto it = users.find(username);
	if (it != users.end())
	{
		return it->second.role == "admin";
	}
	return false;
}

void UserManagement::addUser(const std::string &username, const std::string &password,
							 const std::string &role)
{
	users[username] = {password, role};
	saveUsers();
}

void UserManagement::removeUser(const std::string &username)
{
	users.erase(username);
	saveUsers();
}
