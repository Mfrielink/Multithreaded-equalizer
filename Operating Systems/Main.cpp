#pragma once
#include "Data.h"

#include <String>
#include <iostream> //std::cout
#include <thread>	//std:thread
#include <fstream>  //std:fstream
using namespace std;


int main(){


	string inputfile = "you_and_i.pcm";
	string outputfile = "output.pcm";
	int threadsAmount = 8;
	int bassIntensity = 2;
	int trebleIntensity = 2;

	Data data1(bassIntensity, trebleIntensity);

	data1.initThread(threadsAmount, inputfile);
	cout << "Finished with Equalizing" << endl;
	data1.writeFile(outputfile);
	cout << "Finished Writing" << endl;

	cin.get();

	return 0;
}




