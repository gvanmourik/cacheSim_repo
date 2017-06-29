#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <math.h>
#include <cstdint>
#include <iomanip>
#include <unordered_map>
#include <queue>
#include <ctime>


const std::string TRACE_FILE = "EATraceDDOT.out";
const std::string FULLY_OUTPUT_FILE = "SetAssociativeSimLog.txt";
unsigned int HIT_COUNT;
unsigned int MISS_COUNT;
unsigned int CACHE_MAX_LINES;


/********************************/
/*      Funciton definitions    */  
/********************************/
// Input
void traceFileToVector(std::vector<intptr_t> &, int);
void printVector(std::vector<intptr_t> &); 
int findFileLength();

// Initialization
void initSimData();

// Simulation
void setAssociativeSim(std::vector<intptr_t> &data, std::vector< std::unordered_map<intptr_t, int> > &cache);
bool FOUND_IN_SET(intptr_t targetAddress, std::unordered_map<intptr_t, int> set);
bool SET_IS_FULL(std::queue<intptr_t> fifo_tracker);
void replaceSetData(intptr_t newAddress, std::unordered_map<intptr_t, int> &set, std::queue<intptr_t> &FIFO);
void insertSetData(intptr_t newAddress, std::unordered_map<intptr_t, int> &set, std::queue<intptr_t> &FIFO);
void setAssociativeSimResults(std::ofstream &fout, unsigned int cacheLineSize, double timeForSim, unsigned int numOfDataAddresses);
void simMenu(std::vector<intptr_t> &addressData);
bool clearFile(std::ofstream &target, std::string fName);


/********************************/
/*            Main              */
/********************************/
int main (int argc, char *argv[])
{
        // Input data
        std::vector<intptr_t> traceData;
        std::cout << "Loading simulation data..." << std::endl;
        int fileLineCount = findFileLength();
        traceFileToVector(traceData, fileLineCount-1); //ignore the last line
        //      printVector(traceData);

        // Start simulation
        //simMenu(traceData);


        return 0;
}


/********************************/
/*    Function Implementation   */
/********************************/  
// INPUT
void traceFileToVector(std::vector<intptr_t> &src, int fileLen)
{
        std::string temp;
        intptr_t address;
        std::ifstream traceIn(TRACE_FILE);

        if (traceIn.is_open())
        {
                for (int i = 0; i < fileLen; i++)
                {
                        traceIn.ignore(256, 'x');
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
void initSimData()
{
        // Variables
        int words_per_line = 1;
        int line_size = 8 * words_per_line; //in bytes
        intptr_t cache_size;

        std::cout << "Input cache size (in bytes): ";
        std::cin >> cache_size;
        CACHE_MAX_LINES = cache_size / line_size;
}


// SIMULATION
void setAssociativeSim(std::vector<intptr_t> &data, std::vector< std::unordered_map<intptr_t, int> > &cache, int number_of_sets)
{
	std::cout << "Simulation running..." << std::endl;
	std::vector< std::queue<intptr_t> FIFO(number_of_sets); //Collection of FIFO trackers for each set
	for (int i = 0; i < data.size(); i++)
	{
		// Determine set
		int set_index = data[i] % number_of_sets;

		// Report hit or miss and add data to cache
		if ( FOUND_IN_SET(data[i], cache[set_index]) )
		{
			HIT_COUNT++;
		}
		else
		{
			if ( SET_IS_FULL(FIFO[set_index]) )
			{
				replaceSetData(data[i], cache[set_index], FIFO[set_index]);
			}
			else
			{
				insertSetData(data[i], cache[set_index], FIFO[set_index]);
			}
			MISS_COUNT++;
		}
	}
}


bool FOUND_IN_SET(intptr_t targetAddress, std::unordered_map<intptr_t, int> set)
{
	if (set.find(targetAddress) == set.end())
	{
		return false;
	}
	return true;
}


bool SET_IS_FULL(std::queue<intptr_t> fifo_tracker)
{
	if (fifo_tracker.size() < CACHE_MAX_LINES)
	{
		return false;
	}
	return true;
}


void replaceSetData(intptr_t newAddress, std::unordered_map<intptr_t, int> &set, std::queue<intptr_t> &FIFO)
{
	// Retrieve address that has resided in the set the longest
	intptr_t fifoAddress = FIFO.front();
	FIFO.pop();

	// Replace and set data
	set.erase(fifoAddress);
	set[newAddress] = 1; //1 indicates that data is present
	
	// Add new address to fifo tracker
	FIFO.push(newAddress);
}


void insertSetData(intptr_t newAddress, std::unordered_map<intptr_t, int> &set, std::queue<intptr_t> &FIFO)
{
	set[newAddress] = 1;
	FIFO.push(newAddress);
}


void setAssociativeSimResults(std::ofstream &fout, unsigned int cacheLineSize, double timeForSim, unsigned int numOfDataAddresses)
{
	// Calculations
        float ratio = (float)HIT_COUNT / (float)MISS_COUNT;
        unsigned int cacheSizeKiB = (cacheLineSize*8) / 1024;

        // Results
        fout << "Fully Associative Mapping Simulation Results:" << std::endl;
        fout << "          Cache Size(KiB) = " << cacheSizeKiB << std::endl;
        fout << "                     Hits = " << HIT_COUNT << std::endl;
        fout << "                   Misses = " << MISS_COUNT << std::endl;
        fout << "           Hit/Miss Ratio = " << std::setprecision(5) << ratio << std::endl;
        fout << " Number of Data Addresses = " << numOfDataAddresses << std::endl;
        fout << "       Time of Simulation = " << timeForSim/60 << " minutes" <<  std::endl << std::endl;
}





