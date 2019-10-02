#include <iostream>
#include <cassert>
#include <cstdlib>
#include "json.hpp"

#include "priorityqueue.h"

PriorityQueue::PriorityQueue(std::size_t max_size) :
    nodes_(max_size + 1, KeyValuePair()),
    max_size_(max_size),
    size_(0){}

void PriorityQueue::insert(Key k) {
    insert(std::make_pair(k, std::make_pair(0, 0)));
}
void PriorityQueue::insert(KeyValuePair keyValue) {
    assert(size_ < max_size_);
    nodes_[++size_] = keyValue;
	heapifyUp(size_);
}
KeyValuePair PriorityQueue::min() {
    assert(!isEmpty());
    return nodes_[ROOT];
}
KeyValuePair PriorityQueue::removeMin() {
    assert(!isEmpty());
    KeyValuePair keyValue = nodes_[ROOT];
    removeNode(ROOT);
    return keyValue;
}
bool PriorityQueue::isEmpty() const {
    return (size_ == 0);
}
size_t PriorityQueue::size() const {
    return size_;
}
nlohmann::json PriorityQueue::JSON() const {
    nlohmann::json result;
    for (size_t i = 1; i <= size_; i++)
    {
        nlohmann::json node;
        KeyValuePair keyValue = nodes_[i];
        node["key"] = keyValue.first;
        node["value"] = keyValue.second;
        if (i != ROOT) {
            node["parent"] = std::to_string(i / 2);
        }
        if (2 * i <= size_) {
            node["leftChild"] = std::to_string(2 * i);
        }
        if (2 * i + 1 <= size_) {
            node["rightChild"] = std::to_string(2 * i + 1);
        }
        result[std::to_string(i)] = node;
    }
    result["metadata"]["max_size"] = max_size_;
    result["metadata"]["size"] = size_;
    return result;
}
void PriorityQueue::heapifyUp(size_t i) {
    size_t child = i;
    size_t parent = child / 2;
    while (parent >= ROOT && getKey(child) < getKey(parent)) {
        std::swap(nodes_[child], nodes_[parent]);

        child = parent;
        parent = child / 2;
    }
}
void PriorityQueue::heapifyDown(size_t i)
{
    size_t parent = i;
	size_t iMin = 0;

	while (iMin != parent) {
		iMin = parent;

		size_t left = parent * 2; 
		size_t right = parent * 2 + 1; 

		if (left <= size_ && getKey(left) < getKey(iMin))
			iMin = left;

		if (right <= size_ && getKey(right) < getKey(iMin))
			iMin = right;

		if (iMin != parent) {
			if (getKey(left) == getKey(right)) {
                iMin = rand() % 2 ? left : right;
            }
			std::swap(nodes_[iMin], nodes_[parent]);

			parent = iMin;
			iMin = 0;
		}
	}
}
void PriorityQueue::removeNode(size_t i) {
    nodes_[i] = nodes_[size_--];
    heapifyDown(i);
}
Key PriorityQueue::getKey(size_t i)
{
    return nodes_[i].first;
}
