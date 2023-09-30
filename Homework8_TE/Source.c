// ------------------------------------------------------------------------------------------
// Name: Tyler Ethridge
// Class: SET-151 C
// Abstract: Homework 8 Structure Stress
// ------------------------------------------------------------------------------------------

// ------------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// ------------------------------------------------------------------------------------------
// Constants
// ------------------------------------------------------------------------------------------
const int intARRAY_SIZE = 100;
#define true 1
#define false 0

// ------------------------------------------------------------------------------------------
// User Defined Types
// -----------------------------------------------------------------------------------------
typedef int boolean;

typedef struct
{
	long lngRecordID;
	char strFirstName[50];
	char strMiddleName[50];
	char strLastName[50];
	char strStreet[100];
	char strCity[50];
	char strState[50];
	char strZipCode[50];

} udtAddressType;


// ------------------------------------------------------------------------------------------
// Prototypes
// ------------------------------------------------------------------------------------------
void InitializeAddressList(udtAddressType audtAddressList[]); 
	void InitializeAddress(udtAddressType* pudtAddress); 

void PopulateAddressList(udtAddressType audtAddressList[]); 
	int OpenInputFile(char strFileName[], FILE** ppfilInput);
	void AddAddressToArray(char strAddress[], udtAddressType* pudtAddress);
		void GetNextField(char strAddress[], char strNextField[], char chrDelimiter);
		void BreakApartFullName(udtAddressType* pudtAddress, char strFullname[]);
	

void PrintAddressList(udtAddressType audtAddressList[]); 
	void PrintAddress(int intIndex, udtAddressType udtAddress);


//char IsWhiteSpace(char chrLetterToCheck);
int StringLength(char strSource[]);
void StringCopy( char strDestination[ ], char strSource[ ] );
void AppendString(char strDestination[], char strSource[]);
int FindCharcterInString(char strSource[], char chrLetterToFind);
void Trim(char strSource[]);
boolean IsWhiteSpace(char chrLetterToCheck);
void BreakApartFullName(udtAddressType* pudtAddress, char strFullname[]);
int WordCounter(char strSource[]);
int ReadNextLineFromFile(FILE* pfilInput, char strBuffer[]);
//-------------------------------------------------------------------------------------------
// Name: main
// Abstract: This is where the program starts
// -----------------------------------------------------------------------------------------
void main()
{
	udtAddressType audtAddressList[100];

	InitializeAddressList(audtAddressList);

	PopulateAddressList(audtAddressList);

	PrintAddressList(audtAddressList);

	system("pause");
}





// --------------------------------------------------------------------------------
// Name: InitializeAddressList
// Abstract: Initialize all the Address in the List
// --------------------------------------------------------------------------------
void InitializeAddressList(udtAddressType audtAddressList[])
{
	int intIndex = 0;

	//pass a single array element by pointer
	for (intIndex = 0; intIndex < intARRAY_SIZE; intIndex += 1)
	{
		InitializeAddress(&audtAddressList[intIndex]);
	}
}




// --------------------------------------------------------------------------------
// Name: InitializeAddress
// Abstract: Abstracts all the values to 0 or empty strings
// --------------------------------------------------------------------------------
void InitializeAddress(udtAddressType* pudtAddress)
{
	pudtAddress->lngRecordID = 0;
	StringCopy(pudtAddress->strFirstName, "");
	StringCopy(pudtAddress->strMiddleName, "");
	StringCopy(pudtAddress->strLastName, "");
	StringCopy(pudtAddress->strStreet, "");
	StringCopy(pudtAddress->strCity, "");
	StringCopy(pudtAddress->strState, "");
	StringCopy(pudtAddress->strZipCode, "");
}





// --------------------------------------------------------------------------------
// Name: PopulateAddressList
// Abstract: Copy the source to the destination
// --------------------------------------------------------------------------------
void PopulateAddressList(udtAddressType audtAddressList[])
{
	//Declare A file pointer
	FILE* pfilInput = 0;
	int intResultFlag = 0;
	char strBuffer[100] = "";
	char chrLetter = 0;
	int intIndex = 0;

	// Try to open the file for reading (notice you have to double up the backslashes)
	//intResultFlag = OpenInputFile("c:\\temp\\Addresses1.txt", &pfilInput);
	//intResultFlag = OpenInputFile("c:\\temp\\AddressesDebug.txt", &pfilInput);
	

	intResultFlag = OpenInputFile("a:\\CincinnatiState\\Addresses1.txt", &pfilInput);



	//was the file opened?
	if (intResultFlag == 1)
	{
		//yes, read in records until end of file (EOF)
		while (feof(pfilInput) == 0)
		{
			ReadNextLineFromFile(pfilInput, strBuffer);
			

			// Read one character 
			//chrLetter = fgetc( pfilInput ); 

			// Print out line to screen 
			//printf("%s\n", strBuffer);

			//addong to array
			AddAddressToArray(strBuffer, &audtAddressList[intIndex]);

		}

		// Clean up
		fclose(pfilInput);
	}

}




// --------------------------------------------------------------------------------
// Name: OpenInputFile
// Abstract: Open the file for reading.  Return true if successful.
// --------------------------------------------------------------------------------
int OpenInputFile(char strFileName[], FILE** ppfilInput)
{

	int intResultFlag = 0;

	// Open the file for reading
	*ppfilInput = fopen(strFileName, "rb");

	// Success?
	if (*ppfilInput != 0)
	{
		// Yes
		intResultFlag = 1;
	}
	else
	{
		// No
		printf("Error opening %s for reading!!!\n", strFileName);
	}

	return intResultFlag;

}




// --------------------------------------------------------------------------------
// Name: ReadNextLineFromFile
// Abstract: Read until the end of the line ro file
// --------------------------------------------------------------------------------
int ReadNextLineFromFile(FILE* pfilInput, char strBuffer[])
{
	int intIndex = 0;
	char chrLetter = 0;

	//read data in the file
	while (feof(pfilInput) == 0)
	{
		//yes get the next char
		chrLetter = fgetc(pfilInput);

		//add to string
		strBuffer[intIndex] = chrLetter;

		//count it 
		intIndex += 1;

		//end of the line?
		if (chrLetter == '\n')
		{
			//yes, stop and break
			break;
		}

	}

	//terminate
	strBuffer[intIndex] = 0;
}


// --------------------------------------------------------------------------------
// Name: AddAddressToArray
// Abstract: Add address to the array
// --------------------------------------------------------------------------------
void AddAddressToArray(char strAddress[], udtAddressType* pudtAddress)
{
	char strNextField[50] = "";

	// Append a comma so parsing last field is just like all the others
	AppendString(strAddress, ",");

	// Record ID
	GetNextField(strNextField, strAddress, ',');

	//converting the ascII to long
	pudtAddress->lngRecordID = atol(strNextField);

	// Full Name
	GetNextField(strNextField, strAddress, ',');
	BreakApartFullName(pudtAddress, strNextField);

	// Street Address
	GetNextField(strNextField, strAddress, ',');
	StringCopy(pudtAddress->strStreet, strNextField);

	// City
	GetNextField(strNextField, strAddress, ',');
	StringCopy(pudtAddress->strCity, strNextField);

	// State
	GetNextField(strNextField, strAddress, ',');
	StringCopy(pudtAddress->strState, strNextField);

	// Zip Code
	GetNextField(strNextField, strAddress, ',');
	StringCopy(pudtAddress->strZipCode, strNextField);
}




// --------------------------------------------------------------------------------
// Name: BreakApartFullName
// Abstract: seperate the first, middle and last name of the user
// --------------------------------------------------------------------------------
void BreakApartFullName(udtAddressType* pudtAddress, char strFullname[])
{
	char strNextField[50] = "";
	int intWordCount = 0;

	intWordCount = WordCounter(strFullname);

	//append a space
	AppendString(strFullname, " ");

	//First NAme
	GetNextField(strNextField, strFullname, ' ');
	StringCopy(pudtAddress -> strMiddleName, strNextField);

	//middle name
	if (intWordCount > 2)
	{
		//Middle Name
		GetNextField(strNextField, strFullname, ' ');
		StringCopy(pudtAddress->strMiddleName, strNextField);
	}

	//Last Name (whatever is left is the last name)
	StringCopy(pudtAddress->strLastName, strFullname);
}




// --------------------------------------------------------------------------------
// Name: StringLength
// Abstract: Return the string length
// --------------------------------------------------------------------------------
int StringLength(char strSource[])
{
	int intIndex = 0;
	int intLength = 0;

	// Pre-test because string may be empty
	while (strSource[intIndex] != 0)
	{
		intIndex += 1;
	}

	intLength = intIndex;

	return intLength;
}



// ------------------------------------------------------------------------------------------
// Name: AppendString
// Abstract: This will append the source string to the end of the destination string
// ------------------------------------------------------------------------------------------
void AppendString(char strDestination[], char strSource[])
{
	//Variables
	int intIndex = 0;
	int intEndIndex = 0;

	intEndIndex = StringLength(strDestination);

	while (strSource[intIndex] != 0)
	{
		//will append the letter of the string to the strDestination
		strDestination[intEndIndex] += strSource[intIndex];

		//incrimate the index
		intIndex += 1;
		intEndIndex += 1;
	}

	//termimnate the string
	strDestination[intEndIndex] = 0;
}

// --------------------------------------------------------------------------------
// Name: GetNextField
// Abstract: Gets the next string after the delimiter
// --------------------------------------------------------------------------------
void GetNextField(char strAddress[], char strSource[], char chrDelimiter)
{
	int intDelimiterIndex = 0;
	int intIndex = 0;

	//find first comma
	intDelimiterIndex = FindCharcterInString(strAddress, chrDelimiter);

	//did you find the comma
	if (intDelimiterIndex > 0)
	{
		//copy everyrthing up to comma inti next field
		for (intIndex = 0; intIndex < intDelimiterIndex; intIndex += 1)
		{
			strSource[intIndex] = strAddress[intIndex];
		}

		//terminate
		strSource[intIndex] = 0;

		//resets the zero
		intIndex = 0;

		//copy everything from thr middle of the tring
		while (strAddress[intDelimiterIndex + 1 + intIndex] != 0)
		{
			strAddress[intIndex] = strAddress[intDelimiterIndex + 1 + intIndex];

			intIndex += 1;
		}

		//terminate
		strAddress[intIndex] = 0;
		
		//trim the white spaces
		Trim(strAddress);
		Trim(strSource);

	}

	//int intCommaIndex = 0;
	//int intIndex = 0;
	//char strBuffer[100] = "";
	//char intAddressIndex = 0;

	////find first comma
	//intCommaIndex = FindCharcterInString(strAddress, ',');

	////did you find the comma
	//if (intCommaIndex > 0)
	//{
	//	//copy everyrthing up to comma inti next field
	//	for (intIndex = 0; intIndex < intCommaIndex; intIndex += 1)
	//	{
	//		strNextField[intIndex] = strAddress[intIndex];
	//	}

	//	//terminate
	//	strNextField[intIndex] = 0;

	//	//resets the zero
	//	intIndex = 0;
	//	intAddressIndex = intCommaIndex + 1;

	//	//copy everything after teh comma up to the end of the string
	//	while (strAddress[intAddressIndex] != 0)
	//	{
	//		strBuffer[intIndex] = strAddress[intAddressIndex];

	//		intIndex += 1;
	//		intAddressIndex += 1;
	//	}

	//	//terminate
	//	strBuffer[intIndex] = 0;

	//	//copy back to the address
	//	StringCopy(strAddress, strBuffer);
	//}
	
}



// ------------------------------------------------------------------------------------------
// Name: WordCounter
// Abstract: This will count the words in the string
// ------------------------------------------------------------------------------------------
int WordCounter(char strSource[])
{
	//variables
	int intIndex = 0;
	int intWordCount = 0;

	while (strSource[intIndex] != 0)
	{
		//checking to see if there a space there
		if (strSource[intIndex] == ' ')
		{
			intWordCount += 1;
		}

		//incrimate the index
		intIndex += 1;
	}

	//add one more number for the count since there are one less space then words in the sentence
	intWordCount += 1;

	return intWordCount;
}





// ------------------------------------------------------------------------------------------
// Name: FindCharcterInString
// Abstract: This will iterate through the string and find the Charcter (case sensitive) 
// ------------------------------------------------------------------------------------------
int FindCharcterInString(char strSource[], char chrLetterToFind)
{
	//Variables
	int intIndex = 0;
	int intIndexofLetter = -1;

	//check charcter
	while (strSource[intIndex] != 0)
	{
		//checking to see if the index chr is equal to the entered char
		if (strSource[intIndex] == chrLetterToFind)
		{
			intIndexofLetter = intIndex;

			//stor searching
			break;
		}
		//incrimate the index
		intIndex += 1;
	}

	//return the value
	return intIndexofLetter;
}




// --------------------------------------------------------------------------------
// Name: PrintAddressList
// Abstract: Printing the adress list to the console
// --------------------------------------------------------------------------------
void PrintAddressList(udtAddressType audtAddressList[])
{
	int intIndex = 0;

	for (intIndex = 0; intIndex < intARRAY_SIZE; intIndex += 1)
	{
		if (audtAddressList[intIndex].lngRecordID >= 1)
		{
			// Pass a single array element by pointer 
			PrintAddress(intIndex, audtAddressList[intIndex]);
		}

	}
}



// -------------------------------------------------------------------------------- 
// Name: PrintAddress 
// Abstract: Print all the structure field values 
// -------------------------------------------------------------------------------- 
void PrintAddress(int intIndex, udtAddressType udtAddress)
{
	printf("\n");
	printf("Address #%2d ----------------------------------------\n", intIndex + 1);
	printf("\tAddress ID		: %ld\n", udtAddress.lngRecordID);
	printf("\tFirst Name		: %s\n", udtAddress.strFirstName);
	printf("\tMiddle Name		: %s\n", udtAddress.strMiddleName);
	printf("\tLast Name			: %s\n", udtAddress.strLastName);
	printf("\tStreet Address	: %s\n", udtAddress.strStreet);
	printf("\tCity				: %s\n", udtAddress.strCity);
	printf("\tState				: %s\n", udtAddress.strState);
	printf("\tZipcode			: %s\n", udtAddress.strZipCode);
}







// --------------------------------------------------------------------------------
// Name: StringCopy
// Abstract: Copy the source to the destination
// --------------------------------------------------------------------------------
void StringCopy(char strDestination[], char strSource[])
{
	int intIndex = 0;

	// Copy each character
	while (strSource[intIndex] != 0)
	{
		strDestination[intIndex] = strSource[intIndex];
		intIndex += 1;
	}

	// Terminate
	strDestination[intIndex] = 0;

}



// --------------------------------------------------------------------------------
// Name: Trim
// Abstract: Remove leading and trailing whitespace (space, tab or newline)
// --------------------------------------------------------------------------------
void Trim(char strSource[])
{

	int intIndex = 0;
	int intFirstNonWhitespaceIndex = -1;
	int intLastNonWhitespaceIndex = 0;
	int intSourceIndex = 0;
	int intDestinationIndex = 0;

	// Default first non-whitespace character index to end of string in case string is all whitespace
	// Bug fix.  Not in video.
	intFirstNonWhitespaceIndex = StringLength(strSource);

	// Find first non-whitespace character
	while (strSource[intIndex] != 0)
	{
		// Non-whitespace character?
		if (IsWhiteSpace(strSource[intIndex]) == false)
		{
			// Yes, save the index
			intFirstNonWhitespaceIndex = intIndex;

			// Stop searching!
			break;
		}

		// Next character
		intIndex += 1;
	}

	// Find the last non-whitespace character
	while (strSource[intIndex] != 0)
	{
		// Non-whitespace character?
		if (IsWhiteSpace(strSource[intIndex]) == false)
		{
			// Yes, save the index
			intLastNonWhitespaceIndex = intIndex;
		}

		// Next character
		intIndex += 1;

		//copys everything inbetweenm
		for (intSourceIndex = intFirstNonWhitespaceIndex; intSourceIndex <= intLastNonWhitespaceIndex; intSourceIndex += 1)
		{
			//copy next char
			strSource[intDestinationIndex] = strSource[intSourceIndex];

			intDestinationIndex += 1;
		}

		//terminate
		strSource[intDestinationIndex] = 0;
	}
}




//// --------------------------------------------------------------------------------
//// Name: FindLetterIndex
//// Abstract: Find first occurence of letter (case-sensitive) from left to right.
//// --------------------------------------------------------------------------------
//int FindLetterIndex(char strSource[], char chrLetterToFind)
//{
//	int intLetterToFindIndex = -1;
//	int intIndex = 0;
//
//	// Check character
//	while (strSource[intIndex] != 0)
//	{
//		// Is this the character for which we are looking
//		if (strSource[intIndex] == chrLetterToFind)
//		{
//			// Yes, save the index
//			intLetterToFindIndex = intIndex;
//
//			// Stop searching
//			break;
//		}
//		intIndex += 1;
//	}
//
//	return intLetterToFindIndex;
//}
//
//
//

//
//// --------------------------------------------------------------------------------
//// Name: IsWhiteSpace
//// Abstract: Return true if letter is a space, tab, newline or carriage return
//// --------------------------------------------------------------------------------
//	char IsWhiteSpace(char chrLetterToCheck);
//	{
//		char blnIsWhiteSpace = false;
//		
//		// Space
//		if (chrLetterToCheck == ' ') blnIsWhiteSpace = true;
//
//		// Tab
//		if (chrLetterToCheck == '\t') blnIsWhiteSpace = true;
//
//		// Carriarge return
//		if (chrLetterToCheck == '\r') blnIsWhiteSpace = true;
//
//		// Line feed
//		if (chrLetterToCheck == '\n') blnIsWhiteSpace = true;
//
//		return blnIsWhiteSpace;
//	}
//
//
//	// Any non-whitepsace characters?
//	if (intFirstNonWhitespaceIndex >= 0)
//	{
//		// Yes, copy everything in between
//		for (intSourceIndex = intFirstNonWhitespaceIndex; intSourceIndex <= intLastNonWhitespaceIndex; intSourceIndex += 1)
//		{
//			// Copy next character
//			strSource[intDestinationIndex] = strSource[intSourceIndex];
//
//			intDestinationIndex += 1;
//		}
//	}
//
//	// Terminate 
//	strSource[intDestinationIndex] = 0;
//}



// --------------------------------------------------------------------------------
// Name: IsWhiteSpace
// Abstract: Return true if letter is a space, tab, newline or carriage return
// --------------------------------------------------------------------------------
boolean IsWhiteSpace(char chrLetterToCheck)
{
	boolean blnIsWhiteSpace = false;

	// Space
	if (chrLetterToCheck == ' ') blnIsWhiteSpace = true;

	// Tab
	if (chrLetterToCheck == '\t') blnIsWhiteSpace = true;

	// Carriarge return
	if (chrLetterToCheck == '\r') blnIsWhiteSpace = true;

	// Line feed
	if (chrLetterToCheck == '\n') blnIsWhiteSpace = true;

	return blnIsWhiteSpace;
}

//
//// --------------------------------------------------------------------------------
//// Name: CountWords
//// Abstract: Count the words in a string
//// --------------------------------------------------------------------------------
//int CountWords(char strSource[])
//{
//	int intWordCount = 0;
//	int intIndex = 0;
//	char chrCurrentLetter = 0;
//	char chrPreviousLetter = ' ';
//
//	// Copy each character
//	while (strSource[intIndex] != 0)
//	{
//		chrCurrentLetter = strSource[intIndex];
//
//		// Word boundary (space followed by non-space)?
//		if (chrPreviousLetter == ' ' && chrCurrentLetter != ' ')
//		{
//			// Yes, count it
//			intWordCount += 1;
//		}
//
//		// Save current character
//		chrPreviousLetter = chrCurrentLetter;
//
//		// Next
//		intIndex += 1;
//	}
//
//	return intWordCount;
//}