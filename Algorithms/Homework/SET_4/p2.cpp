#include <iostream>
#include <unordered_map>

struct Node {
    int key_;
    int left_;
    int right_;
    char color_;
};

class RBTree {
private:
    int root_;
    int size_;
    std::unordered_map<int, Node> nodes_;

    bool checkBlackHeight(int number) {
        if (number == -1) {
            return true;
        }

        Node& node = nodes_[number];
        if (node.left_ != -1 && nodes_[node.left_].key_ >= node.key_) {
            return false;
        }
        if (node.right_ != -1 && nodes_[node.right_].key_ <= node.key_) {
            return false;
        }

        return checkBlackHeight(node.left_) && checkBlackHeight(node.right_);
    }

    bool dfs(int number, int& black_height) {
        if (number == -1) {
            black_height = 1;
            return true;
        }

        Node& node = nodes_[number];
        if (node.color_ == 'R') {
            if (node.left_ != -1 && nodes_[node.left_].color_ == 'R' ||
                node.right_ != -1 && nodes_[node.right_].color_ == 'R') {
                return false;
            }
        }

        int left_height = 0;
        int right_height = 0;
        if (!(dfs(node.left_, left_height) && dfs(node.right_, right_height))) {
            return false;
        }

        if (left_height != right_height) {
            return false;
        }

        black_height = left_height + (node.color_ == 'B' ? 1 : 0);
        return true;
    }

public:
    RBTree(int n, int root) : size_(n), root_(root) {};

    void insert(int number, Node node) {
        nodes_[number] = node;
    }

    bool check() {
        if (size_ == 0) {
            return true;
        }
        if (nodes_[root_].color_ == 'R') {
            return false;
        }
        int black_height = 0;
        return dfs(root_, black_height) && checkBlackHeight(root_);
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;

    if (n == 0) {
        std::cout << "NO" << '\n';
        return 0;
    }

    int root;
    std::cin >> root;

    RBTree rb_tree(n, root);

    int number;
    int key;
    std::string left_str;
    std::string right_str;
    int left;
    int right;
    char color;
    for (int i = 0; i < n; ++i) {
        std::cin >> number >> key >> left_str >> right_str >> color;

        left = (left_str == "null" ? -1 : std::stoi(left_str));
        right = (right_str == "null" ? -1 : std::stoi(right_str));

        Node node = {key, left, right, color};
        rb_tree.insert(number, node);
    }

    if (rb_tree.check()) {
        std::cout << "YES" << '\n';
    } else {
        std::cout << "NO" << '\n';
    }
    return 0;
}
