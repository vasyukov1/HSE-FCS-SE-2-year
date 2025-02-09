#include "SkipList.h"
#include <iostream>
#include <random>
#include <cstdlib>

SkipList::SkipList() : level(1){
    int neg_inf_int = std::numeric_limits<int>::min();
    head = new Node(neg_inf_int, max_level);
}

SkipList::~SkipList() {
    Node* cur = head->next[0];
    while (cur != nullptr) {
        Node* temp = cur;
        cur = cur->next[0];
        delete temp;
    }
    delete head;
}

int SkipList::experiment() const {
    int new_level = 1;
    while (rand() % 2 && new_level < max_level) {
        ++new_level;
    }
    return new_level;
}

void SkipList::insert(int value) {
    std::vector<Node*> update(max_level, nullptr);
    Node* cur = head;

    for (int i = level - 1; i >= 0; --i) {
        while (cur->next[i] != nullptr && cur->next[i]->value < value) {
            cur = cur->next[i];
        }
        update[i] = cur;
    }

    cur = cur->next[0];

    if (cur != nullptr && cur->value == value) {
        return;
    }

    int new_level = experiment();
    Node* new_node = new Node(value, new_level);

    if (new_level > level) {
        for (int i = level; i < new_level; ++i) {
            update[i] = head;
        }
        level = new_level;
    }

    for (int i = 0; i < new_level; ++i) {
        new_node->next[i] = update[i]->next[i];
        update[i]->next[i] = new_node;
    }
}

void SkipList::remove(int value) {
    std::vector<Node*> update(max_level, nullptr);
    Node* cur = head;

    for (int i = level; i >= 0; --i) {
        while (cur->next[i] != nullptr && cur->next[i]->value < value) {
            cur = cur->next[i];
        }
        update[i] = cur;
    }

    cur = cur->next[0];
    if (cur == nullptr || cur->value != value) {
        return;
    }

    for (int i = 0; i <= level; ++i) {
        if (update[i]->next[i] != cur) {
            break;
        }
        update[i]->next[i] = cur->next[i];
    }

    delete cur;

    while (level > 1 && head->next[level - 1] == nullptr) {
        --level;
    }
}

bool SkipList::search(int value) {
    Node* cur = head;
    for (int i = level - 1; i >= 0; --i) {
        while (cur->next[i] && cur->next[i]->value < value) {
            cur = cur->next[i];
        }
    }

    cur = cur->next[0];
    return cur != nullptr && cur->value == value;
}

void SkipList::print() {
    for (int i = level - 1; i >= 0; i--) {
        Node* current = head->next[i];
        std::cout << "Level " << i << ": ";
        while (current != nullptr) {
            std::cout << current->value << " ";
            current = current->next[i];
        }
        std::cout << std::endl;
    }
}