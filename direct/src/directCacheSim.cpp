#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <cstdint>
#include <iomanip>


const std::string TRACE_FILE = "EATraceDDOT.out";
const std::string DIRECT_OUTPUT_FILE = "DirectSimLog.txt";
unsigned int HIT_COUNT;
unsigned int MISS_COUNT;


/********************************/
/*	Funciton definitions 	*/
/********************************/
// Input
void traceFileToVector(std::vector<intptr_t> &, int);
void printVector(std::vector<intptr_t> &);
int findFileLength();

// Initialization
void initSimData(std::vector<intptr_t> &);
void generateCacheVector(intptr_t, std::vector<intptr_t> &, intptr_t);

// Simulation
void directCacheSim(std::vector<intptr_t>, std::vector<intptr_t>);
intptr_t findCacheIndex(intptr_t, intptr_t);
bool NOT_EQUAL(intptr_t, intptr_t);
void replaceLineData(intptr_t, std::vector<intptr_t> &, intptr_t);
void directSimResults(std::ofstream &, unsigned int);
void simMenu(std::vector<intptr_t>);
bool clearFile(std::ofstream &, std::string);


/********************************/
/*	      Main      	*/
/********************************/
int main (int argc, char *argv[])
{
	// Input data
	std::vector<intptr_t> traceData;
	std::cout << "Loading simulation data..." << std::endl;
	int fileLineCount = findFileLength();
	traceFileToVector(traceData, fileLineCount-1); //ignore the last line
	
	// Start simulation
	simMenu(traceData);


	return 0;
}


/********************************/
/*    Function Implementation 	*/
/********************************/
// INPUT
void traceFileToVector(std::vector<intptr_t> &src, int fileLen)
{
	std::string temp;
	std::stringstream stream;
	intptr_t address;
	std::ifstream traceIn(TRACE_FILE);

	if (traceIn.is_open())
	{	
		for (int i = 0; i < fileLen; i++)
		{
			traceIn.ignore(256, 'x');
			getline(traceIn, temp);
			address = std::stoul(temp, nullptr, 16);	
			stream << address;
			stream >> std::hex >> address;	
	
			src.push_back(address);
		}
		traceIn.close();
	}
	else
	{
		std::cout << "Error: Unable to open trace file." << std::endl;	
	}
}


void printVector(std::vector<intptr_t> &src)
{
	for (int i = 0; i < src.size(); i++)
	{
		std::cout << " ["<< i << "]: " << src[i] << std::endl;
	}
}


int findFileLength()
{
	int lineCount = 0;
	std::string temp;	
	std::ifstream fin(TRACE_FILE);
	
	if (fin.is_open())
	{	
		while (getline(fin, temp))
		{	
			lineCount++;
		}
		fin.close();
		
		return lineCount;
	}
	else
	{
		std::cout << "Error: Unable to open trace file." << std::endl;
		return 0;
	}
}


// INITIALIZATION
void initSimData(std::vector<intptr_t> &cache_vector)
{
	// Variables
	int words_per_line = 1;
	int line_size = 8 * words_per_line; //in bytes
	intptr_t cache_size;
	intptr_t num_of_lines;

	std::cout << "Input cache size (in bytes): ";
	std::cin >> cache_size;
	num_of_lines = cache_size / line_size;

	// Initialize cache vector elements with the third function input	
	generateCacheVector(num_of_lines, cache_vector, 0); 
}


void generateCacheVector(intptr_t numOfLines, std::vector<intptr_t> &src, intptr_t initial_value)
{
	for (int i = 0; i < numOfLines; i++)
	{
		src.push_back(initial_value);
	}
}


// SIMULATION
void directCacheSim(std::vector<intptr_t> data, std::vector<intptr_t> cache)
{
	intptr_t cacheSize = cache.size();
	for (int i = 0; i < data.size(); i++)
	{
		// Fine line to which the data address maps to
		intptr_t line_index = findCacheIndex(data[i], cacheSize);
		
		// Report hit or miss
		if ( NOT_EQUAL(data[i], cache[line_index]) )
		{
			MISS_COUNT++;
			replaceLineData(data[i], cache, line_index);
		}
		else
		{
			HIT_COUNT++;
		} 	
	}

}


intptr_t findCacheIndex(intptr_t address, intptr_t cache_size)
{
	return address % cache_size;
}


bool NOT_EQUAL(intptr_t a, intptr_t b)
{
	if (a != b)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void replaceLineData(intptr_t memory_address, std::vector<intptr_t> &cache, intptr_t line_index)
{
	cache[line_index] = memory_address;
}


void directSimResults(std::ofstream &fout, unsigned int cacheSize)
{
	// Calculations
	float ratio = (float)HIT_COUNT / (float)MISS_COUNT;
	unsigned int cacheSizeKiB = (cacheSize*8) / 1024;

	// Results
	fout << "Direct Mapping Simulation Results:" << std::endl;
	fout << "          Cache Size(KiB) = " << cacheSizeKiB << std::endl;
	fout << "		      Hits = " << HIT_COUNT << std::endl;
	fout << "	            Misses = " << MISS_COUNT << std::endl;
	fout << "	    Hit/Miss Ratio = " << std::setprecision(5) << ratio << std::endl << std::endl;
}


void simMenu(std::vector<intptr_t> addressData)
{
	std::ofstream directSimLog;                                                    	
        clearFile(directSimLog, DIRECT_OUTPUT_FILE);
        directSimLog.open(DIRECT_OUTPUT_FILE, std::ofstream::out | std::ofstream::app);

	if (directSimLog.is_open())
	{
		int choice;                                                                    		
		do
		{
			// Display
			std::cout << "Select one of the options below:" << std::endl;
			std::cout << "1) Begin Direct-Mapping Cache Simulation" << std::endl;
			std::cout << "2) EXIT and Print Results to " << DIRECT_OUTPUT_FILE << std::endl;
			std::cout << "Enter an option: ";
			std::cin >> choice;
	
			if (choice == 1)
			{
				// Gather user data for cache
				std::vector<intptr_t> cache;
                                initSimData(cache);
				unsigned int cache_size = cache.size();
				
				// Run simulation
				HIT_COUNT = 0;
				MISS_COUNT = 0;
				directCacheSim(addressData, cache);
				directSimResults(directSimLog, cache_size);
			}
			else if (choice == 2)
			{
				std::cout << "Closing simulation program..." << std::endl;
			}
			else
			{
				std::cout << "__Invalid option...try again__" << std::endl;
			}
		} while (choice != 2);
	
		directSimLog.close();
	}
	else
	{
		std::cout << "Error: Unable to open " << DIRECT_OUTPUT_FILE << std::endl;
	}
}


bool clearFile(std::ofstream &srcFile, std::string fName)
{
	srcFile.open(fName, std::ofstream::out | std::ofstream::trunc);
	if (srcFile.is_open())
	{
		srcFile.close();
		return true;
	}
	return false;
}
