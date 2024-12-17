#include "avl_tree.h"
#include <algorithm>

Node::Node(int value) {
    this->value = value;
    left = nullptr;
    right = nullptr;
    height = 1;
}

Node::~Node() {
    delete left;
    delete right;
}

AVLTree::AVLTree() {
    root_ = nullptr;
    size_ = 0;
}

AVLTree::AVLTree(int value) {
    root_ = new Node(value);
    size_ = 1;
}

int AVLTree::getHeight() {
    return getNodeHeight(root_);
}

void AVLTree::insert(int value) {
    root_ = insertNode(root_, value);
}

void AVLTree::erase(int value) {
    root_ = removeNode(root_, value);
}

int* AVLTree::find(int value) {
    Node* node = findNode(root_, value);
    return node ? &node->value : nullptr;
}

int* AVLTree::traversal() {
    if (size_ == 0) {
        return nullptr;
    }
    int* result = new int[size_];
    int index = 0;
    traversalInternal(root_, result, &index);
    return result;
}

int* AVLTree::lowerBound(int value) {
    Node* node = lowerBoundInternal(root_, value);
    return node ? &node->value : nullptr;
}

bool AVLTree::empty() {
    return size_ == 0;
}

Node *AVLTree::getRoot() {
    return root_;
}

int AVLTree::getSize() {
    return size_;
}

AVLTree::~AVLTree() {
    delete root_;
}

int AVLTree::getNodeHeight(Node* node) {
    return node ? node->height : 0;
}

int AVLTree::balanceFactor(Node* node) {
    return node ? getNodeHeight(node->left) - getNodeHeight(node->right) : 0;
}

void AVLTree::balanceHeight(Node* node) {
    if (node) {
        node->height = std::max(getNodeHeight(node->left), getNodeHeight(node->right)) + 1;
    }
}

Node *AVLTree::rotateRight(Node *node) {
    Node* new_node = node->left;
    node->left = new_node->right;
    new_node->right = node;
    balanceHeight(node);
    balanceNode(new_node);
    return new_node;
}

Node *AVLTree::rotateLeft(Node *node) {
    Node* new_node = node->right;
    node->right = new_node->left;
    new_node->left = node;
    balanceHeight(node);
    balanceHeight(new_node);
    return new_node;
}

Node *AVLTree::balanceNode(Node *node) {
    balanceHeight(node);
    int bf = balanceFactor(node);
    if (bf > 1) {
        if (balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }
    if (bf < -1) {
        if (balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    return node;
}

Node *AVLTree::insertNode(Node *node, int value) {
    if (!node) {
        ++size_;
        return new Node(value);
    }
    if (value < node->value) {
        node->left = insertNode(node->left, value);
    } else if (value > node->value) {
        node->right = insertNode(node->right, value);
    }
    return balanceNode(node);
}

Node *AVLTree::findMinNode(Node *node) {
    return node->left ? findMinNode(node->left) : node;
}

Node *AVLTree::removeMinNode(Node *node) {
    if (!node->left) {
        return node->right;
    }
    node->left = removeMinNode(node->left);
    return balanceNode(node);
}

Node *AVLTree::removeNode(Node *node, int value) {
    if (!node) {
        return nullptr;
    }

    if (value < node->value) {
        node->left = removeNode(node->left, value);
    } else if (value > node->value) {
        node->right = removeNode(node->right, value);
    } else {
        Node* left_node = node->left;
        Node* right_node = node->right;
        node->left = nullptr;
        node->right = nullptr;
        delete node;
        --size_;

        if (!right_node) {
            return left_node;
        }

        Node* min_node = findMinNode(right_node);
        min_node->right = removeMinNode(right_node);
        min_node->left = left_node;
        return balanceNode(min_node);
    }
    return balanceNode(node);
}

Node *AVLTree::findNode(Node *node, int value) {
    if (!node || node->value == value) {
        return node;
    }
    return value < node->value ? findNode(node->left, value) : findNode(node->right, value);
}

void AVLTree::traversalInternal(Node *node, int *array, int *index) {
    if (!node) {
        return;
    }
    traversalInternal(node->left, array, index);
    array[(*index)++] = node->value;
    traversalInternal(node->right, array, index);
}

Node *AVLTree::lowerBoundInternal(Node *current, int value) const {
    if (!current) {
        return nullptr;
    }
    if (current->value >= value) {
        Node* left_lower_bound = lowerBoundInternal(current->left, value);
        return left_lower_bound ? left_lower_bound : current;
    }
    return lowerBoundInternal(current->right, value);
}