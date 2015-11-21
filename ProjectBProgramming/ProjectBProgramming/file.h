#pragma once
#include <stack>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using std::fstream;

class File
{
	fstream inputFile;
	fstream outputFile;
	std::string translate(std::string input);
public:
	File(std::string input_path);
	void compile();
	virtual ~File();
};

