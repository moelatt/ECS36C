// Insertion Sort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include "insertionsort.h"

void InsertionSort(std::vector<int>* numbers, int &cmpCount, int &memCount) {
   std::size_t i = 0;
   int j = 0;
   int temp = 0;  // Temporary variable for swap
   
   // Init statistic counters
   cmpCount = 0;
   memCount = 0;

   // Do sorting
   for (i = 1; i < numbers->size(); ++i) {
      j = i;

      // Insert numbers[i] into sorted part
      // stopping once numbers[i] in correct position
      while (j > 0 && (*numbers)[j] < (*numbers)[j - 1]) {
         
         // Swap numbers[j] and numbers[j - 1]
         temp = (*numbers)[j];
         (*numbers)[j] = (*numbers)[j - 1];
         (*numbers)[j - 1] = temp;
         --j;

         // Count done comparison and 6 memory accesses
         memCount += 6;
         cmpCount++;
      }

      // Accesses and comparisons were not counted in while()
      // only if j was (is) > 0, but the comparison faile
      if (j > 0) {
         memCount += 2;
         cmpCount++;
      }

   }
   
   return;
}