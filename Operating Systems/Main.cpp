#include <cstdlib>
#include "UserInput.h"

int main(int argc, char *argv[]){

	UserInput inputHandler(argc, argv);

	inputHandler.printData();
	return 0;
}
/*
void charToInt(){
	char character = '8';

	int integer = character - '0';		// works
	int integer2 = atoi(&character);	// works
	int integer3 = toDigit(character);	// works

	std::cout << integer  << std::endl;
	std::cout << integer2 << std::endl;
	std::cout << integer3 << std::endl;
}
*/