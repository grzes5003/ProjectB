#pragma once
#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using std::fstream;

class File
{
	bool START_PROGRAMU = false;

	template<class T>
	struct data{
		std::string name;
		T value;
	};

	std::vector<data<int>> intTab;
	std::vector<data<double>> doubleTab;
	std::vector<data<float>> floatTab;
	std::vector<data<std::string>> stringTab;

	template<class T>
	File::data<T>& find(std::vector<data<T>> &tab, std::string name);

	fstream inputFile;
	fstream outputFile;
	std::string translate(std::string input);
public:
	File(std::string input_path);
	void compile();
	virtual ~File();
};



