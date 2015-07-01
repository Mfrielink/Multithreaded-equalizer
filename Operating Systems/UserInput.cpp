
#include "UserInput.h"

void UserInput::showInput(int argc, char *argv[]){
	std::cout << "Command-line arguments: " << std::endl;
	for (int c = 0; c < argc; c++){
		std::cout << "argv[" << c << "] " << argv[c] << std::endl;
	}
}

void UserInput::validateInput(int argc, char *argv[]){

	if (argc != 6){
		std::cout << "Error: wrong amount of arguments!" << std::endl; 
		showHelpInfo(); 
		exit(1);
	}

	for (int c = 1; c < argc; c++){
		switch (c){
		case 1:
			// Number of threads(2...8)
			if (toDigit(*argv[c]) >= 2 && toDigit(*argv[c]) <= 8){
				this->threads = toDigit(*argv[c]);
			}
			else{
				std::cout << "Error: Number of threads is not correct!" << std::endl; 
				showHelpInfo(); 
				exit(1); 
			}

		case 2:
			// value of bass(-6...6)
			if (toDigit(*argv[c]) >= -6 && toDigit(*argv[c]) <= 6){
				this->bass = toDigit(*argv[c]);
			}
			else{
				std::cout << "Error: Value of bass is not correct!" << std::endl; 
				showHelpInfo(); 
				exit(1);
			}

		case 3:
			// value of treble(-6...6)
			if (toDigit(*argv[c]) && toDigit(*argv[c])){
				this->treble = toDigit(*argv[c]);
			}
			else{
				std::cout << "Error: Value of treble is not correct!" << std::endl;
				showHelpInfo();
				exit(1);
			}

		case 4: 
			// InputFile (.pcm)
			if (argv[c] != NULL)this->inputFile = argv[c];
			
		case 5:
			// OuputFile (.pcm)
			if (argv[c] != NULL)this->outputFile = argv[c];


		}
	}
}

void UserInput::showHelpInfo(){
	std::cout << "	" << "-p  number of threads	( 2...8)"	<< std::endl;
	std::cout << "	" << "-b  bass intensity	(-6...6)"   << std::endl;
	std::cout << "	" << "-t  treble intensity	(-6...6)"	<< std::endl;
	std::cout << "	" << "    input file"		<< std::endl;
	std::cout << "	" << "    output file"		<< std::endl;
}

void UserInput::printData(){
	std::cout << "Number of threads:	"	<< this->threads	<< std::endl;
	std::cout << "Value of bass:		"		<< this->bass		<< std::endl;
	std::cout << "Value of treble:	"		<< this->treble		<< std::endl;
	std::cout << "InputFile:		"			<< this->inputFile	<< std::endl;
	std::cout << "OutputFile:		"			<< this->outputFile	<< std::endl;

}