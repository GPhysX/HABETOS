#include <SD.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "utilities.h"
#include <WProgram.h>

#define ASCII_ZERO 48

/**
 * This function will search a SD card to find the next available filename to be used
 * for data logging. It will automatically increment the number appened to the base filename
 * up to the max number supplied.
 *
 * This function returns 0 if an available filename was found, and 1 if no files are available.
 *
 * @param sdCard A pointer to the SD card object to search
 * @param Basename A character array containing the base filename to use in the search
 * @param endFilename A character array containing the first free filename
 * @param maxFileNumber The max number to append to the end of the base filename
 * @param baseNameLength The length of the base filename
 * @return Code stating if a file has been found
 */
uint8_t findNextFilename(SDClass* sdCard, char *Basename, char *extension, char *endFilename, uint8_t maxFileNumber) {
	uint8_t i=0;
	uint8_t digitsNeeded = 0;
	uint8_t HundredsPlace = 0, TenthsPlace = 0, OnesPlace = 0;
	char *tempFilename;
	uint8_t baseNameLength = strlen(Basename);
	uint8_t extensionLength = strlen(extension);

	// Determine how many digits the numbers after the filename require
	if (maxFileNumber > 99) {
		digitsNeeded = 3;
		HundredsPlace = baseNameLength;
		TenthsPlace = baseNameLength + 1;
		OnesPlace = baseNameLength + 2;
		strcat(Basename, "000");
	} else if (maxFileNumber > 9) {
		digitsNeeded = 2;
		TenthsPlace = baseNameLength;
		OnesPlace = baseNameLength + 1;
		strcat(Basename, "00");
	} else {
		digitsNeeded = 1;
		OnesPlace = baseNameLength;
		strcat(Basename, "0");
	}
	
	// Create the temporary file name
	tempFilename = (char *) calloc(baseNameLength + digitsNeeded + extensionLength, sizeof(char));
	strcpy(tempFilename, Basename);
	strcat(tempFilename, extension);

	for (i=0; i<maxFileNumber; i++) {
		// Determine the next filename to try
		switch (digitsNeeded) {
		case 3:
			tempFilename[HundredsPlace] = '0' + (i / 100);
		case 2:
			tempFilename[TenthsPlace] = '0' + (i / 10);
		case 1:
			tempFilename[OnesPlace] = '0' + (i % 10);
		}

		// Check to see if the file exists
		if ( !sdCard->exists(tempFilename) ) {
			// If it does not exist, copy the name used into the output buffer and exit the for loop
			strcpy(endFilename, tempFilename);
			return(0);
		}
	}
	return(1);
}

