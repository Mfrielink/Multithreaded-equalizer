#pragma once
#include "Data.h"
#include "UserInput.h"

#include <String>
#include <iostream> //std::cout
#include <thread>	//std:thread
#include <fstream>  //std:fstream
using namespace std;

int main(int argc, char *argv[]){

	/* //TESTING PURPOSE
	string outputfile = "output.pcm";
	string inputfile = "you_and_i.pcm";
	int threadsAmount = 8;
	int bassIntensity = 2;
	int trebleIntensity = 2;*/

	UserInput inputHandler(argc, argv);

	string inputfile = inputHandler.getInputFile();
	
	string outputfile = inputHandler.getOutputFile();
	int threadsAmount = inputHandler.getNumberOfThreads();
	int bassIntensity = inputHandler.getValueOfBass();
	int trebleIntensity = inputHandler.getValueOfTreble();

	Data data1(bassIntensity, trebleIntensity);

	data1.initThread(threadsAmount, inputfile);
	cout << "Finished with Equalizing" << endl;
	data1.writeFile(outputfile);
	cout << "Finished Writing" << endl;

	cout << "End of program" << endl;

	cin.get();

	return 0;
}




