#include "Block.h"

Block::Block(){
	this->sampleCount = 0;
	this->edited = false;
}

int Block::getSampleCount(){
	return this->sampleCount;
}

bool Block::getEditStatus(){
	return this->edited;
}

int Block::getOrderNumber(){
	return this->order;
}

void Block::setOrderNumber(int blockNumber){
	this->order = blockNumber;
}


int Block::addSample(signed short sample){
	if (this->sampleCount < 1024){
		this->samples[this->sampleCount] = sample;
		this->sampleCount++;
		//cout << "Added sample" << endl;
		return 1;
	}
	else{
		//cout << "Block full!" << endl;
		return 0;
	}
}

signed short Block::getSample(int sampleNumber){
	if (this->sampleCount > 0){
		signed short tmp = this->samples[sampleNumber];
		//signed short tmp = this->samples[this->sampleCount];
		//this->samples[this->sampleCount] = NULL;
		//this->sampleCount--;
		//cout << "returning sample: " << tmp << endl;
		return tmp;
	}
	else{
		//cout << "Block empty" << endl;
		return -1;
	}
}