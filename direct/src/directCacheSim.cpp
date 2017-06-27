#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

const std::string TRACE_FILE = "EATraceDDOT.out";


// Funciton definitions
void traceFileToVector(std::vector<unsigned int> &);
void printDVector(std::vector<unsigned int> &);


int main (int argc, char *argv[])
{
	std::vector<unsigned int> traceData;
	traceFileToVector(traceData);
	//printDVector(traceData);	
	std::cout << traceData[0] << std::endl;	

	return 0;
}


void traceFileToVector(std::vector<unsigned int> &src)
{
	unsigned int temp;
	std::string line;	
	std::ifstream traceIn;
	std::stringstream ss;
	traceIn.open(TRACE_FILE);

	if (traceIn.is_open())
	{
		std::cout << "in IF" << std::endl;
		while (!traceIn.fail())
		{
			getline(traceIn, line, '\n');
			ss << std::hex << line;
			ss >> temp;
			std::cout << temp << std::endl;
			src.push_back(temp);	
		}
		traceIn.close();
	}
	else
	{
		std::cout << "Error: Unable to open trace file." << std::endl;	
	}
}


void printDVector(std::vector<unsigned int> &src)
{
	for (int i = 0; i < src.size(); i++)
	{
		std::cout << "Vector ["<< i << "]: " << src[i] << std::endl;
	}
}
