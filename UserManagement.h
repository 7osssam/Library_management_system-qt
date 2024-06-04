// UserManagement.h
#ifndef USERMANAGEMENT_H
#define USERMANAGEMENT_H

#include <string>
#include <unordered_map>

struct UserData
{
	std::string password;
	std::string role;
};

class UserManagement
{
private:
	std::unordered_map<std::string, UserData> users;
	void									  loadUsers();
	void									  saveUsers();

public:
	UserManagement();
	bool authenticate(const std::string& username, const std::string& password);
	bool isAdmin(const std::string& username);
	void addUser(const std::string& username, const std::string& password, const std::string& role);
	void removeUser(const std::string& username);
};

#endif // USERMANAGEMENT_H
