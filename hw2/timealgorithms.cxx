#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

#include "json.hpp"

#include "insertionsort.h"
#include "mergesort.h"
#include "quicksort.h"

typedef void (*SortingFunction)(std::vector<int> *, int &, int &);

void processArrayWithAlgorithm(std::vector<int> numbers, SortingFunction sortFunc) {
   clock_t time;
   time = clock();
   int comparisonsCount, memoryAccessCount;
   sortFunc(&numbers, comparisonsCount, memoryAccessCount);
   time = clock() - time;
   std::cout << "," << ((double)time)/CLOCKS_PER_SEC << "," << comparisonsCount << "," << memoryAccessCount;
}

int main(int argc, char** argv) {
   // Verify usage
	if (argc != 2) {
		printf("Usage: %s file.json\n", argv[0]);
		return EXIT_FAILURE;
	}

   // Try to open supplied file as an input file
   std::ifstream inputFile(argv[1]);
   if (!inputFile.is_open()) {
      std::cout << "Cannot open the input file " << argv[1] << std::endl;
      return EXIT_FAILURE;
   }

   // Read JSON object from the supplied file
   nlohmann::json inputJson;
   inputFile >> inputJson;

   // Close the input file
   inputFile.close();

   // Print CSV output headers
   std::cout << "Sample,InsertionSortTime,InsertionSortCompares,InsertionSortMemaccess,"
         << "MergeSortTime,MergeSortCompares,MergeSortMemaccess,"
         << "QuickSortTime,QuickSortCompares,QuickSortMemaccess" << std::endl;
   // Set fixed point float style
   std::cout << std::fixed;

   // Process all samples
   for (auto it = inputJson.begin(); it != inputJson.end(); ++it) {
      // Skip metadata, because it can't be a sample
      if (it.key() == "metadata") {
         continue;
      }

      // Print sample name
      std::cout << it.key();

      // Print different algorithms stats
      processArrayWithAlgorithm(it.value(), InsertionSort);
      processArrayWithAlgorithm(it.value(), MergeSort);
      processArrayWithAlgorithm(it.value(), QuickSort);

      // Print newline
      std::cout << std::endl;
   }

   return EXIT_SUCCESS;
}