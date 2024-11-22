#ifndef LIST_H
#define LIST_H

#include <vector>

struct Node {
public:
    Node* prev;
    Node* next;

    int value;

    Node() {
        next = prev = nullptr;
        value = 0;
    }

    Node(const int& value): value(value) { next = prev = nullptr; }

    Node(Node* prev, Node* next, const int& value) : prev(prev), next(next), value(value) { }

    Node(const Node& other) {
        value = other.value;

        next = prev = nullptr;
    }
};

class List {
public:
    List();

    List(const List& other);

    explicit List(std::vector<int> array);

    ~List();

    int front();

    int back();

    void push_back(int value);

    void push_front(int value);

    void insert(Node* pos, int value);

    void pop_front();

    void pop_back();

    void erase(Node* pos);

    void clear();

    void reverse();

    void remove_duplicates();

    void replace(int old_value, int new_value);

    void copy(const List& other);

    void merge(const List& other);

    [[nodiscard]] bool check_cycle() const;

    [[nodiscard]] size_t size() const;

    [[nodiscard]] bool empty() const;

    size_t _size;

    Node* head;
    Node* tail;
};

#endif
