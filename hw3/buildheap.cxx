#include <iostream>
#include <fstream>
#include <string>

#include "json.hpp"

#include "priorityqueue.h"


static const char* META = "metadata";
static const char* MAX_SIZE = "maxHeapSize";
static const char* NUM_OPS = "numOperations";
static const char* OP = "operation";
static const char* KEY = "key";
const char* INSERT = "insert";
const char* REMOVE = "removeMin";

void performOperation(PriorityQueue& pq, const nlohmann::json &jsonOp)
{
    std::string op = jsonOp[OP];

    if (op == INSERT) {
        double key = jsonOp[KEY];
        pq.insert(key);
    }
    else if (op == REMOVE) {
        pq.removeMin();
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: buildheap file.json" << std::endl;
        return 0;
    }
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Can't open input file." << std::endl;
        return -1;
    }
    nlohmann::json jsonIn;
    file >> jsonIn;

    PriorityQueue pq((size_t) jsonIn[META][MAX_SIZE]);
    int numOps = 0;

    for (auto iter = jsonIn.begin(); iter != jsonIn.end(); ++iter)
    {
        if (iter.key() != META)
        {
            ++numOps;
            performOperation(pq, *iter);
        }
    }
    nlohmann::json jsonOut = pq.JSON();
    jsonOut[META][NUM_OPS] = numOps;
    jsonOut[META][MAX_SIZE] = jsonIn[META][MAX_SIZE];

    std::cout << jsonOut.dump(2) << "\n";
    return 0;
}
