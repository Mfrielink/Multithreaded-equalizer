#pragma once
#include "Block.h"
#include <String>
#include <iostream> //std::cout
#include <thread>	//std:thread
#include <fstream>  //std:fstream
#include <stdlib.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <time.h>
#include <queue>

class Queue
{
private:
	CRITICAL_SECTION readCriticalSection;
	CRITICAL_SECTION writeCriticalSection;
	HANDLE writeEvent;
	HANDLE readEvent;
	queue<Block> buffer;
public:
	Queue();
	Block get();
	void put(Block block);
	int getQueueSize();
};

class Data{
private:
	int blockCount;
	Queue prebuffer;
	Queue postbuffer;
	double b0, b1, b2, a1, a2;
	int bassIntensity, trebleIntensity;

public:
	Data(int bass, int treble);
	void initThread(int threadAmount, string filename);
	void readFile(string filename);

	void equalize(int zulu);
	void bassCoefficients(int intensity, double *b0, double *b1, double *b2, double *a1, double *a2);
	void trebleCoefficients(int intensity, double *b0, double *b1, double *b2, double *a1, double *a2);

	void writeFile(string filename);
};
