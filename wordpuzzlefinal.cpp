#include "Variables.h"
#include "functions.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


/****************************************************************************************************************/

/* creating a text file & passing by copying */
ofstream outfile("text2DFiltered.txt");	

/*file presence check*/

void fileerrorcheck()
{

	ifstream infile("search2D.txt");

	if (!infile)
	{
		cout << "ERROR: ";
		std::cout << "Can't open input file\n";
	}

}

void fileerrorcheck1()
{

	ifstream infile("text2D.txt");

	if (!infile)
	{
		std::cout << "ERROR: ";
		std::cout << "Can't open input file\n";
	}

}

/****************************************************************************************************************/


// placing  files into array 

ifstream searchFile("search2D.txt");

void readSearchFile(ifstream &searchFile)
{
	// This is a formatting command. It says do not skip // white space when reading from the input stream
	int indexPosWordCollumn = initial; // index of the searched word in the 2D array 
	int indexPosWordRows = initial;
	while (!searchFile.eof())
	{

		char ch;
		searchFile >> ch;
		if (!searchFile.eof())
		{
			if (ch == '\n')
			{

				indexPosWordRows++;  //searching the chars in the rows one by one looping round 
				indexPosWordCollumn = initial; // Resetting the collomns back to 0 so it doesnt go outside the array 
			}
			else
			{
				searchTable[indexPosWordRows][indexPosWordCollumn] = ch; // passing in the current char in the element of the array 
			}

			indexPosWordCollumn++;  //searching the chars in the collumns one by one looping round 

		}

	}
}

ifstream DisplayFile("text2D.txt");

void readDisplayFile(ifstream & displayFile)
{
	// This is a formatting command. It says do not skip // white space when reading from the input stream



	int indexPosWordCollumn = initial; // index of the searched word in the 2D array 
	int indexPosWordRows = initial;
	while (!displayFile.eof())
	{

		char ch;
		displayFile >> ch;
		if (!displayFile.eof())
		{

			if (ch == '\n')
			{

				indexPosWordRows++;  //searching the chars in the rows one by one looping round 
				indexPosWordCollumn = initial; // Resetting the collomns back to 0 so it doesnt go outside the array 
			}
			else
			{
				displayTable[indexPosWordRows][indexPosWordCollumn] = ch; // passing in the current char in the element of the array 
				dummyTable[indexPosWordRows][indexPosWordCollumn] = ch; // Passing the same current char int the element of the dummy array 
			}

			indexPosWordCollumn++;  //searching the chars in the collumns one by one looping round 
		}

	}


}

/*Declaration of Enumarated Types*/

enum Direction
{
	north,
	east,               // creating directional data types that have no assigned value 
	south,
	west,
	southeast,
	southwest,
	orthogonalrightdown,
	orthogonalleftdown,
	orthogonaldownleft,
	orthogonaldownright
};

void replaceChar(string word, int j, int k, Direction direction)
{

	
	if (!outfile)
	{
		cout << "Cannot open output file" << endl;		// checking to see if it can open the file, if not the output 'cannot open file'
		exit(1);			// closes the whole environment 
	}
	
	else
	{
		outfile << word << endl; // stream out the words  to the filtered textfile 
		
	};
	

	/* east direction (east)*/

							// three dummy table statements because there are 3 letters to find to make a word 
	if (direction == east)
	{
		/*placing the dots after the word */
		dummyTable[j][k] = '.';
		dummyTable[j][k + 1] = '.'; // determining the directions of the rows and columns k = columns j = rows  [k + 1] moves the collumn search over to the right till the end line
		dummyTable[j][k + 2] = '.'; // replacing the word with  '...' three dots 
	}

	/* west direction (left)*/
	else if (direction == west)
	{
		dummyTable[j][k] = '.';
		dummyTable[j][k - 1] = '.'; // determining the directions of the rows and columns k = columns j = rows  [k - 1] [k - 2] moves the collumn search over to the left till the end line
		dummyTable[j][k - 2] = '.'; // replacing the word with  '...' three dots 
	}

	/* southeast direction (down right)*/
	else if (direction == southeast)
	{
		dummyTable[j][k] = '.';			// replacing the word with  '...' three dots 
		dummyTable[j + 1][k + 1] = '.'; // determining the directions of the rows and columns k = columns j = rows  [k + 1] [k + 2] moves the collumn search over to the right till the end line
		dummyTable[j + 2][k + 2] = '.'; // Also using the J rows to move down to create a south east direction 
	}

	/* southwest direction (down left)*/
	else if (direction == southwest)
	{
		dummyTable[j][k] = '.';			// replacing the word with  '...' three dots
		dummyTable[j + 1][k - 1] = '.'; // determining the directions of the rows and columns k = columns j = rows  [k + 1] [k + 2] moves the collumn search over to the right till the end line
		dummyTable[j + 2][k - 2] = '.'; // Also using the J rows to move down to create a south east direction 
	}

	else if (direction == north)
	{
		dummyTable[j][k] = '.';
		dummyTable[j - 1][k] = '.';   // as above
		dummyTable[j - 2][k] = '.';
	}


	else if (direction == south)
	{
		dummyTable[j][k] = '.';
		dummyTable[j + 1][k] = '.';
		dummyTable[j + 2][k] = '.';
	}

	else if (direction == orthogonalrightdown)
	{
		dummyTable[j][k] = '.';
		dummyTable[j][k + 1] = '.';
		dummyTable[j + 1][k + 1] = '.';
	}

	else if (direction == orthogonalleftdown)
	{
		dummyTable[j][k] = '.';
		dummyTable[j][k - 1] = '.';
		dummyTable[j + 1][k - 1] = '.';
	}

	else if (direction == orthogonaldownleft)
	{
		dummyTable[j][k] = '.';
		dummyTable[j + 1][k] = '.';
		dummyTable[j + 1][k - 1] = '.';
	}

	else if (direction == orthogonaldownright)
	{
		dummyTable[j][k] = '.';
		dummyTable[j + 1][k] = '.';
		dummyTable[j + 1][k + 1] = '.';
		
	}

	
}

/******************************************************************************************************************************/

int main()
{
	//file error detection 
	fileerrorcheck;

	// wordsearch
	readSearchFile(searchFile);		//passing searchfile by reference into readSearchFile
	readDisplayFile(DisplayFile);	//passing displayfile by reference into readDisplayFile



	for (int i = 0; i < numOfSearchWords; i++)  // i ++ is incrementing by one looping through the list while the variable is less than the total of the words found
	{
		identifiedWord = false;  // setting this boolean to false until it finds a word then it will be set to true
		for (int j = 0; j < ROWS && !identifiedWord; j++) // looping through the rows whilst the boolean is set to false (whilst it hasnt yet found a word
		{
			for (int k = 0; k < COLUMNS && !identifiedWord; k++) // looping through the collumns whilst the boolean is set to false (whilst it hasnt yet found a word
			{
				//word search heading right 
				if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j][k + 1] && searchTable[i][2] == displayTable[j][k + 2]) // k & j = the rows and collumns and it is searching through each individual character in the array 
				{
					std::cout << "found the word " << searchTable[i][firstChar] << searchTable[i][secondChar] << searchTable[i][thirdChar] << " at position " << k << " on line " << j << endl; // outputting [i][0] = 1st letter [i][1] = 2nd letter [i][3] = 3rd letter 
					replaceChar(searchTable[i], j, k, east);  // calling the replace char function to place the characters found with dots, and placing them into the search table by passing it into the function as a parameter 
					identifiedWord = true; // changing the boolean state to true when it has identified a word in the array 
				}

				// word search  heading left
				else if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j][k - 1] && searchTable[i][2] == displayTable[j][k - 2])  // as above     
				{
					std::cout << "found the word " << searchTable[i][firstChar] << searchTable[i][secondChar] << searchTable[i][thirdChar] << " at position " << k << " on line " << j << endl;
					replaceChar(searchTable[i], j, k, west);
					identifiedWord = true;
				}

				// word search heading right down 
				else if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j + 1][k + 1] && searchTable[i][2] == displayTable[j + 2][k + 2])
				{
					std::cout << "found the word " << searchTable[i][firstChar] << searchTable[i][secondChar] << searchTable[i][thirdChar] << " at position " << k << " on line " << j << endl;
					replaceChar(searchTable[i], j, k, southeast);
					identifiedWord = true;
				}

				else if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j + 1][k - 1] && searchTable[i][2] == displayTable[j + 2][k - 2])
				{
					std::cout << "found the word " << searchTable[i][firstChar] << searchTable[i][secondChar] << searchTable[i][thirdChar] << " at position " << k << " on line " << j << endl;
					replaceChar(searchTable[i], j, k, southwest);
					identifiedWord = true;
				}

				//wordsearch heading up 
				else if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j - 1][k] && searchTable[i][2] == displayTable[j - 2][k])
				{
					std::cout << "found the word " << searchTable[i][firstChar] << searchTable[i][secondChar] << searchTable[i][thirdChar] << " at position " << k << " on line " << j << endl;
					replaceChar(searchTable[i], j, k, north);
					identifiedWord = true;
				}
				//wordsearch heading down
				else if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j + 1][k] && searchTable[i][2] == displayTable[j + 2][k])
				{
					std::cout << "found the word " << searchTable[i][firstChar] << searchTable[i][secondChar] << searchTable[i][thirdChar] << " at position " << k << " on line " << j << endl;
					replaceChar(searchTable[i], j, k, south);
					identifiedWord = true;
				}

				//wordsearch heading right down (orthogonal)
				else if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j][k + 1] && searchTable[i][2] == displayTable[j + 1][k + 1])
				{
					std::cout << "found the word " << searchTable[i][firstChar] << searchTable[i][secondChar] << searchTable[i][thirdChar] << " at position " << k << " on line " << j << endl;
					replaceChar(searchTable[i], j, k, south);
					identifiedWord = true;
				}

				//wordsearch heading right down (orthogonal)
				else if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j][k - 1] && searchTable[i][2] == displayTable[j + 1][k - 1])
				{
					std::cout << "found the word " << searchTable[i][firstChar] << searchTable[i][secondChar] << searchTable[i][thirdChar] << " at position " << k << " on line " << j << endl;
					replaceChar(searchTable[i], j, k, south);
					identifiedWord = true;
				}

				//wordsearch heading down right (orthogonal)
				else if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j + 1][k] && searchTable[i][2] == displayTable[j + 1][k - 1])
				{
					std::cout << "found the word " << searchTable[i][firstChar] << searchTable[i][secondChar] << searchTable[i][thirdChar] << " at position " << k << " on line " << j << endl;
					replaceChar(searchTable[i], j, k, south);
					identifiedWord = true;
				}

				//wordsearch heading down left (orthogonal)
				else if (searchTable[i][0] == displayTable[j][k] && searchTable[i][1] == displayTable[j + 1][k] && searchTable[i][2] == displayTable[j + 1][k + 1])
				{
					std::cout << "found the word " << searchTable[i][0] << searchTable[i][1] << searchTable[i][2] << " at position " << k << " on line " << j << endl;
					replaceChar(searchTable[i], j, k, south);
					identifiedWord = true;

				}
				
			}

		}
	}
	
	outfile.close();

	std::cout << endl << endl << endl;
	system("pause");
}

