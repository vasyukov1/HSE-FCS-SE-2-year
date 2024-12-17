#include <iostream>
#include <cmath>

struct Node {
    int value_;
    Node* left_;
    Node* right_;

    Node(int value) : value_(value), left_(nullptr), right_(nullptr) {};
};

class BinaryTree {
private:
    Node* root;

    int checkBalance(Node* node) {
        if (!node) {
            return 0;
        }

        int left_height = checkBalance(node->left_);
        if (left_height == -1) {
            return -1;
        }

        int right_height = checkBalance(node->right_);
        if (right_height == -1) {
            return -1;
        }

        if (std::abs(left_height - right_height) > 1) {
            return -1;
        }

        return std::max(left_height, right_height) + 1;
    }

    Node* insertNode(Node* node, int value) {
        if (!node) {
            return new Node(value);
        }

        if (value < node->value_) {
            node->left_ = insertNode(node->left_, value);
        } else if (value > node->value_) {
            node->right_ = insertNode(node->right_, value);
        }

        return node;
    }

public:
    BinaryTree() : root(nullptr) {};

    bool isAVL() {
        return checkBalance(root) != -1;
    }

    void insert(int value) {
        root = insertNode(root, value);
    }
};

int main() {
    BinaryTree bt;
    int value;

    while (std::cin >> value && value != 0) {
        bt.insert(value);
    }

    if (bt.isAVL()) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}
