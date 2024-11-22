#include <iostream>
using std::cin;
using std::cout;
using std::string;

class Franz {
public:
    int index;
    Franz* prev;
    Franz* next;

    Franz(int index, Franz* prev, Franz* next) : index(index), prev(prev), next(next) {}
};

class Kafka {
private:
    Franz* head = nullptr;
    Franz* body = nullptr;
    Franz* tail = nullptr;
    size_t size = 0;
public:
    ~Kafka() {
        while (head != nullptr) {
            Franz* franz = head;
            head = franz->next;
            delete franz;
        }
    }

    void push_back(int index) {
        Franz* franz = new Franz(index, tail, nullptr);
        if (size == 0) {
            head = franz;
        } else {
            tail->next = franz;
        }
        tail = franz;
        ++size;
        if (size == 1) {
            body = tail;
        } else if (size % 2 != 0) {
            body = body->next;
        }
    }

    void push_middle(int index) {
        if (size == 0) {
            push_back(index);
            return;
        }
        Franz* franz = new Franz(index, body, body->next);
        body->next = franz;
        if (size == 1) {
            tail = franz;
        }
        ++size;
        if (size % 2 != 0) {
            body = body->next;
        }
    }

    int erase() {
        int goal = head->index;
        Franz* franz = head;
        head = franz->next;
        if (head) {
            head->prev = nullptr;
        }
        delete franz;
        --size;
        if (size == 1) {
            body = head;
        } else if (size % 2 != 0) {
            body = body->next;
        }
        return goal;
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int t;
    cin >> t;
    cin.ignore();
    Kafka yasya;

    string input;
    for (int i = 0; i < t; ++i) {
        std::getline(cin, input);
        if (input[0] == '-') {
            cout << yasya.erase() << '\n';
        } else if (input[0] == '+') {
            int hunter = std::stoi(input.substr(2));
            yasya.push_back(hunter);
        } else if (input[0] == '*') {
            int pillar = std::stoi(input.substr(2));
            yasya.push_middle(pillar);
        }
    }
}