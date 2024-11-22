#include <stdexcept>
#include <unordered_set>
#include "list.h"

List::List() : _size(0), head(nullptr), tail(nullptr) { }

List::List(const List& other) : _size(0), head(nullptr), tail(nullptr) {
    copy(other);
}

List::List(std::vector<int> array) : _size(0), head(nullptr), tail(nullptr) {
    for (int value : array) {
        push_back(value);
    }
}

List::~List() {
    clear();
}

int List::front() {
    if (empty()) {
        throw std::runtime_error("List is empty");
    }
    return head->value;
}


int List::back() {
    if (empty()) {
        throw std::runtime_error("List is empty");
    }
    return tail->value;
}

void List::push_back(int value) {
    Node* newNode = new Node(value);
    newNode->next = nullptr;
    newNode->prev = tail;
    if (tail) {
        tail->next = newNode;
    } else {
        head = newNode;
    }
    tail = newNode;
    ++_size;
}

void List::push_front(int value) {
    Node* newNode = new Node(value);
    newNode->next = head;
    newNode->prev = nullptr;
    if (head) {
        head->prev = newNode;
    } else {
        tail = newNode;
    }
    head = newNode;
    ++_size;
}

void List::insert(Node* pos, int value) {
    if (!pos) {
        throw std::runtime_error("Incorrect position!");
    }
    Node* node = new Node(value);
    node->next = pos->next;
    node->prev = pos;
    if (pos->next) {
        pos->next->prev = node;
    }
    pos->next = node;
    if (pos == tail) {
        tail = node;
    }
    ++_size;
}

void List::pop_front() {
    if (empty()) {
        throw std::runtime_error("Deleting in empty list");
    }
    Node* node = head;
    head = head->next;
    if (head) {
        head->prev = nullptr;
    } else {
        tail = nullptr;
    }
    delete node;
    --_size;
}

void List::pop_back() {
    if (empty()) {
        throw std::runtime_error("Deleting in empty list");
    }
    Node* node = tail;
    tail = tail->prev;
    if (tail) {
        tail->next = nullptr;
    } else {
        head = nullptr;
    }
    delete node;
    --_size;
}

void List::erase(Node* pos) {
    if (!pos) {
        throw std::runtime_error("Incorrect position!");
    }
    if (pos == head) {
        pop_front();
    } else if (pos == tail) {
        pop_back();
    } else {
        pos->prev->next = pos->next;
        pos->next->prev = pos->prev;
        delete pos;
        --_size;
    }
}

void List::clear() {
    while (!empty()) {
        pop_back();
    }
    head = nullptr;
    tail = nullptr;
}

void List::reverse() {
    if (empty()) {
        return;
    }
    Node* curr = head;
    while (curr) {
        std::swap(curr->prev, curr->next);
        curr = curr->prev;
    }
    std::swap(head, tail);
}

void List::remove_duplicates() {
    std::unordered_set<int> elements;
    Node* curr = head;
    while (curr) {
        if (elements.find(curr->value) != elements.end()) {
            Node* bad = curr;
            curr = curr->next;
            erase(bad);
        } else {
            elements.insert(curr->value);
            curr = curr->next;
        }
    }
}

void List::replace(int old_value, int new_value) {
    if (empty()) {
        return;
    }
    Node* curr = head;
    while (curr) {
        if (curr->value == old_value) {
            curr->value = new_value;
        }
        curr = curr->next;
    }
}

void List::merge(const List& other) {
    Node* node = other.head;
    while (node) {
        push_back(node->value);
        node = node->next;
    }
}

bool List::check_cycle() const {
    if (empty()) {
        return false;
    }
    Node* tortoise = head;
    Node* hare = head;
    while (hare && hare->next) {
        tortoise = tortoise->next;
        hare = hare->next->next;
        if (tortoise == hare) {
            return true;
        }
    }
    return false;
}

size_t List::size() const {
    return _size;
}

bool List::empty() const{
    return _size == 0;
}

void List::copy(const List& other) {
    clear();
    Node* node = other.head;
    while (node) {
        push_back(node->value);
        node = node->next;
    }
}
