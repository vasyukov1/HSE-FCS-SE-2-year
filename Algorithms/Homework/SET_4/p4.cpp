#include "rbtree.h"
#include <algorithm>

Node::Node(int key) {
  this->key = key;
  left = nullptr;
  right = nullptr;
  parent = nullptr;
  color = Color::RED;
  height = 1;
  size = 1;
}

int countHeight(RBTree* tree, Node* node) {
  if (!node) {
    return 0;
  }

  int height = countHeight(tree, node->left);
  height = countHeight(tree, node->right);
  node->height = height;

  if (node->color == Color::BLACK) {
    ++node->height;
  }

  int size = 1;
  if (node->left) {
    size += node->left->size;
  }

  if (node->right) {
    size += node->right->size;
  }

  node->size = size;
  return node->height;
}

void rotateLeft(RBTree* tree, Node* &node) {
  Node* child = node->right;
  node->right = child->left;

  if (node->right)
    node->right->parent = node;

  child->parent = node->parent;
  if (!node->parent) {
    tree->root = child;
  } else if (node == node->parent->left) {
    node->parent->left = child;
  } else {
    node->parent->right = child;
  }

  child->left = node;
  node->parent = child;
}

void rotateRight(RBTree* tree, Node* &node) {
  Node* child = node->left;
  node->left = child->right;

  if (node->left)
    node->left->parent = node;

  child->parent = node->parent;
  if (!node->parent) {
    tree->root = child;
  } else if (node == node->parent->left) {
    node->parent->left = child;
  } else {
    node->parent->right = child;
  }

  child->right = node;
  node->parent = child;
}

void fixInsert(RBTree* tree, Node* &node) {
  Node* parent = nullptr;
  Node* grandparent = nullptr;
  
  while (node != tree->root && node->color == Color::RED && node->parent->color == Color::RED) {
    parent = node->parent;
    grandparent = parent->parent;
    
    if (parent == grandparent->left) {
      Node* uncle = grandparent->right;
      if (uncle && uncle->color == Color::RED) {
        grandparent->color = Color::RED;
        parent->color = Color::BLACK;
        uncle->color = Color::BLACK;
        node = grandparent;
      } else {
        if (node == parent->right) {
          rotateLeft(tree, parent);
          node = parent;
          parent = node->parent;
        }

        rotateRight(tree, grandparent);
        std::swap(parent->color, grandparent->color);
        node = parent;
      }
    } else {
      Node* uncle = grandparent->left;
      if (uncle && uncle->color == Color::RED) {
        grandparent->color = Color::RED;
        parent->color = Color::BLACK;
        uncle->color = Color::BLACK;
        node = grandparent;
      } else {
        if (node == parent->left) {
          rotateRight(tree, parent);
          node = parent;
          parent = node->parent;
        }

        rotateLeft(tree, grandparent);
        std::swap(grandparent->color, parent->color);
        node = parent;
      }
    }

    countHeight(tree, node);
    Node* el = node->parent;
    int height = node->height;
    
    while (el) {
      if (el->color == Color::BLACK) {
        ++height;
      }
      el->height = height;
      el = el->parent;
    }
  }
  tree->root->color = Color::BLACK;
}

void fixDelete(RBTree* tree, Node* &node) {
  while (node != tree->root && node->color == Color::BLACK) {
    if (node == node->parent->left) {
      Node* sibling = node->parent->right;
      if (sibling->color == Color::RED) {
        sibling->color = Color::BLACK;
        node->parent->color = Color::RED;
        rotateLeft(tree, node->parent);
        sibling = node->parent->right;
      }

      if ((!sibling->left || sibling->left->color == Color::BLACK)
        && (!sibling->right || sibling->right->color == Color::BLACK)) {
        sibling->color = Color::RED;
        node = node->parent;
      } else {
        if (!sibling->right || sibling->right->color == Color::BLACK) {
          if (sibling->left) {
            sibling->left->color = Color::BLACK;
          }
                    sibling->color = Color::RED;
          rotateRight(tree, sibling);
          sibling = node->parent->right;
        }

        sibling->color = node->parent->color;
        node->parent->color = Color::BLACK;
        if (sibling->right) {
          sibling->right->color = Color::BLACK;
        }

        rotateLeft(tree, node->parent);
        node = tree->root;
      }
    } else {
      Node* sibling = node->parent->left;
      if (sibling->color == Color::RED) {
        sibling->color = Color::BLACK;
        node->parent->color = Color::RED;
        rotateRight(tree, node->parent);
        sibling = node->parent->left;
      }
      if ((!sibling->left || sibling->left->color == Color::BLACK)
        && (!sibling->right || sibling->right->color == Color::BLACK)) {
        sibling->color = Color::RED;
        node = node->parent;
      } else {
        if (!sibling->left || sibling->left->color == Color::BLACK) {
          if (sibling->right) {
            sibling->right->color = Color::BLACK;
          }

          sibling->color = Color::RED;
          rotateLeft(tree, sibling);
          sibling = node->parent->left;
        }

        sibling->color = node->parent->color;
        node->parent->color = Color::BLACK;
        if (sibling->left) {
          sibling->left->color = Color::BLACK;
        }

        rotateRight(tree, node->parent);
        node = tree->root;
      }
    }
  }
  node->color = Color::BLACK;
}

void transplant(RBTree* tree, Node* &u, Node* &v) {
  if (!u->parent) {
    tree->root = v;
  }
  else if (u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }

  if (v) {
    v->parent = u->parent;
  }
}

bool RBTree::empty() const {
  return root == nullptr;
}

Node* minValueNode(Node* &node) {
  Node* current = node;
  while (current->left) {
    current = current->left;
  }

  return current;
}

void RBTree::erase(int key) {
  Node* node = root;
  Node* x = nullptr;
  Node* y = nullptr;
  Node* z = nullptr;
  
  while (node) {
    if (node->key == key) {
      z = node;
    }
    if (node->key <= key) {
      node = node->right;
    } else {
      node = node->left;
    }
  }

  if (!z) {
    return;
  }

  y = z;
  Color yOriginalColor = y->color;
  if (!z->left) {
    x = z->right;
    transplant(this, z, z->right);
  } else if (!z->right) {
    x = z->left;
    transplant(this, z, z->left);
  } else {
    y = minValueNode(z->right);
    yOriginalColor = y->color;
    x = y->right;
    if (y->parent == z) {
      if (x)
        x->parent = y;
    }
    else {
      transplant(this, y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(this, z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  delete z;
  if (yOriginalColor == Color::BLACK) {
    fixDelete(this, x);
  }

  countHeight(this, root);
}

int RBTree::height() const {
  return root ? root->height : 0;
}

void deleteTree(Node* node) {
  if (node) {
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
  }
}

RBTree::RBTree() : root(nullptr) {}

RBTree::RBTree(std::initializer_list<int> list) {
  for (auto i : list) {
    insert(i);
  }
}

RBTree::~RBTree() {
  deleteTree(root);
}

void RBTree::insert(int key) {
  Node* node = new Node(key);
  Node* parent = nullptr;
  Node* current = root;
  
  while (current) {
    parent = current;
    if (node->key == current->key) {
      return;
    }
    if (node->key < current->key) {
      current = current->left;
    } else {
      current = current->right;
    }
  }

  node->parent = parent;
  if (!parent) {
    root = node;
  } else if (node->key < parent->key) {
    parent->left = node;
  } else {
    parent->right = node;
  }
  
  while (parent) {
    ++parent->size;
    parent = parent->parent;
  }

  fixInsert(this, node);
}

int* findNode(Node* node, int key) {
  if (!node) {
    return nullptr;
  }
  if (key > node->key) {
    return findNode(node->right, key);
  }
  if (key < node->key) {
    return findNode(node->left, key);
  }
  return &node->key;
}

int* RBTree::find(int key) {
  return findNode(root, key);
}

int RBTree::size() const {
  return root ? root->size : 0;
}

int* findLowerBound(Node* node, int key) {
  if (!node) {
    return nullptr;
  }
  if (key > node->key) {
    return findLowerBound(node->right, key);
  }
  if (key < node->key) {
    int* lower = findLowerBound(node->left, key);
    if (!lower) {
      lower = &node->key;
    }
    return lower;
  }
  return &node->key;
}

int* RBTree::lowerBound(int key) {
  return findLowerBound(root, key);
}