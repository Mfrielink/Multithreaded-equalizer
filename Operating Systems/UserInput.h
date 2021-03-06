#ifndef USERINPUT_H
#define USERINPUT_H

#include <string>
#include <iostream>

#define toDigit(c) (c-'0')

class UserInput{

	std::string inputFile, outputFile;
	int threads, bass, treble;

public:
	UserInput(int argc, char *argv[]);

	void showInput(int argc, char *argv[]);
	void validateInput(int argc, char *argv[]);

	void showHelpInfo();
	void printData();

	int getNumberOfThreads();
	int getValueOfBass();
	int getValueOfTreble();
	std::string getInputFile();
	std::string getOutputFile();
};

#endif