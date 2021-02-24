import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;

public class Image {
	
	private int imageRows;
	private int imageCols;
	private int imageMin;
	private int imageMax;
	
	private int structRows;
	private int structCols;
	private int structMin;
	private int structMax;
	private int originRow;
	private int originCol;
	
	private int rowFrameSize;
	private int colFrameSize;
	
	private int extraRows;
	private int extraCols;
	
	public int[][] zeroFramedArray;
	public int[][] morphologyArray;
	public int[][] structuringElement;
	
	public Image(Scanner input1, Scanner input2) {
		
		if(input1.hasNext()) {
			this.imageRows = input1.nextInt();
		}
		
		if(input1.hasNext()) {
			this.imageCols = input1.nextInt();
		}
		
		if(input1.hasNext()) {
			this.imageMin = input1.nextInt();
		}
		
		if(input1.hasNext()) {
			this.imageMax = input1.nextInt();
		}
		
		if(input2.hasNext()) {
			this.structRows = input2.nextInt();
		}
		
		if(input2.hasNext()) {
			this.structCols = input2.nextInt();
		}
		
		if(input2.hasNext()) {
			this.structMin = input2.nextInt();
		}
		
		if(input2.hasNext()) {
			this.structMax = input2.nextInt();
		}
		
		if(input2.hasNext()) {
			this.originRow = input2.nextInt();
		}
		
		if(input2.hasNext()) {
			this.originCol = input2.nextInt();
		}
		
		this.rowFrameSize = this.structRows / 2;
		this.colFrameSize = this.structCols / 2;
		this.extraRows = this.rowFrameSize * 2;
		this.extraCols = this.colFrameSize * 2;
		
		initializeArrays();		
		
	}
	
	public void initializeArrays() {
		
		zeroFramedArray = new int[this.imageRows + this.extraRows][this.imageCols + this.extraCols];
		morphologyArray = new int[this.imageRows + this.extraRows][this.imageCols + this.extraCols];
		structuringElement = new  int[this.structRows][this.structCols];
		
		for(int i = 0; i < (this.imageRows + this.extraRows); i++) {
			for(int j = 0; j < (this.imageCols + this.extraCols); j++) {
				zeroFramedArray[i][j] = 0;
				morphologyArray[i][j] = 0;
				if(i < this.structRows && j < this.structCols) {
					structuringElement[i][j] = 0;
				}
			}
		}		
	}
	
	public void loadImage(Scanner input, int[][] zeroFramedArray) {
		while(input.hasNext()) {
			
			for(int i = originRow; i < (this.imageRows + this.rowFrameSize); i++) {
				for(int j = originCol; j < (this.imageCols + this.colFrameSize); j++) {
					zeroFramedArray[i][j] = input.nextInt();					
				}
			}				
		}
	}
	
	public void loadStructuringElement(Scanner input, int[][] structuringElement) {
		while (input.hasNext()) {

			for (int i = 0; i < this.structRows; i++) {
				for (int j = 0; j < this.structCols; j++) {
					structuringElement[i][j] = input.nextInt();
				}
			}
		}
	}
	
	public void reset2DArray(int arrayType, int[][] array) {
		
		if(arrayType == 1) {
			for(int i = 0; i < (this.imageRows + this.extraRows); i++) {
				for(int j = 0; j < (this.imageCols + this.extraCols); j++) {
					array[i][j] = 0;					
				}
			}	
		}
		
		else if(arrayType == 2) {
			for (int i = 0; i < this.structRows; i++) {
				for (int j = 0; j < this.structCols; j++) {
					array[i][j] = 0;
				}
			}
		}
	}
	
	public void computeDilation(int[][] zeroFramedArray, int[][] morphologyArray) {
		
		for(int i = this.rowFrameSize; i < (this.imageRows + this.rowFrameSize); i++) {
			for(int j = this.colFrameSize; j < (this.imageCols + this.colFrameSize); j++) {
				if(zeroFramedArray[i][j] > 0) {
					dilation(i, j);
				}
			}
		}
		
	}

	public void computeErosion(int[][] zeroFramedArray, int[][] morphologyArray) {
		
		for(int i = this.rowFrameSize; i < (this.imageRows + this.rowFrameSize); i++) {
			for(int j = this.colFrameSize; j < (this.imageCols + this.colFrameSize); j++) {
				if(zeroFramedArray[i][j] > 0) {
					erosion(i, j);
				}
			}
		}

	}

	public void computeOpening(int[][] zeroFramedArray, int[][] morphologyArray) {
		int[][] tempArray = new int[this.imageRows + this.extraRows][this.imageCols + this.extraCols];
		for(int i = 0; i < (this.imageRows + this.extraRows); i++) {
			for(int j = 0; j < (this.imageCols + this.extraCols); j++) {
				tempArray[i][j] = 0;
			}
		}
		opening(zeroFramedArray, morphologyArray, tempArray);
	}

	public void computeClosing(int[][] zeroFramedArray, int[][] morphologyArray) {
		
		int[][] tempArray = new int[this.imageRows + this.extraRows][this.imageCols + this.extraCols];
		for(int i = 0; i < (this.imageRows + this.extraRows); i++) {
			for(int j = 0; j < (this.imageCols + this.extraCols); j++) {
				tempArray[i][j] = 0;
			}
		}
		closing(zeroFramedArray, morphologyArray, tempArray);
		
	}
	
	public void dilation(int i, int j) {
		int rowOffset, colOffset;
		for(int row = 0; row < structRows; row++) {
			rowOffset = i - originRow;
			for(int col = 0; col < structCols; col++) {
				colOffset = j - originCol;
				if(structuringElement[row][col] == 1) {
					morphologyArray[rowOffset + row][colOffset + col] = structuringElement[row][col];
				}
			}
		}
	}

	public void erosion(int i, int j) {
		boolean match = true;
		int rowOffset, colOffset;
		
		for(int row = 0; row < structRows; row++) {
			rowOffset = i - originRow;
			for(int col = 0; col < structCols; col++) {
				colOffset = j - originCol;
				if(structuringElement[row][col] == 1 && zeroFramedArray[rowOffset + row][colOffset + col] != 1) {
					match = false;
					break;
				}			
			}
			if(!match) {
				break;
			}
		}
		
		if(match) {
			morphologyArray[i][j] = 1;
		}
		else {
			morphologyArray[i][j] = 0;
		}
	}

	public void opening(int[][] zeroFramedArray, int[][] morphologyArray, int[][] array) {
		reset2DArray(1, array);
		computeErosion(zeroFramedArray, array);
		computeDilation(array, morphologyArray);
	}

	public void closing(int[][] zeroFramedArray, int[][] morphologyArray, int[][] array) {
		reset2DArray(1, array);
		computeDilation(zeroFramedArray, array);
		computeErosion(array, morphologyArray);
		
	}
	
	public void printArrayResult(int[][] array, FileWriter output) {
		try {
			
			output.write(imageRows + " " + imageCols + " " + imageMin + " " + imageMax + "\n");
			
			for (int i = 0; i < (this.imageRows + this.extraRows); i++) {
				for (int j = 0; j < (this.imageCols + this.extraCols); j++) {
					output.write(array[i][j] + " ");
				}
				output.write("\n");
			}
			output.write("\n\n");

		} 
		catch(IOException e) {
			e.printStackTrace();
		}
	}
	
	public void prettyPrint(int[][] array, int arrayType, FileWriter output) {
		
		if(arrayType == 1) {
			try {
				for(int i = this.rowFrameSize; i < (this.imageRows + this.extraRows); i++) {
					for(int j = this.colFrameSize; j < (this.imageCols + this.extraCols); j++) {
						if(array[i][j] == 0) {
							output.write(". ");
						}	
						else {
							output.write(array[i][j] + " ");
						}		
					}
					output.write("\n");
				}
				output.write("\n\n");
			}
			catch(IOException e) {
				e.printStackTrace();
			}
		}
		
		else if(arrayType == 2) {
			try {
				for (int i = 0; i < this.structRows; i++) {
					for (int j = 0; j < this.structCols; j++) {
						if(array[i][j] == 0) {
							output.write(". ");
						}	
						else {
							output.write(array[i][j] + " ");
						}
					}
					output.write("\n");
				}
				output.write("\n\n");
			}
			catch(IOException e) {
				e.printStackTrace();
			}
		}
		
	}

}
