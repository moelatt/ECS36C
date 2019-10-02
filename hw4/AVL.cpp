#include "AVL.h"

#include <cassert>
#include <iostream>
#include <string>
#include <stack>
#include <queue>

#include "json.hpp"


AVLTreeNode::AVLTreeNode(int key) :
    key_(key),
    parent_(std::weak_ptr<AVLTreeNode>()),
    left_(nullptr),
    right_(nullptr),
    height_(1) {}

AVLTreeNode::AVLTreeNode(int key, std::weak_ptr<AVLTreeNode> parent) :
    key_(key),
    parent_(parent),
    left_(nullptr),
    right_(nullptr),
    height_(1) {}

bool AVLTreeNode::IsLeaf() const {
    return left_ == nullptr && right_ == nullptr;
}

bool AVLTreeNode::HasLeftChild() const {
    return left_ != nullptr;
}

bool AVLTreeNode::HasRightChild() const {
    return right_ != nullptr;
}

void AVLTreeNode::DeleteChild(std::shared_ptr<AVLTreeNode> v) {
    if (left_ == v) {
        left_ = nullptr;
    } else if (right_ == v) {
        right_ = nullptr;
    } else {
        std::cerr << "BSTNode::DeleteChild Error: non-child passed as argument\n";
        exit(EXIT_FAILURE);
    }
}

void AVLTreeNode::ReplaceChild(std::shared_ptr<AVLTreeNode> v, std::shared_ptr<AVLTreeNode> u) {
    if (left_ == u || right_ == u) {
        std::cerr << "BSTNode::ReplaceChild Error: child passed as replacement\n";
    }
    if (left_ == v) {
        left_ = u;
        u->parent_ = v->parent_;
    } else if (right_ == v) {
        right_ = u;
        u->parent_ = v->parent_;
    } else {
        std::cerr << "BSTNode::ReplaceChild Error: non-child passed as argument\n";
        exit(EXIT_FAILURE);
    }
}

AVLTree::AVLTree() : root_(nullptr), size_(0) {}

void AVLTree::Insert(int key) {
    if (root_ == nullptr) {
        root_ = std::make_shared<AVLTreeNode>(key);
        size_++;
        return;
    }
    std::stack<std::shared_ptr<AVLTreeNode>> path_stack;
    std::shared_ptr<AVLTreeNode> currentNode = root_;
    while (currentNode != nullptr) {
        path_stack.push(currentNode);
        currentNode = key < currentNode->key_ ? currentNode->left_ : currentNode->right_;
    }

    if (key < path_stack.top()->key_) {
        path_stack.top()->left_ = std::make_shared<AVLTreeNode>(key, path_stack.top());
    } else {
        path_stack.top()->right_ = std::make_shared<AVLTreeNode>(key, path_stack.top());
    }
    size_++;

    //BalanceTree(path_stack);
    while(!path_stack.empty()) {
        std::shared_ptr<AVLTreeNode> node = path_stack.top();
        path_stack.pop();

        node->height_ = std::max(GetNodeHeightSafe(node->left_), GetNodeHeightSafe(node->right_))+1;
        int balance = GetNodeHeightSafe(node->left_) - GetNodeHeightSafe(node->right_);


        // Left Left Case
        if (balance > 1 && key < node->left_->key_)
            RightRotate(node);
            // Right Right Case
        else if (balance < -1 && key > node->right_->key_)
            LeftRotate(node);
        // Left Right Case
        else if (balance > 1 && key > node->left_->key_)
        {
            LeftRotate(node->left_);
            RightRotate(node);
        }
        // Right Left Case
        else if (balance < -1 && key < node->right_->key_)
        {
            RightRotate(node->right_);
            LeftRotate(node);
        }
    }
}

bool AVLTree::Delete(int key) {
    std::stack<std::shared_ptr<AVLTreeNode>> path_stack;

    std::shared_ptr<AVLTreeNode> currentNode = root_;
    while (currentNode != nullptr) {
        path_stack.push(currentNode);
        if (currentNode->key_ == key) {
            path_stack.pop(); // the element on top is going to be deleted
            if (currentNode->IsLeaf()) {
                DeleteLeaf(currentNode);
            } else if (currentNode->left_ == nullptr) {
                assert(currentNode->right_ != nullptr);
                std::shared_ptr<AVLTreeNode> parent = currentNode->parent_.lock();
                parent->ReplaceChild(currentNode, currentNode->right_);
                size_--;
            } else if (currentNode->right_ == nullptr) {
                assert(currentNode->left_ != nullptr);
                std::shared_ptr<AVLTreeNode> parent = currentNode->parent_.lock();
                parent->ReplaceChild(currentNode, currentNode->left_);
                size_--;
            } /*else {
                currentNode->key_ = DeleteMin(currentNode);
            }*/
        }
        currentNode = (key < currentNode->key_) ?
            currentNode->left_ : currentNode->right_;
    }

    BalanceTree(path_stack);

    return false;
}

bool AVLTree::Find(int key) const {
    std::shared_ptr<AVLTreeNode> currentNode = root_;
    while (currentNode != nullptr) {
        if (currentNode->key_ == key) {
            return true;
        }
        currentNode = (key < currentNode->key_) ?
            currentNode->left_ : currentNode->right_;
    }
    return false;
}

std::string AVLTree::JSON() const {
    nlohmann::json result;
    std::queue< std::shared_ptr<AVLTreeNode> > nodes;
    if (root_ != nullptr) {
        result["height"] = root_->height_-1;
        result["root"] = root_->key_;
        nodes.push(root_);
        while (!nodes.empty()) {
            auto v = nodes.front();
            nodes.pop();
            std::string key = std::to_string(v->key_);
            result[key]["balance factor"] = GetNodeHeightSafe(v->right_) - GetNodeHeightSafe(v->left_);
            result[key]["height"] = GetNodeHeightSafe(v)-1;
            if (v->left_ != nullptr) {
                result[key]["left"] = v->left_->key_;
                nodes.push(v->left_);
            }
            if (v->right_ != nullptr) {
                result[key]["right"] = v->right_->key_;
                nodes.push(v->right_);
            }
            if (v->parent_.lock() != nullptr) {
                result[key]["parent"] = v->parent_.lock()->key_;
            } else {
                result[key]["root"] = true;
            }
        }
    }
    result["size"] = size_;
    return result.dump(2) + "\n";
}

int AVLTree::DeleteMin() {
    return DeleteMin(root_);
}


void AVLTree::DeleteLeaf(std::shared_ptr<AVLTreeNode> currentNode) {
    std::shared_ptr<AVLTreeNode> parent = currentNode->parent_.lock();
    if (parent == nullptr) {
        // Delete root
        root_ = nullptr;
        size_--; assert(size_ == 0);
    } else {
        if (parent->right_ == currentNode) {
            parent->right_ = nullptr;
        } else if (parent->left_ == currentNode) {
            parent->left_ = nullptr;
        } else {
            std::cerr << "BST::DeleteLeaf Error: inconsistent state\n";
        }
        size_--;
    }
}

int AVLTree::DeleteMin(std::shared_ptr<AVLTreeNode> currentNode) {
    std::stack<std::shared_ptr<AVLTreeNode>> path_stack;

    std::shared_ptr<AVLTreeNode> lastNode = nullptr;
    while (currentNode != nullptr) {
        path_stack.push(currentNode);
        lastNode = currentNode;
        currentNode = currentNode->left_;
    }
    int result = lastNode->key_;
    std::shared_ptr<AVLTreeNode> parent = lastNode->parent_.lock();
    if (parent == nullptr) {
        // lastNode is root
        if (lastNode->right_ != nullptr) {
            root_ = lastNode->right_;
            lastNode->right_->parent_.reset();
        } else {
            root_ = nullptr;
        }
    } else {
        // lastNode under the root
        if (lastNode->right_ != nullptr) {
            parent->left_ = lastNode->right_;
            lastNode->right_->parent_ = parent;
        } else {
            parent->left_ = nullptr;
        }
  }
    size_--;

    path_stack.pop(); // the element on top is the deleted element
    BalanceTree(path_stack);

    return result;
}

int AVLTree::GetNodeHeightSafe(std::shared_ptr<AVLTreeNode> node) const {
    if (!node)
        return 0;
    return node->height_;
}

int AVLTree::GetNodeBalanceSafe(std::shared_ptr<AVLTreeNode> node) const {
    if (!node)
        return 0;
    return GetNodeHeightSafe(node->left_) - GetNodeBalanceSafe(node->right_);
}

void AVLTree::BalanceTree(std::stack<std::shared_ptr<AVLTreeNode> >& path_stack) {
    while(!path_stack.empty()) {
        std::shared_ptr<AVLTreeNode> node = path_stack.top();
        path_stack.pop();

        node->height_ = std::max(GetNodeHeightSafe(node->left_), GetNodeHeightSafe(node->right_))+1;
        int balance = GetNodeHeightSafe(node->left_) - GetNodeHeightSafe(node->right_);


        // Left Left Case
        if (balance > 1 && GetNodeBalanceSafe(node->left_) >= 0)
            RightRotate(node);
        // Right Right Case
        else if (balance < -1 && GetNodeBalanceSafe(node->right_) <= 0 )
            LeftRotate(node);
        // Left Right Case
        else if (balance > 1 && GetNodeBalanceSafe(node->left_) < 0)
        {
            LeftRotate(node->left_);
            RightRotate(node);
        }
        // Right Left Case
        else if (balance < -1 && GetNodeBalanceSafe(node->right_) > 0)
        {
            RightRotate(node->right_);
            LeftRotate(node);
        }
    }
}

void AVLTree::RightRotate(std::shared_ptr<AVLTreeNode> node) {
    std::shared_ptr<AVLTreeNode> parent = node->parent_.lock();
    std::shared_ptr<AVLTreeNode> left_child = node->left_;
    std::shared_ptr<AVLTreeNode> left_child_right_child = left_child->right_;

    left_child->parent_ = node->parent_;
    node->parent_ = left_child;
    if (left_child_right_child)
        left_child_right_child->parent_ = node;

    left_child->right_ = node;
    node->left_ = left_child_right_child;

    if (parent) {
        if (parent->left_ == node)
            parent->left_ = left_child;
        else
            parent->right_ = left_child;
    }
    else
        root_ = left_child;

    node->height_ = std::max(GetNodeHeightSafe(node->left_), GetNodeHeightSafe(node->right_))+1;
    left_child->height_ = std::max(GetNodeHeightSafe(left_child->left_), GetNodeHeightSafe(left_child->right_))+1;
}

void AVLTree::LeftRotate(std::shared_ptr<AVLTreeNode> node) {
    std::shared_ptr<AVLTreeNode> parent = node->parent_.lock();
    std::shared_ptr<AVLTreeNode> right_child = node->right_;
    std::shared_ptr<AVLTreeNode> right_child_left_child = right_child->left_;

    right_child->parent_ = node->parent_;
    node->parent_ = right_child;
    if (right_child_left_child)
        right_child_left_child->parent_ = node;

    right_child->left_ = node;
    node->right_ = right_child_left_child;

    if (parent) {
        if (parent->left_ == node)
            parent->left_ = right_child;
        else
            parent->right_ = right_child;
    }
    else
        root_ = right_child;

    node->height_ = std::max(GetNodeHeightSafe(node->left_), GetNodeHeightSafe(node->right_))+1;
    right_child->height_ = std::max(GetNodeHeightSafe(right_child->left_), GetNodeHeightSafe(right_child->right_))+1;
}

size_t AVLTree::size() const {
    return size_;
}

bool AVLTree::empty() const {
    return size_ == 0;
}


