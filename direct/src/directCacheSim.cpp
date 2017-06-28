#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>


const std::string TRACE_FILE = "EATraceDDOT.out";


// Funciton definitions
void traceFileToVector(std::vector<unsigned int> &, int);
void printVector(std::vector<unsigned int> &);
int findFileLength(std::string);
void getCacheParams(double &, double &, double &);
void generateCacheVector(double, std::vector<unsigned int> &);


int main (int argc, char *argv[])
{
	// Input data
	std::vector<unsigned int> traceData;
	std::cout << "Loading simulation data..." << std::endl;
	int fileLineCount = findFileLength(TRACE_FILE);
	traceFileToVector(traceData, fileLineCount-1); //ignore the last line
		//printVector(traceData);


	// Gather user data for cache
	double cacheSize;
	double cacheLineSize;
	double cacheNumOfLines;
	std::vector<unsigned int> cache;
	getCacheParams(cacheSize, cacheLineSize, cacheNumOfLines);
//		std::cout << "TEST user input funciton..." << std::endl;
//		std::cout << "      cacheSize(bytes): " << cacheSize << std::endl;
//		std::cout << "  cacheLineSize(bytes): " << cacheLineSize << std::endl;
//		std::cout << "cacheNumOfLines(bytes): " << cacheNumOfLines << std::endl;
	generateCacheVector(cacheNumOfLines, cache);
	printVector(cache);
	


	return 0;
}


void traceFileToVector(std::vector<unsigned int> &src, int fileLen)
{
	std::string temp;
	unsigned int address;
	std::stringstream ss;
	std::ifstream traceIn;
	traceIn.open(TRACE_FILE);

	if (traceIn.is_open())
	{	
		for (int i = 0; i < fileLen; i++)
		{
			getline(traceIn, temp);
			address = std::stoul(temp, nullptr, 16);	
			src.push_back(address);
		}
		traceIn.close();
	}
	else
	{
		std::cout << "Error: Unable to open trace file." << std::endl;	
	}
}


void printVector(std::vector<unsigned int> &src)
{
	for (int i = 0; i < src.size(); i++)
	{
		std::cout << " ["<< i << "]: " << src[i] << std::endl;
	}
}


int findFileLength(std::string fileName)
{
	int lineCount = 0;
	std::string temp;	
	std::ifstream fin;
	fin.open(fileName);
	
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


void getCacheParams(double &cacheSize, double &lineSize, double &numOfLines)
{
	double wordsPerLine;
	std::cout << "Input cache size (in bytes): ";
	std::cin >> cacheSize;
	std::cout << "      Input # of words/line: ";
	std::cin >> wordsPerLine;

	lineSize = 8 * wordsPerLine;
	numOfLines = cacheSize / lineSize;
}


void generateCacheVector(double numOfLines, std::vector<unsigned int> &src)
{
	unsigned int initial_value = 0;
	for (int i = 0; i < numOfLines; i++)
	{
		src.push_back(initial_value);
	}
}
