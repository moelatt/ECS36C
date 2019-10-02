#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "json.hpp"

nlohmann::json compareSamples(const std::string &file1, const std::string &file2,
      const nlohmann::json &sampleJson1, const nlohmann::json &sampleJson2,
      bool &Mismatch) {
   // Create object to return
   nlohmann::json resultJson;

   // Copy the samples data
   resultJson[file1] = sampleJson1;
   resultJson[file2] = sampleJson2;

   // Process elements to check for data mismatches
   for (auto it1 = sampleJson1.begin(), it2 = sampleJson2.begin();
         it1 != sampleJson1.end() && it2 != sampleJson2.end();
         ++it1, ++it2) {
      // Mismatch counts if the element from first file is different from other
      if (it1.value() != it2.value()) {
         // Add mismatch
         resultJson["Mismatches"][std::to_string(it1 - sampleJson1.begin())] = 
               nlohmann::json{it1.value(), it2.value()};
         Mismatch = true;
      }
   }

   // Return info
   return resultJson;
}

int main(int argc, char** argv) {
   // Verify usage
	if (argc != 3) {
		printf("Usage: %s file1.json file2.json\n", argv[0]);
		return EXIT_FAILURE;
	}

   // Try to open supplied files as an input file
   std::ifstream inputFile1(argv[1]);
   if (!inputFile1.is_open()) {
      std::cout << "Cannot open the input file " << argv[1] << std::endl;
      return EXIT_FAILURE;
   }
   std::ifstream inputFile2(argv[2]);
   if (!inputFile2.is_open()) {
      std::cout << "Cannot open the input file " << argv[2] << std::endl;
      return EXIT_FAILURE;
   }

   // Read JSON objects from the supplied files
   nlohmann::json inputJson1, inputJson2;
   inputFile1 >> inputJson1;
   inputFile2 >> inputJson2;

   // Close the input files
   inputFile1.close();
   inputFile2.close();

   // Extract metadata from JSONs
   int arraySize1 = inputJson1["metadata"]["arraySize"];
   int numSamples1 = inputJson1["metadata"]["numSamples"];
   int arraySize2 = inputJson2["metadata"]["arraySize"];
   int numSamples2 = inputJson2["metadata"]["numSamples"];
   int samplesWithConflictingResults = 0;

   if (arraySize1 != arraySize2 || numSamples1 != numSamples2) {
      std::cout << "Cannot compare files due to the different array size and/or samples count"
            << std::endl;
      return EXIT_FAILURE;
   }

   // Process all samples from file 1
   nlohmann::json outputJson;
   for (auto it = inputJson1.begin(); it != inputJson1.end(); ++it) {
      // Skip metadata, because it can't be a sample
      if (it.key() == "metadata") {
         continue;
      }

      // Check if the same sample exists in the second file
      if (inputJson2.find(it.key()) == inputJson2.end()) {
         std::cout << "Second file does not contains sample \"" << it.key()
               << "\" mentioned in the first file" << std::endl;
         return EXIT_FAILURE;
      }

      // Extract sample arrays, and verify their result
      const nlohmann::json &sampleJson1 = it.value();
      const nlohmann::json &sampleJson2 = inputJson2[it.key()];
      if (sampleJson1.size() != sampleJson2.size()) {
         std::cout << "Sample \"" << it.key()
               << "\" has inconsistent size in files" << std::endl;
         return EXIT_FAILURE;
      }

      // Add output object for the sample
      nlohmann::json samplesOutputJson;
      bool Mismatch = false;
      samplesOutputJson = compareSamples(argv[1], argv[2], 
            sampleJson1, sampleJson2, Mismatch);

      // Count inconsistency if happened
      if (Mismatch) {
         outputJson[it.key()] = samplesOutputJson; 
         samplesWithConflictingResults++;
      }
   }

   // Fill output JSON metadata with the gathered info
   outputJson["metadata"]["File1"]["arraySize"] = arraySize1;
   outputJson["metadata"]["File1"]["name"] = argv[1];
   outputJson["metadata"]["File1"]["numSamples"] = numSamples1;
   outputJson["metadata"]["File2"]["arraySize"] = arraySize2;
   outputJson["metadata"]["File2"]["name"] = argv[2];
   outputJson["metadata"]["File2"]["numSamples"] = numSamples2;
   outputJson["metadata"]["samplesWithConflictingResults"] = samplesWithConflictingResults;

   // Print the output JSON object to standard output
   std::cout << std::setw(4) << outputJson << std::endl;

   return EXIT_SUCCESS;
}