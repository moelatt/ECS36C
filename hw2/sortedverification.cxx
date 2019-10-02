#include <cstdio>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

#include "json.hpp"

nlohmann::json processSample(const nlohmann::json &sampleJson, bool &hadInversion) {
   // Create object to return
   nlohmann::json resultJson;

   // Copy the sample data
   resultJson["sample"] = sampleJson;

   // Process elements to check for inversion
   for (auto it = sampleJson.begin(); it != sampleJson.end() - 1; ++it) {
      // Inversion counts if this element is greater than next
      if (it.value() > (it + 1).value()) {
         // Add inversion
         resultJson["ConsecutiveInversions"][std::to_string(it - sampleJson.begin())]
          = nlohmann::json {it.value(), (it + 1).value()};
         hadInversion = true;
      }
   }

   // Return info
   return resultJson;
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

   // Extract metadata from JSON
   int arraySize = inputJson["metadata"]["arraySize"];
   int numSamples = inputJson["metadata"]["numSamples"];
   int samplesWithInversions = 0;

   // Process all samples
   nlohmann::json outputJson;
   for (auto it = inputJson.begin(); it != inputJson.end(); ++it) {

      if (it.key() == "metadata") {
         continue;
      }

      // Add output object for the sample
      nlohmann::json sampleOutputJson;
      bool hadInversion = false;
      sampleOutputJson = processSample(it.value(), hadInversion);

      // Count inversion if happened
      if (hadInversion) {
         outputJson[it.key()] = sampleOutputJson; 
         samplesWithInversions++;
      }
   }

   // Fill output JSON metadata with the gathered info
   outputJson["metadata"]["arraySize"] = arraySize;
   outputJson["metadata"]["file"] = argv[1];
   outputJson["metadata"]["numSamples"] = numSamples;
   outputJson["metadata"]["samplesWithInversions"] = samplesWithInversions;

   // Print the output JSON object to standard output
   std::cout << std::setw(4) << outputJson << std::endl;

   return EXIT_SUCCESS;
}