#include "splay-tree.h"
#include <algorithm>

Node::Node(int key, Node *parent) : key(key), parent(parent), left(nullptr), right(nullptr) {}

SplayTree::SplayTree() : root(nullptr) {}

SplayTree::~SplayTree() {
    clear(root);
}

void SplayTree::insert(int key) {
    Node* node = root;
    Node* parent = nullptr;

    while (node) {
        if (key == node->key) {
            return;
        }
        parent = node;
        node = (key < node->key) ? node->left : node->right;
    }

    Node* new_node = new Node(key, parent);
    if (!parent) {
        root = new_node;
    } else if (key < parent->key) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
}

Node* SplayTree::find(int key) const {
    Node* node = root;
    while (node) {
        if (key == node->key) {
            return node;
        }
        node = (key < node->key) ? node->left : node->right;
    }
    return nullptr;
}

int SplayTree::splay(Node* node) {
    if (!node) {
        return 0;
    }
    int counter = 0;
    while (node->parent) {
        Node* parent = node->parent;
        Node* grandparent = node->parent->parent;

        if (!grandparent) {
            if (node == parent->left) {
                rotateRight(parent);
            } else {
                rotateLeft(parent);
            }
            ++counter;
        } else if (node == parent->left && parent == grandparent->left) {
            rotateRight(grandparent);
            rotateRight(parent);
            counter += 2;
        } else if (node == parent->right && parent == grandparent->right) {
            rotateLeft(grandparent);
            rotateLeft(parent);
            counter += 2;
        } else if (node == parent->left && parent == grandparent->right) {
            rotateRight(parent);
            rotateLeft(grandparent);
            ++counter;
        } else {
            rotateLeft(parent);
            rotateRight(grandparent);
            ++counter;
        }
    }

    root = node;
    return counter;
}

int SplayTree::getHeight() const {
    return getHeight(root);
}

void SplayTree::clear(Node* node) {
    if (node) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

void SplayTree::rotateLeft(Node* node) {
    if (!(node && node->right)) {
        return;
    }

    Node* child = node->right;
    node->right = child->left;
    if (child->left) {
        child->left->parent = node;
    }

    child->parent = node->parent;
    if (!node->parent) {
        root = child;
    } else if (node == node->parent->left) {
        node->parent->left = child;
    } else {
        node->parent->right = child;
    }

    child->left = node;
    node->parent = child;
}

void SplayTree::rotateRight(Node* node) {
    if (!(node && node->left)) {
        return;
    }

    Node* child = node->left;
    node->left = child->right;
    if (child->right) {
        child->right->parent = node;
    }

    child->parent = node->parent;
    if (!node->parent) {
        root = child;
    } else if (node == node->parent->right) {
        node->parent->right = child;
    } else {
        node->parent->left = child;
    }

    child->right = node;
    node->parent = child;
}

int SplayTree::getHeight(Node* node) const {
    if (!node) {
        return -1;
    }
    return std::max(getHeight(node->left), getHeight(node->right)) + 1;
}