#include "file.h"
#include <sstream>
#include <iterator>
#include <locale>

bool is_number( const std::string& s ) {
	std::locale loc;
	std::string::const_iterator it = s.begin();
	while( it != s.end() && std::isdigit( *it,loc ) ) ++it;
	return !s.empty() && it == s.end();
}

template<class T>
inline File::data<T>& File::find( std::vector<data<T>>& tab, std::string name ) {
	for( unsigned int i = 0; i < tab.size(); i++ ) {
		if( tab.at( i ).name == name ) {
			return tab.at( i );
		}
	}
	return File::data<T>();
}

std::string File::translate( std::string input ) {
	std::istringstream buf( input );
	std::istream_iterator<std::string> beg( buf ), end;

	std::vector<std::string> words(beg,end);
	std::string toreturn;

	
	bool USING_MACRO = false;
	bool MATH_EXPR = false;
	bool NEW_VAR = false;
	bool PRINT = false;
	bool USING_MACRO_START_END = false;
	bool SYNTAX_ERROR = false;
	bool ABORT = false;
	{
		std::string spliter;
		//-----------sprawdzam makra-----------//
		if( words.at( 0 ).at( 0 ) == '/' ) {
			if( words.at( 0 ) == "/START" ) {
				USING_MACRO_START_END = true;
				START_PROGRAMU = true;
			}
			if( START_PROGRAMU ) {
				if( words.at( 0 ) == "/USING" ) {
					USING_MACRO = true;
				}
				else if( words.at( 0 ) == "/END" ) {
					START_PROGRAMU = false;
					USING_MACRO_START_END = true;
				}
			}
		}
		//----------sprawdzam inne komendy----------//
		else if( words.at( 0 ).size() >= 3 ) {
			if( START_PROGRAMU ) {
				spliter = std::string( words.at( 0 ).at( 0 ), words.at( 0 ).at( 2 ) );
				if( spliter == "var" ) {
					NEW_VAR = true;
				}
				if( words.at( 0 ).size() >= 5 ) {
					spliter = std::string( words.at( 0 ).at( 0 ), words.at( 0 ).at( 4 ) );
					if( spliter == "print" ) {
						toreturn += "std::cout <<";
						PRINT = true;
					}
				}
			}
		}
		else {
			MATH_EXPR = true;
		}
	}
	if( START_PROGRAMU ) {
		if( USING_MACRO ) {
			if( words.at( 2 ) == "\"IOSTREAM\"" ) {
				toreturn += "#include <iostream>\n";
			}
		}
		else if( NEW_VAR ) {
			if( words.at( 2 ) == ":" ) {
				if( words.at( 3 ) == "int" ) {
					if( find( intTab, words.at( 1 ) ).name.empty() ) {
						intTab.push_back( data<int>{} );
						intTab.at( intTab.size() - 1 ).name = words.at( 1 );
					}
					else {
						std::cout << "Name already occupated" << std::endl;
					}
				}
				else if( words.at( 3 ) == "float" ) {
					floatTab.push_back( data<float>{} );
					intTab.at( intTab.size() - 1 ).name = words.at( 1 );
				}
				else if( words.at( 3 ) == "string" ) {
					stringTab.push_back( data<std::string>{} );
					intTab.at( intTab.size() - 1 ).name = words.at( 1 );
				}
				else {
					std::cout << "Syntax error: there is no such type as"<< words.at( 3 ) << std::endl;
				}
			}
			else {
				std::cout << "Syntax error: wrong var declaration" << std::endl;
				ABORT = true;
			}
		}
		//---------math----------//
		if( MATH_EXPR ) {
			if( !find( intTab, words.at( 0 ) ).name.empty() ) {
				if( words.at( 1 ) == "=" ) {
					for( unsigned int i = 2; i < words.size(); i++ ) {
						//if( i > 2 ) {
							//if( words.at( 2 ) == "-" || words.at( 2 ) == "+" ) {

							//}
							//else {
							if( i % 2 == 0 ) {
								if( is_number( words.at( i )) ) {
									find( intTab, words.at( 0 ) ).value += std::stoi( words.at( i ) );
								}
								else if( !find( intTab, words.at( i ) ).name.empty() ) {
									find( intTab, words.at( 0 ) ).value += find( intTab, words.at( i ) ).value;
								}
								else {
									std::cout << "var not declarated yet" << std::endl;
									ABORT = true;
								}
							}
							//}
						//}
					}
				}
			}
			else {
				std::cout << "var not declarated yet" << std::endl;
				ABORT = true;
			}
			if( !ABORT ) {
				toreturn = "#math";
			}
		}
		else if( PRINT ) {
			std::string split = std::string( words.at( 0 ).at( 6 ), words.at( 0 ).at( words.size() - 3 ) );
			if( split.at( 0 ) == '"' && split.at( split.size() - 1 ) == '"' ) {
				toreturn += std::string( split.at( 1 ), split.at( split.size() - 2 ) );
			}
			else {
				if( find( intTab, split ).name.empty() ) {
					std::cout << "var undefined" << std::endl;
					ABORT = true;
				}
				else {
					toreturn += find( intTab, split ).value;
				}
			}
		}
	}

	if( !ABORT ) {
		toreturn += ";";
		return toreturn;
	}
	else
		return "error";
}

//for( unsigned int i = 1; i < words.size(); i++ ) {
//	if( USING_MACRO ) {
//		if( words.at( 2 ) == "\"IOSTREAM\"" ) {
//			toreturn += "#include <iostream>\n";
//		}
//	}
//	if( NEW_VAR ) {
//
//	}
//}

File::File( std::string input_path ) {
	bool goodExt;
	{
		//valid extencion = "gk"
		std::string extencion = input_path.substr( input_path.size() - 3 , input_path.size() - 1 );
		std::cout << extencion << std::endl;
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
		std::string newPath = input_path.substr(0, input_path.size()-3);
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
	std::string pomoc;
	for( unsigned int i = 0; i < instrTab.size(); i++ ) {
		pomoc = translate( instrTab.at( i ) );
		std::cout << pomoc << std::endl;
		if( pomoc == "error" ) {
			std::cout << "compilation aborded" << std::endl;
			break;
		}
		else {
			outputInstr.push_back( pomoc );
		}
		pomoc.clear();
	}
	///----------daj do nowego pliku---------///
	for( unsigned int i = 0; i < outputInstr.size(); i++ ) {
		outputFile << outputInstr.at( i );
	}
	outputFile.close();
	std::cout << "compilation finished";
}

File::~File() {
}
