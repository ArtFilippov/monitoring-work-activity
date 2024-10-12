#include "Server.h"

int main() try
{
	Server s{};

	return 0;
}
catch (std::string e) 
{
	std::cerr << e << std::endl;

	return 0;
}