#pragma once
#include <String>
#include <iostream> //std::cout
#include <thread>	//std:thread
#include <fstream>  //std:fstream

using namespace std;

class Block{
private:
	int order;
	signed short samples[1024];
	int sampleCount;
	bool edited;

public:
	Block();
	
	int getSampleCount();
	bool getEditStatus();
	void setOrderNumber(int blockNumber);
	int getOrderNumber();
	int addSample(signed short sample);
	signed short getSample(int sampleNumber);
};