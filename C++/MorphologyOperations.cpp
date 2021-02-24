#include <iostream>
#include <fstream>
#include <string>

#include "Image.h"

using namespace std;

int main(int argc, char** argv) {

	string imageFileName;
	string elementFileName;
	string dilationFileName;
	string erosionFileName;
	string openingFileName;
	string closingFileName;
	string printingFileName;

	if (argc != 8) {
		cout << "Invalid number of arguments.";
		cout << "There should be image file, structuring element file, dilationOutput file, erosionOutput file, openingOutput file, and closingOutputFile.";
		exit(0);
	}
	try {

		imageFileName = argv[1];
		elementFileName = argv[2];
		dilationFileName = argv[3];
		erosionFileName = argv[4];
		openingFileName = argv[5];
		closingFileName = argv[6];
		printingFileName = argv[7];

		ifstream image;
		ifstream element;
		ofstream dilation;
		ofstream erosion;
		ofstream opening;
		ofstream closing;
		ofstream printing;

		image.open(imageFileName);
		element.open(elementFileName);
		dilation.open(dilationFileName);
		erosion.open(erosionFileName);
		opening.open(openingFileName);
		closing.open(closingFileName);
		printing.open(printingFileName);

		Image* morphology = new Image(image, element);

		morphology->reset2DArray(1, morphology->zeroFramedArray);
		morphology->loadImage(image, morphology->zeroFramedArray);
		printing << "Output of ZeroFramaedArray after loading the Image\n\n";
		morphology->prettyPrint(morphology->zeroFramedArray, 1, printing);

		morphology->reset2DArray(2, morphology->structuringElement);
		morphology->loadStructuringElement(element, morphology->structuringElement);
		printing << "Output of Structuring Element\n\n";
		morphology->prettyPrint(morphology->structuringElement, 2, printing);

		morphology->reset2DArray(1, morphology->morphologyArray);
		morphology->computeDilation(morphology->zeroFramedArray, morphology->morphologyArray);
		morphology->printArrayResult(morphology->morphologyArray, dilation);
		printing << "Output of MorphologyArray after Dilation\n\n";
		morphology->prettyPrint(morphology->morphologyArray, 1, printing);

		morphology->reset2DArray(1, morphology->morphologyArray);
		morphology->computeErosion(morphology->zeroFramedArray, morphology->morphologyArray);
		morphology->printArrayResult(morphology->morphologyArray, erosion);
		printing << "Output of MorphologyArray after Erosion\n\n";
		morphology->prettyPrint(morphology->morphologyArray, 1, printing);

		morphology->reset2DArray(1, morphology->morphologyArray);
		morphology->computeOpening(morphology->zeroFramedArray, morphology->morphologyArray);
		morphology->printArrayResult(morphology->morphologyArray, opening);
		printing << "Output of MorphologyArray after Opening\n\n";
		morphology->prettyPrint(morphology->morphologyArray, 1, printing);

		morphology->reset2DArray(1, morphology->morphologyArray);
		morphology->computeClosing(morphology->zeroFramedArray, morphology->morphologyArray);
		morphology->printArrayResult(morphology->morphologyArray, closing);
		printing << "Output of MorphologyArray after Closing\n\n";
		morphology->prettyPrint(morphology->morphologyArray, 1, printing);

		image.close();
		element.close();
		dilation.close();
		erosion.close();
		opening.close();
		closing.close();
		printing.close();

	}
	catch (exception e) {
		cout << "There is an error: " << e.what() << endl;
	}
	return 0;
}