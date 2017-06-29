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
const std::string FULLY_OUTPUT_FILE = "FullyAssociativeSimLog.txt";
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
void fullyAssCacheSim(std::vector<intptr_t> &, std::unordered_map<intptr_t, int> &);
bool FOUND_IN_CACHE(intptr_t, std::unordered_map<intptr_t, int>);
bool CACHE_IS_FULL(std::queue<intptr_t>);
void replaceCacheData(intptr_t, std::unordered_map<intptr_t, int> &, std::queue<intptr_t> &);
void insertCacheData(intptr_t, std::unordered_map<intptr_t, int> &, std::queue<intptr_t> &);
void fullyAssSimResults(std::ofstream &, unsigned int, double, unsigned int);
void simMenu(std::vector<intptr_t> &);
bool clearFile(std::ofstream &, std::string);


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
	//	printVector(traceData);

        // Start simulation
        simMenu(traceData);


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
void fullyAssCacheSim(std::vector<intptr_t> &data, std::unordered_map<intptr_t, int> &cache)
{
	std::cout << "Simulation running..." << std::endl;
	std::queue<intptr_t> LRU; //least recently used
	for (int i = 0; i < data.size(); i++)
	{
		//Report hit or miss and and add data to cache
		if ( FOUND_IN_CACHE(data[i], cache) )
		{
			HIT_COUNT++;
		} 
		else
		{
			if ( CACHE_IS_FULL(LRU) )
			{
				replaceCacheData(data[i], cache, LRU);
			}
			else
			{
				insertCacheData(data[i], cache, LRU);
			}
			MISS_COUNT++;
		}
	} 
}


bool FOUND_IN_CACHE(intptr_t address, std::unordered_map<intptr_t, int> cache)
{
	if (cache.find(address) == cache.end())
	{
		return false;
	}
	return true;
}


bool CACHE_IS_FULL(std::queue<intptr_t> lru)
{
	if (lru.size() < CACHE_MAX_LINES)
	{
		return false;
	}
	return true;
}


void replaceCacheData(intptr_t newAddress, std::unordered_map<intptr_t, int> &cache, std::queue<intptr_t> &lru)
{	
	// Retrieve least recently used address
	intptr_t lruAddress = lru.front();
	lru.pop();
	
	// Replace and set cache data
	cache.erase(lruAddress);
	cache[newAddress] = 1; //1 indicates that data is present
	//cache.insert( std::pair<intptr_t, int>(newAddress, 1) ); //1 indicates that data is present

	// Add new address to lru queue
	lru.push(newAddress);
}


void insertCacheData(intptr_t newAddress, std::unordered_map<intptr_t, int> &cache, std::queue<intptr_t> &lru)
{	
	cache[newAddress] = 1;
	//cache.insert( std::pair<intptr_t, int>(newAddress, 1) );
	lru.push(newAddress);
}


void fullyAssSimResults(std::ofstream &fout, unsigned int cacheLineSize, double timeForSim, unsigned int numOfDataAddresses)
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
	fout << "	Time of Simulation = " << timeForSim/60 << std::endl << std::endl;

}


void simMenu(std::vector<intptr_t> &addressData)
{
        std::ofstream fullyAssSimLog;
        clearFile(fullyAssSimLog, FULLY_OUTPUT_FILE);
        fullyAssSimLog.open(FULLY_OUTPUT_FILE, std::ofstream::out | std::ofstream::app);

        if (fullyAssSimLog.is_open())
        {
                int choice;                                                                       
                do
                {
                        // Display
                        std::cout << "Select one of the options below:" << std::endl;
                        std::cout << "1) Begin Fully Associative Mapping Cache Simulation" << std::endl;
                        std::cout << "2) EXIT and Print Results to " << FULLY_OUTPUT_FILE << std::endl;
                        std::cout << "Enter an option: ";
                        std::cin >> choice;

                        if (choice == 1)
                        {
                                // Gather user data for cache
                                std::unordered_map<intptr_t, int> cache;
                                initSimData();

                                // Run simulation
                                HIT_COUNT = 0;
                                MISS_COUNT = 0;

				std::clock_t begin = clock();
                                fullyAssCacheSim(addressData, cache);
				std::clock_t end = clock();
				double seconds = double(end - begin) / CLOCKS_PER_SEC;

                                fullyAssSimResults(fullyAssSimLog, CACHE_MAX_LINES, seconds, addressData.size());
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

                fullyAssSimLog.close();
        }
        else
        {
                std::cout << "Error: Unable to open " << FULLY_OUTPUT_FILE << std::endl;
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
