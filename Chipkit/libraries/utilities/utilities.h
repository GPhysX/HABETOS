#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <SD.h>
#include <stdint.h>
#include "WProgram.h"

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
uint8_t findNextFilename(SDClass* sdCard, char *Basename, char *extension, char *endFilename, uint8_t maxFileNumber);

#endif
