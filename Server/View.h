#pragma once
#include <iostream>
#include <format>
#include <string>
#include <vector>

class View
{
	std::string computers;
	std::string screenshot_path_{};
	std::string prompting_to_enter_data;
	std::string page{};
public:
	View(std::vector<std::string>& clients);

	void update_computers(std::vector<std::string>& clients);
	void update_screenshot_path(std::string screenshot_path);
private:
	void show();
};

