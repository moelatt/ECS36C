// Merge Sort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include "mergesort.h"

void MergeSort(std::vector<int>* numbers, int &cmpCount, int &memCount) {
   // Init statistic counters
   cmpCount = 0;
   memCount = 0;

   // Do the sorting
   MergeSortRecurse(numbers, 0, numbers->size() - 1, cmpCount, memCount);
}


void MergeSortRecurse(std::vector<int>* numbers, int i, int k, int &cmpCount, int &memCount) {
   int j = 0;
   
   if (i < k) {
      j = (i + k) / 2;  // Find the midpoint in the partition
      
      // Recursively sort left and right partitions
      MergeSortRecurse(numbers, i, j, cmpCount, memCount);
      MergeSortRecurse(numbers, j + 1, k, cmpCount, memCount);
      
      // Merge left and right partition in sorted order
      Merge(numbers, i, j, k, cmpCount, memCount);
   }
}

void Merge(std::vector<int>* numbers, int i, int j, int k, int &cmpCount, int &memCount) {
   int mergedSize = k - i + 1;                // Size of merged partition
   int mergePos = 0;                          // Position to insert merged number
   int leftPos = 0;                           // Position of elements in left partition
   int rightPos = 0;                          // Position of elements in right partition
   std::vector<int> mergedNumbers;
   mergedNumbers.resize(mergedSize);          // Dynamically allocates temporary array
                                              // for merged numbers
   
   leftPos = i;                               // Initialize left partition position
   rightPos = j + 1;                          // Initialize right partition position
   
   // Add smallest element from left or right partition to merged numbers
   while (leftPos <= j && rightPos <= k) {
      if ((*numbers)[leftPos] < (*numbers)[rightPos]) {
         mergedNumbers[mergePos] = (*numbers)[leftPos];
         ++leftPos;
      }
      else {
         mergedNumbers[mergePos] = (*numbers)[rightPos];
         ++rightPos;
      }
      ++mergePos;

      // Count comparison and 2 accesses made by condition
      // And 2 accesses for the assignment
      cmpCount++;
      memCount += 4;
   }
   
   // If left partition is not empty, add remaining elements to merged numbers
   while (leftPos <= j) {
      mergedNumbers[mergePos] = (*numbers)[leftPos];
      ++leftPos;
      ++mergePos;

      // Count 2 accesses made by assignment
      memCount += 2;
   }
   
   // If right partition is not empty, add remaining elements to merged numbers
   while (rightPos <= k) {
      mergedNumbers[mergePos] = (*numbers)[rightPos];
      ++rightPos;
      ++mergePos;

      // Count 2 accesses made by assignment
      memCount += 2;
   }
   
   // Copy merge number back to numbers
   for (mergePos = 0; mergePos < mergedSize; ++mergePos) {
      (*numbers)[i + mergePos] = mergedNumbers[mergePos];
      
      // Count 2 accesses made by assignment
      memCount += 2;
   }
}