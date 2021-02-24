#pragma once
#include <iostream>
#include <fstream>
#include <string>

using namespace std;


class Image{

private:
	int imageRows;
	int imageCols;
	int imageMin;
	int imageMax;

	int structRows;
	int structCols;
	int structMin;
	int structMax;
	int originRow;
	int originCol;

	int rowFrameSize;
	int colFrameSize;

	int extraRows;
	int extraCols;

public:

	int** zeroFramedArray;
	int** morphologyArray;
	int** structuringElement;

public:

	Image(ifstream& input1, ifstream& input2);
	
	~Image();
	
	void initializeArrays();

	void loadImage(ifstream&, int**);

	void loadStructuringElement(ifstream&, int**);

	void reset2DArray(int, int**);

	void computeDilation(int**, int**);

	void computeErosion(int**, int**);

	void computeOpening(int**, int**);

	void computeClosing(int**, int**);

	void dilation(int, int);

	void erosion(int, int);

	void opening(int**, int**, int**);

	void closing(int**, int**, int**);

	void printArrayResult(int**, ofstream&);

	void prettyPrint(int**, int, ofstream&);

};
