#include "Data.h"
Queue::Queue(){
	writeEvent = CreateEvent(0, 0, 0, 0);
	readEvent = CreateEvent(0, 0, 0, 0);

	InitializeCriticalSection(&readCriticalSection);
	InitializeCriticalSection(&writeCriticalSection);
}

Block Queue::get()
{
	EnterCriticalSection(&readCriticalSection);
	while (buffer.empty()){
		WaitForSingleObject(writeEvent, INFINITE);
	}
	Block block = buffer.front();
	buffer.pop();
	//cout << "Block taken from queue" << endl;
	//cout << "size of queue is: " << buffer.size() << endl;
	SetEvent(readEvent);
	LeaveCriticalSection(&readCriticalSection);
	return block;

}

void Queue::put(Block block)
{
	EnterCriticalSection(&writeCriticalSection);
	/*while (count == BUFLEN){
		WaitForSingleObject(readEvent, INFINITE);
	}*/
	//cout << producername << ": puts " << block << endl;
	buffer.push(block);
	//cout << "Block added to queue" << endl;
	//cout << "size of queue is: " << buffer.size() << endl;
	SetEvent(writeEvent);
	LeaveCriticalSection(&writeCriticalSection);
}

int Queue::getQueueSize(){
	return buffer.size();
}

Data::Data(int bass, int treble){
	this->bassIntensity = bass;
	this->trebleIntensity = treble;
}

void Data::readFile(string filename){
	Block tmp;
	signed short a;
	tmp = Block();
	tmp.setOrderNumber(this->blockCount);
	fstream mystream;
	mystream.open(filename, fstream::binary | fstream::in | fstream::out);

	if (!mystream){
		cout << "file niet aanwezig! " << endl;
	}
	else{
		//read something
		cout << "Opened file: " << filename << endl;
		cout << "Start reading file and filling blocks" << endl;

		while(mystream.read((char *)&a, sizeof(signed short))){
			
			//cout << "Samplecount: " << this->tmp.getSampleCount() << endl;
			if (tmp.getSampleCount() < 1024){
				tmp.addSample(a);
				//cout << "Added Sample, total sampleCount: " << this->tmp.getSampleCount() << endl;
			}
			else{
				this->prebuffer.put(tmp); //ADD BLOCK TO QUEUE PREBUFFER
				//cout << "Block: " << blockCount << " added to queue" << endl;
				blockCount++;
				tmp = Block();
				tmp.setOrderNumber(this->blockCount);
				tmp.addSample(a);
			}
		}
		//ADD LAST BLOCK WITH POSSIBILITY OF < 1024 SAMPLES TO QUEUE
		prebuffer.put(tmp);
		this->blockCount++;
	}
	cout << "Finished reading file, " << this->blockCount << " filled." << endl;
}

void Data::initThread(int threadAmount, string filename){
	cout << "Initializing Threads. " << threadAmount << " Threads will be using for equalizing" << endl;
	thread fileRead = thread(&Data::readFile, this, filename);
	int blocks = 0;
	int zulu = 1;
	thread *myThreads = new thread[threadAmount];
	
	while (blocks < 2676){
		for (int x = 0; x < threadAmount; x++){
			int zulu = 1;
			myThreads[x] = thread(&Data::equalize, this, zulu);
		}
		for (int x = 0; x < threadAmount; x++){
			if (blocks < 2676){
				myThreads[x].join();
			}
			else{
				myThreads[x].detach();
			}
			blocks++;
		}
	}
	fileRead.join();
	cout << "Size Postbuffer:  " << postbuffer.getQueueSize() << endl; //2678
}

void Data::trebleCoefficients(int intensity, double *b0, double *b1, double *b2, double *a1, double *a2)
{
	double frequency = 3300;
	double qFactor = 0.5;
	double gain = intensity;
	double sampleRate = 44100;
	double pi = 4.0*atan(1);
	double a = pow(10.0, gain / 40);
	double w0 = 2 * pi*frequency / sampleRate;
	double alpha = sin(w0) / (2.0*qFactor);

	double a0 = (a + 1) - (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha;

	*a1 = -(2.0*((a - 1) - (a + 1)*cos(w0))) / a0;
	*a2 = -((a + 1) - (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha) / a0;
	*b0 = (a*((a + 1) + (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha)) / a0;
	*b1 = (-2.0*a*((a - 1) + (a + 1)*cos(w0))) / a0;
	*b2 = (a*((a + 1) + (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha)) / a0;
}

void Data::bassCoefficients(int intensity, double *b0, double *b1, double *b2, double *a1, double *a2)
{
	double frequency = 330;
	double qFactor = 0.5;
	double gain = intensity;
	double sampleRate = 44100;
	double pi = 4.0*atan(1);

	double a = pow(10.0, gain / 40);
	double w0 = 2 * pi*frequency / sampleRate;
	double alpha = sin(w0) / (2.0*qFactor);
	double a0 = (a + 1) + (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha;

	*a1 = -(-2.0*((a - 1) + (a + 1)*cos(w0))) / a0;
	*a2 = -((a + 1) + (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha) / a0;
	*b0 = (a*((a + 1) - (a - 1)*cos(w0) + 2.0*sqrt(a)*alpha)) / a0;
	*b1 = (2 * a*((a - 1) - (a + 1)*cos(w0))) / a0;
	*b2 = (a*((a + 1) - (a - 1)*cos(w0) - 2.0*sqrt(a)*alpha)) / a0;
}

void Data::equalize(int zulu){
	Block input;
	Block temp;
	Block output;
	signed short x = 1, x1 = 1, x2 = 1, y = 1, y1 =1, y2 = 1;
	int count = 0;

	input = prebuffer.get();
	output.setOrderNumber(input.getOrderNumber());

	bassCoefficients(this->bassIntensity, &b0, &b1, &b2, &a1, &a2);
		while (count < input.getSampleCount()){
				x = input.getSample(count);

				y = (b0 * x) + (b1 * x1) + (b2 * x2) + (a1 * y1) + (a2 * y2);
				temp.addSample(y);

				x2 = x1;
				x1 = x;
				y2 = y1;
				y1 = y;

			count++;
		}

		count = 0;

	trebleCoefficients(this->trebleIntensity, &b0, &b1, &b2, &a1, &a2);
		while (count < temp.getSampleCount()){
				x = temp.getSample(count);

				y = (b0 * x) + (b1 * x1) + (b2 * x2) + (a1 * y1) + (a2 * y2);
				output.addSample(y);

				x2 = x1;
				x1 = x;
				y2 = y1;
				y1 = y;

			count++;
		}

	postbuffer.put(output);
}

void Data::writeFile(string filename){
	int size = postbuffer.getQueueSize();
	Block *blockArray = new Block[size];

	for (int element = 0; element < size; element++){ //FILING FINAL ARRAY / SORTING
		Block tmp = postbuffer.get();
		int pos = tmp.getOrderNumber();
		blockArray[pos] = tmp;
	}

	int error = 0;
	for (int x = 0; x < size; x++){ //PLACING CHECK
		if (x != blockArray[x].getOrderNumber()){
			cout << "False! position " << x << " with blockNumber: " << blockArray[x].getOrderNumber() << endl;
		}
	}
	if (error != 0){
		cout << error << " errors in placing";
	}
	else{
		cout << "No errors in placing order" << endl;
		cout << "Writing File: " << filename << endl;

		//FILLING FILE
		ofstream outfile(filename, ofstream::binary);
		for (int outer = 0; outer < size; outer++){
			for (int inner = 0; inner < blockArray[outer].getSampleCount(); inner++){
				signed short sample = blockArray[outer].getSample(inner);
				outfile.write((char *)&sample, sizeof(signed short));
			}
		}
		cout << "Finished writing File: " << filename << endl;
 	}


}


