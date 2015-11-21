#include "file.h"
#include <sstream>
#include <iterator>


std::string File::translate( std::string input ) {
	std::istringstream buf( input );
	std::istream_iterator<std::string> beg( buf ), end;

	std::vector<std::string> words(beg,end);
	std::string toreturn;

	for( unsigned int i = 0; i < words.size(); i++){
		std::string spliter;
		if( words.at( i ).size() >= 5 ) {
			spliter = std::string( words.at( i ).at( 0 ), words.at( i ).at( 4 ) );

			if( spliter == "print" ) {
				toreturn += "std::cout";
			}
		}
	}

	return std::string();
}

File::File( std::string input_path ) {
	bool goodExt;
	{
		//valid extencion = "gk"
		std::string extencion = std::string( input_path.at( input_path.size() - 4 ), (input_path.size() - 1) );
		if( extencion == ".gk" )
			goodExt = true;
		else
			goodExt = false;
	}
	if( goodExt ) {
		inputFile.open( input_path, std::ios::in );
		if( !inputFile.good() ) {
			std::cout << "wrong file path" << std::endl;
		}
		std::string newPath = std::string(input_path.at(0), input_path.at(input_path.size()-5));
		outputFile.open(newPath+".txt", std::ios::app);
	}
	else {
		std::cout << "invalid file extencion" << std::endl;
	}
}

void File::compile() {
	std::vector<std::string> instrTab;
	std::vector<std::string> outputInstr;
	std::stack<std::string> instrStack;

	std::string temp;
	while( inputFile.eof() ) {
		std::getline( inputFile, temp );
		instrTab.push_back( temp );
		temp.clear();
	}

	///----------proces interpretacji---------///

	for( unsigned int i = 0; i < instrTab.size(); i++ ) {

	}
}

File::~File() {
}
