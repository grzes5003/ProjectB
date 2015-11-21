
#include "file.h"


int main() {
	std::string Input_path;
	std::cout << "Write file path" << std::endl;
	std::cin >> Input_path;
	File file(Input_path);
	file.compile();
	char m;
	std::cin >> m;
}