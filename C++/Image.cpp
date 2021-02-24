#include "Image.h"

Image::Image(ifstream& input1, ifstream& input2){

	if (input1.good()) {
		input1 >> this->imageRows;
		input1 >> this->imageCols;
		input1 >> this->imageMin;
		input1 >> this->imageMax;
	}

	if (input2.good()) {
		input2 >> this->structRows;
		input2 >> this->structCols;
		input2 >> this->structMin;
		input2 >> this->structMax;
		input2 >> this->originRow;
		input2 >> this->originCol;
	}

	this->rowFrameSize = (this->structRows / 2);
	this->colFrameSize = (this->structCols / 2);
	this->extraRows = (this->rowFrameSize * 2);
	this->extraCols = (this->colFrameSize * 2);

	initializeArrays();
}

Image::~Image(){
	for (int i = 0; i < (this->imageRows + this->extraRows); i++) {
		delete[] zeroFramedArray[i];
		delete[] morphologyArray[i];
		if (i < this->structRows) {
			delete[] structuringElement[i];
		}
	}
	delete[] zeroFramedArray;
	delete[] morphologyArray;
	delete[] structuringElement;
}

void Image::initializeArrays() {

	zeroFramedArray = new int*[this->imageRows + this->extraRows];
	morphologyArray = new int*[this->imageRows + this->extraRows];
	structuringElement = new int*[this->structRows];

	for (int i = 0; i < (this->imageRows + this->extraRows); i++) {

		zeroFramedArray[i] = new int[this->imageCols + this->extraCols];
		morphologyArray[i] = new int[this->imageCols + this->extraCols];

		if (i < this->structRows) {
			structuringElement[i] = new int[this->structCols];
		}

		for (int j = 0; j < (this->imageCols + this->extraCols); j++) {
			zeroFramedArray[i][j] = 0;
			morphologyArray[i][j] = 0;
			if (i < this->structRows && j < this->structCols) {
				structuringElement[i][j] = 0;
			}
		}
	}
}

void Image::loadImage(ifstream& input, int** zeroFramedArray){

	while (!input.eof()) {
		for (int i = originRow; i < (this->imageRows + this->rowFrameSize); i++) {
			for (int j = originCol; j < (this->imageCols + this->colFrameSize); j++) {
				input >> zeroFramedArray[i][j];
			}
		}
	}
}

void Image::loadStructuringElement(ifstream& input, int** structuringElement) {

	while (!input.eof()) {
		for (int i = 0; i < this->structRows; i++) {
			for (int j = 0; j < this->structCols; j++) {
				input >> structuringElement[i][j];
			}
		}
	}
}

void Image::reset2DArray(int arrayType, int** array){
	if (arrayType == 1) {
		for (int i = 0; i < (this->imageRows + this->extraRows); i++) {
			for (int j = 0; j < (this->imageCols + this->extraCols); j++) {
				array[i][j] = 0;
			}
		}
	}

	else if (arrayType == 2) {
		for (int i = 0; i < this->structRows; i++) {
			for (int j = 0; j < this->structCols; j++) {
				array[i][j] = 0;
			}
		}
	}
}

void Image::computeDilation(int** zeroFramedArray, int** morphologyArray){
	for (int i = this->rowFrameSize; i < (this->imageRows + this->rowFrameSize); i++) {
		for (int j = this->colFrameSize; j < (this->imageCols + this->colFrameSize); j++) {
			if (zeroFramedArray[i][j] > 0) {
				dilation(i, j);
			}
		}
	}
}

void Image::computeErosion(int** zeroFramedArray, int** morphologyArray) {
	for (int i = this->rowFrameSize; i < (this->imageRows + this->rowFrameSize); i++) {
		for (int j = this->colFrameSize; j < (this->imageCols + this->colFrameSize); j++) {
			if (zeroFramedArray[i][j] > 0) {
				erosion(i, j);
			}
		}
	}
	
	for (int i = 0; i < (this->imageRows + this->extraRows); i++) {
		delete[] tempArray[i]; 
	}
	delete[] tempArray;
}

void Image::computeOpening(int** zeroFramedArray, int** morphologyArray) {

	int** tempArray = new int*[this->imageRows + this->extraRows];

	for (int i = 0; i < (this->imageRows + this->extraRows); i++) {
		tempArray[i] = new int[this->imageCols + this->extraCols];
		for (int j = 0; j < (this->imageCols + this->extraCols); j++) {
			tempArray[i][j] = 0;
		}
	}
	opening(zeroFramedArray, morphologyArray, tempArray);
	
	for (int i = 0; i < (this->imageRows + this->extraRows); i++) {
		delete[] tempArray[i]; 
	}
	delete[] tempArray;
}

void Image::computeClosing(int** zeroFramedArray, int** morphologyArray) {

	int** tempArray = new int* [this->imageRows + this->extraRows];

	for (int i = 0; i < (this->imageRows + this->extraRows); i++) {
		tempArray[i] = new int[this->imageCols + this->extraCols];
		for (int j = 0; j < (this->imageCols + this->extraCols); j++) {
			tempArray[i][j] = 0;
		}
	}
	closing(zeroFramedArray, morphologyArray, tempArray);

}

void Image::dilation(int i, int j){
	int rowOffset, colOffset;
	for (int row = 0; row < structRows; row++) {
		rowOffset = i - originRow;
		for (int col = 0; col < structCols; col++) {
			colOffset = j - originCol;
			if (structuringElement[row][col] == 1) {
				morphologyArray[rowOffset + row][colOffset + col] = structuringElement[row][col];
			}
		}
	}
}

void Image::erosion(int i, int j) {
	bool match = true;
	int rowOffset, colOffset;

	for (int row = 0; row < structRows; row++) {
		rowOffset = i - originRow;
		for (int col = 0; col < structCols; col++) {
			colOffset = j - originCol;
			if (structuringElement[row][col] == 1 && zeroFramedArray[rowOffset + row][colOffset + col] != 1) {
				match = false;
				break;
			}
		}
		if (!match) {
			break;
		}
	}

	if (match) {
		morphologyArray[i][j] = 1;
	}
	else {
		morphologyArray[i][j] = 0;
	}
}

void Image::opening(int** zeroFramedArray, int** morphologyArray, int** array) {
	reset2DArray(1, array);
	computeErosion(zeroFramedArray, array);
	computeDilation(array, morphologyArray);
}

void Image::closing(int** zeroFramedArray, int** morphologyArray, int** array) {
	reset2DArray(1, array);
	computeDilation(zeroFramedArray, array);
	computeErosion(array, morphologyArray);
}

void Image::printArrayResult(int** array, ofstream& output) {
	try {

		output << imageRows << " " << imageCols << " " << imageMin << " " << imageMax << endl;

		for (int i = 0; i < (this->imageRows + this->extraRows); i++) {
			for (int j = 0; j < (this->imageCols + this->extraCols); j++) {
				output << array[i][j] <<  " ";
			}
			output << endl;
		}
		output << endl << endl;

	}
	catch (exception e) {
		cout << "There is an error: " <<  e.what() << endl;
	}
}

void Image::prettyPrint(int** array, int arrayType, ofstream& output) {

	if (arrayType == 1) {
		try {
			for (int i = this->rowFrameSize; i < (this->imageRows + this->extraRows); i++) {
				for (int j = this->colFrameSize; j < (this->imageCols + this->extraCols); j++) {
					if (array[i][j] == 0) {
						output << ". ";
					}
					else {
						output << array[i][j] << " ";
					}
				}
				output << endl;
			}
			output << endl << endl;
		}
		catch (exception e) {
			cout << "There is an error: " << e.what() << endl;
		}
	}

	else if (arrayType == 2) {
		try {
			for (int i = 0; i < this->structRows; i++) {
				for (int j = 0; j < this->structCols; j++) {
					if (array[i][j] == 0) {
						output << ". ";
					}
					else {
						output << array[i][j] << " ";
					}
				}
				output << endl;
			}
			output << endl << endl;
		}
		catch (exception e) {
			cout << "There is an error: " << e.what() << endl;
		}
	}

}
