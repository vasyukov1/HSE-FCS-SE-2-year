#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <vector>

class SkipList {
    struct Node {
        int value;
        std::vector<Node*> next;

        Node(int value, int level) : value(value), next(level, nullptr) {}
    };

    int level;
    int max_level = 5;
    Node* head;
    int experiment() const;

public:
    explicit SkipList();
    ~SkipList();
    void insert(int value);
    void remove(int value);
    bool search(int value);
    void print();
};


#endif //SKIPLIST_H
