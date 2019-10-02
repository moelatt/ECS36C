#ifndef AVL_H
#define AVL_H

#include <memory>
#include <stack>

class AVLTree;

class AVLTreeNode {
 public:
    AVLTreeNode(int key);
    AVLTreeNode(int key, std::weak_ptr<AVLTreeNode> parent);
    bool IsLeaf() const;
    bool HasLeftChild() const;
    bool HasRightChild() const;
    void DeleteChild(std::shared_ptr<AVLTreeNode> v);
    void ReplaceChild(std::shared_ptr<AVLTreeNode> v, std::shared_ptr<AVLTreeNode> u);

 private:
  int key_;
  std::weak_ptr<AVLTreeNode> parent_;
  std::shared_ptr<AVLTreeNode> left_;
  std::shared_ptr<AVLTreeNode> right_;
  int height_;

  friend AVLTree;
}; // class BSTNode

class AVLTree {
 public:
    AVLTree();

    void Insert(int key);
    bool Delete(int key);
    bool Find(int key) const;
    std::string JSON() const;
    size_t size() const;
    bool empty() const;
    int DeleteMin();

 private:
    void DeleteLeaf(std::shared_ptr<AVLTreeNode> currentNode);
    int DeleteMin(std::shared_ptr<AVLTreeNode> currentNode);

    int GetNodeHeightSafe(std::shared_ptr<AVLTreeNode> node) const;
    int GetNodeBalanceSafe(std::shared_ptr<AVLTreeNode> node) const;

    void BalanceTree(std::stack<std::shared_ptr<AVLTreeNode>>& path_stack);
    void RightRotate(std::shared_ptr<AVLTreeNode> node);
    void LeftRotate(std::shared_ptr<AVLTreeNode> node);

    std::shared_ptr<AVLTreeNode> root_;
    size_t size_;
}; // class AVLTree

#endif // AVL_H
