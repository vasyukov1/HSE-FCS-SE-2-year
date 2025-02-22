#include "btree.h"
#include <queue>

Node::Node(int t) {
    key = {};
    children = {};
    parent = nullptr;
    isLeaf = true;
    this->t = t;
}

BTree::BTree(int t) : root(nullptr), t_(t), size_(0) {}

void clear(Node* node) {
   if (!node) {
      return;
   }
   for (auto child : node->children) {
      clear(child);
   }
   delete node;
}

BTree::~BTree() {
   clear(root);
}

bool find(Node* node, int key) {
    if (!node) {
       return false;
    }

    for (int i : node->key) {
       if (key == i) {
          return true;
       }
    }

    int index = 0;
    while (index < node->key.size() && node->key[index] < key) {
       ++index;
    }
    if (node->children.empty()) {
       return false;
    }
   return find(node->children[index], key);
}

void split(Node* node, int index, int& size) {
   ++size;
   Node* new_node = new Node(node->t);
   Node* child = node->children[index];
   new_node->isLeaf = child->isLeaf;

  for (int i = 0; i < node->t - 1; ++i) {
     new_node->key.push_back(child->key[i + node->t]);
  }
  if (!child->isLeaf) {
     for (int i = 0; i < node->t; ++i) {
     new_node->children.push_back(child->children[i + node->t]);
     new_node->children.back()->parent = new_node;
     }
  }

  child->key.resize(node->t - 1);
  if (!child->isLeaf) {
     child->children.resize(node->t);
  }

   node->key.insert(node->key.begin() + index, child->key[node->t - 1]);
   node->children.insert(node->children.begin() + index + 1, new_node);
   new_node->parent = node;
}

void insertNode(Node* node, int key, int& size) {
     if (node->isLeaf) {
        node->key.push_back(key);
        for (int i = node->key.size() - 1; i > 0 && node->key[i] < node->key[i - 1]; --i) {
        int tmp = node->key[i];
        node->key[i] = node->key[i - 1];
        node->key[i - 1] = tmp;
        }
     } else {
        int index = node->key.size() - 1;
        while (index >= 0 && node->key[index] > key) {
         --index;
        }
        ++index;
        if (node->children[index]->key.size() == node->t * 2 - 1) {
            split(node, index, size);
           if (key > node->key[index]) {
               ++index;
           }
        }
        insertNode(node->children[index], key, size);
     }
}

void BTree::insert(int key) {
     if (find(root, key)) {
        return;
     }
     if (!root) {
        root = new Node(this->t_);
        root->key = {key};
        ++size_;
     } else if (root->key.size() == this->t_ * 2 - 1) {
        Node* node = root;
        root = new Node(this->t_);
        root->isLeaf = false;
        root->children.push_back(node);
        ++size_;
        split(root, 0, size_);
        insertNode(root, key, size_);
     } else {
        insertNode(root, key, size_);
     }
}

size_t BTree::size() const {
     return size_;
}

int64_t BTree::sum() const {
     if (!root) {
        return 0;
     }
   
     int64_t leaf_sum = 0;
     std::queue<Node*> nodes = {};
     nodes.push(root);
     while (!nodes.empty()) {
        if (nodes.front()->isLeaf) {
           for (int i = 0; i < nodes.front()->key.size(); ++i) {
               leaf_sum += nodes.front()->key[i];
           }
        } else {
           for (int i = 0; i < nodes.front()->children.size(); ++i) {
               nodes.push(nodes.front()->children[i]);
           }
        }
        nodes.pop();
     }
     return leaf_sum;
}