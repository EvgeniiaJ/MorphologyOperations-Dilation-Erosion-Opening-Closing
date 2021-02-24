import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class MorphologyOperations {

	public static void main(String[] args) {
		
		String imageFileName;
		String elementFileName;
		String dilationFileName;
		String erosionFileName;
		String openingFileName;
		String closingFileName;
		String printingFileName;

		if (args.length != 7) {
			System.out.println("Invalid number of arguments.");
			System.out.println(
					"There should be image file, structuring element file, dilationOutput file, erosionOutput file, openingOutput file, and closingOutputFile.");
			System.exit(0);
		}
		try {

			imageFileName = args[0];
			elementFileName = args[1];
			dilationFileName = args[2];
			erosionFileName = args[3];
			openingFileName = args[4];
			closingFileName = args[5];
			printingFileName = args[6];
			
			Scanner image = new Scanner(new File(imageFileName));
			Scanner element = new Scanner(new File(elementFileName));
			FileWriter dilation = new FileWriter(new File(dilationFileName));
			FileWriter erosion = new FileWriter(new File(erosionFileName));
			FileWriter opening = new FileWriter(new File(openingFileName));
			FileWriter closing = new FileWriter(new File(closingFileName));
			FileWriter printing = new FileWriter(new File(printingFileName));

			Image morphology = new Image(image, element);
			morphology.reset2DArray(1, morphology.zeroFramedArray);
			morphology.loadImage(image, morphology.zeroFramedArray);
			printing.write("Output of ZeroFramaedArray after loading the Image\n\n");
			morphology.prettyPrint(morphology.zeroFramedArray, 1, printing);
			
			morphology.reset2DArray(2, morphology.structuringElement);
			morphology.loadStructuringElement(element, morphology.structuringElement);			
			printing.write("Output of Structuring Element\n\n");
			morphology.prettyPrint(morphology.structuringElement, 2, printing);
			
			morphology.reset2DArray(1, morphology.morphologyArray);
			morphology.computeDilation(morphology.zeroFramedArray, morphology.morphologyArray);
			morphology.printArrayResult(morphology.morphologyArray, dilation);
			printing.write("Output of MorphologyArray after Dilation\n\n");
			morphology.prettyPrint(morphology.morphologyArray, 1, printing);
			
			morphology.reset2DArray(1, morphology.morphologyArray);
			morphology.computeErosion(morphology.zeroFramedArray, morphology.morphologyArray);
			morphology.printArrayResult(morphology.morphologyArray, erosion);
			printing.write("Output of MorphologyArray after Erosion\n\n");
			morphology.prettyPrint(morphology.morphologyArray, 1, printing);
			
			morphology.reset2DArray(1, morphology.morphologyArray);
			morphology.computeOpening(morphology.zeroFramedArray, morphology.morphologyArray);
			morphology.printArrayResult(morphology.morphologyArray, opening);
			printing.write("Output of MorphologyArray after Opening\n\n");
			morphology.prettyPrint(morphology.morphologyArray, 1, printing);
			
			morphology.reset2DArray(1, morphology.morphologyArray);
			morphology.computeClosing(morphology.zeroFramedArray, morphology.morphologyArray);
			morphology.printArrayResult(morphology.morphologyArray, closing);
			printing.write("Output of MorphologyArray after Closing\n\n");
			morphology.prettyPrint(morphology.morphologyArray, 1, printing);
			
			image.close();
			element.close();
			dilation.close();
			erosion.close();
			opening.close();
			closing.close();
			printing.close();

		} catch (IOException e) {
			e.printStackTrace();
		}
	}

}
