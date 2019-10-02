// Merge Sort
// 
// Author: Rob Gysel
// ECS60, UC Davis
// Adapted from: Lysecky & Vahid "Data Structures Essentials", zyBooks

#include <vector>

void MergeSort(std::vector<int>* numbers, int &cmpCount, int &memCount);
void MergeSortRecurse(std::vector<int>* numbers, int i, int k, int &cmpCount, int &memCount);
void Merge(std::vector<int>* numbers, int i, int j, int k, int &cmpCount, int &memCount);