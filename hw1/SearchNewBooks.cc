// Copyright 2019 MOE LATT
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono> // NOLINT (build/c++11)
#include <sstream>

class Clock {
 private:
    std::chrono::high_resolution_clock::time_point start;
 public:
    void Reset() {
        start = std::chrono::high_resolution_clock::now();
    }
    double CurrentTime() {
        auto end = std::chrono::high_resolution_clock::now();
        double elapsed_us = std::chrono::duration<double,
std::micro>(end - start).count();
        return elapsed_us;
    }
};

class books{
 private:
    std::string type;
    int ISBN;
 public:
    void setIsbn(int x) {
        ISBN = x;
    }
    void setType(std::string y) {
        type = y;
    }
    int putIsbn() {
        return ISBN;
    }
    std::string putType() {
        return type;
    }
};
std::ostream &operator <<(std::ostream &out, std::vector<books> &myvector) {
    for (int i = 0; i < myvector.size(); i++) {
        out << myvector[i].putIsbn() << std::endl;
        out << myvector[i].putType() << std::endl;
    }
    return out;
}
int setVectorFile(std::string file, std::vector<books> *myvector) {
    std::ifstream myfile;
    myfile.open(file);
     if (!myfile) {
        std::cout << "Error: cannot open file " << file << std::endl;
        return -1;
    }
    std::string character;
     while (!myfile.eof()) {
        books* pointer;
        getline(myfile, character, ',');
        pointer = new books;
        int x;
        std::stringstream convertToInteger(character);
        convertToInteger >> x;
        pointer -> setIsbn(x);
        getline(myfile, character);
        pointer ->setType(character);
        myvector->push_back(*pointer);
    }
    return true;
}
int algorithmSort(books x, books y) {
    if (x.putIsbn() == y.putIsbn()) {
        return x.putType() < y.putType();
    } else {
        return x.putIsbn() < y.putIsbn();
    }
}
bool binarySearch(std::vector<books> myvector, int search, std::string type) {
    int mid;
    int low = 0;
    int find = -1;
    int high = myvector.size() - 1;
     while (low <= high) {
        mid = (high + low) / 2;
            if ( search == myvector[mid].putIsbn() &&
type == myvector[mid].putType() ) {
                return true;
            } else if (search > myvector[mid].putIsbn() ||
(search == myvector[mid].putIsbn() && type > myvector[mid].putType())) {
                low = mid + 1;
            } else {
                high = mid - 1;
         }
    }
    return false;
}
bool linearSearch(std::vector<books>myvector, int search, std:: string type) {
    for (int i = 0; i < myvector.size(); i++) {
        if (search == myvector[i].putIsbn() && type == myvector[i].putType()) {
           return true;
        }
    }
    return false;
}
int main(int argc, char **argv) {
    bool found = true;
    std::string myfile1, myfile2;
    std::vector<books> NewBooks;
    std::vector<books> RequestdBooks;
    Clock ct;

    if (argc != 4 || argv[1] == NULL || argv[2] == NULL || argv[3] == NULL) {
    std::cout << "Usage: ./SearchNewBooks <newBooks.dat>";
    std::cout << "<request.dat> <result_file.dat>" << std::endl;
    return -1;
    }
    myfile1 = argv[1];
    myfile2 = argv[2];
    char character;
    if (setVectorFile(myfile1, &NewBooks) == -1) {
        return -1;
    }
    if (setVectorFile(myfile2, &RequestdBooks) == -1) {
         return -1;
    }
    std::cout << "Choice of search method ([l]inear, [b]inary)?" << std::endl;
    std::cin >> character;
    std::ofstream out;
    out.open(argv[3]);
    if (!out) {
        return -1;
    }
    int count = 0;
    while (std::cin) {
        ct.Reset();
    switch (character) {
        case 'l':
            for (int i = 0; i < RequestdBooks.size(); i++) {
              if (linearSearch(NewBooks, RequestdBooks[i].putIsbn(),
RequestdBooks[i].putType()) == found) {
                   count++;
               }
            }
            out << count << std::endl;
            std::cout << "CPU time: " << ct.CurrentTime()
<< " microseconds" << std::endl;
            exit(0);
            break;
        case 'b' :
            sort(NewBooks.begin(), NewBooks.end(), algorithmSort);
            for (int i = 0; i < RequestdBooks.size(); i++) {
              if (binarySearch(NewBooks, RequestdBooks[i].putIsbn(),
RequestdBooks[i].putType()) == found) {
                    count++;
                }
            }
            out << count << std::endl;
            std::cout << "CPU time: " << ct.CurrentTime()
<< " microseconds" << std::endl;
            exit(0);
            break;
            default:
            std::cout << "Invalid choice" << std::endl;
            std::cout << "Choice of search method ([l]inear, [b]inary)?"
<< std::endl;
            std::cin >> character;
            }
    }
}


























