#pragma once

#include <string>

class CHAT
{
public:
	CHAT();
	~CHAT();
	std::string read(const std::string& firstUsername, const std::string& secondUsername);
	void write(const std::string& firstUsername, const std::string& secondUsername, const std::string& data);

private:
	static std::string get_chat_file_path(const std::string& firstUsername, const std::string& secondUsername);
};
