#include "View.h"

View::View(std::vector<std::string>& clients) : 
	prompting_to_enter_data{"Enter the computer number to get a screenshot"}
{
	update_computers(clients);
}

void View::update_computers(std::vector<std::string>& clients)
{
	for (int i = 0; i < (int)clients.size(); ++i) {
		computers += std::format("{} {}\n", i, clients[i]);
	}
	show();
}

void View::update_screenshot_path(std::string screenshot_path)
{
	screenshot_path_ = screenshot_path + "\n";
	show();
}

void View::show()
{
	int lines = 0;
	for (auto c : page) {
		if (c == '\n') {
			++lines;
		}
	}

	page = std::format("{}\n{}\n{}\n", computers, 
		screenshot_path_, prompting_to_enter_data);

	std::cout << '\r' << std::string(lines, '\n') << page << std::endl;
}