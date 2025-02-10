#include <iostream>
#include <vector>

template <class KeyType, class ValueType>
struct Node {
    KeyType key;
    ValueType value;
    Node *next;

    Node(KeyType key, ValueType value) : key(key), value(value), next(nullptr) {};
};

template <class KeyType, class ValueType, class Func = std::hash<KeyType>>
class HashTable {
    size_t _capacity;
    double _load_factor;
    size_t _size;
    Func _hasher;
    std::vector<Node<KeyType, ValueType>*> _table;

    void _rehash() {
        size_t new_capacity = _capacity * 2;
        std::vector<Node<KeyType, ValueType>*> new_table(new_capacity, nullptr);

        for (size_t i = 0; i < _capacity; ++i) {
            Node<KeyType, ValueType>* cur = _table[i];
            while (cur) {
                Node<KeyType, ValueType>* next = cur->next;
                size_t index = _hasher(cur->key) % new_capacity;

                if (!new_table[index]) {
                    new_table[index] = cur;
                    cur->next = nullptr;
                } else {
                    Node<KeyType, ValueType>* tail = new_table[index];
                    while (tail->next) {
                        tail = tail->next;
                    }
                    tail->next = cur;
                    cur->next = nullptr;
                }

                cur = next;
            }
        }

        _table = std::move(new_table);
        _capacity = new_capacity;
    }

public:
    HashTable() : _capacity(100), _load_factor(0.5), _size(0), _hasher(Func()) {
        _table.resize(_capacity, nullptr);
    }

    HashTable(Func hasher) : _capacity(100), _load_factor(0.5), _size(0), _hasher(hasher) {
        _table.resize(_capacity, nullptr);
    }

    HashTable(size_t capacity, double load_factor, Func hasher = std::hash<KeyType>())
        : _capacity(capacity),  _size(0), _hasher(hasher) {
        if (0 < load_factor && load_factor <= 1) {
            _load_factor = load_factor;
        } else {
            _load_factor = 0.5;
        }
        _table.resize(_capacity, nullptr);
    }

    ~HashTable() {
        for (size_t i = 0; i < _capacity; ++i) {
            Node<KeyType, ValueType>* cur = _table[i];
            while (cur) {
                Node<KeyType, ValueType>* next = cur->next;
                delete cur;
                cur = next;
            }
        }
    }

    void insert(KeyType key, ValueType value) {
        if (_size + 1 > _capacity * _load_factor) {
            _rehash();
        }

        size_t index = _hasher(key) % _capacity;
        Node<KeyType, ValueType>* cur = _table[index];
        Node<KeyType, ValueType>* prev = nullptr;

        while (cur) {
            if (cur->key == key) {
                cur->value = value;
                return;
            }
            prev = cur;
            cur = cur->next;
        }

        Node<KeyType, ValueType>* new_node = new Node<KeyType, ValueType>(key, value);
        ++_size;
        if (prev) {
            prev->next = new_node;
        } else {
            _table[index] = new_node;
        }
    }

    ValueType* find(KeyType key) {
        size_t index = _hasher(key) % _capacity;
        Node<KeyType, ValueType>* cur = _table[index];

        while (cur) {
            if (cur->key == key) {
                return &cur->value;
            }
            cur = cur->next;
        }

        return nullptr;
    }

    void erase(KeyType key) {
        size_t index = _hasher(key) % _capacity;
        Node<KeyType, ValueType>* cur = _table[index];
        Node<KeyType, ValueType>* prev = nullptr;

        while (cur) {
            if (cur->key == key) {
                if (prev) {
                    prev->next = cur->next;
                } else {
                    _table[index] = cur->next;
                }
                delete cur;
                --_size;
                return;
            }
            prev = cur;
            cur = cur->next;
        }
    }

    Node<KeyType, ValueType> &operator[](uint64_t index) {
        if (index >= _capacity) {
            throw std::out_of_range("Index out of range");
        }
        if (!_table[index]) {
            throw std::runtime_error("Cell is empty");
        }
        return *_table[index];
    }

    Node<KeyType, ValueType> at(uint64_t index) {
        if (index >= _capacity) {
            throw std::out_of_range("Index out of range");
        }
        if (!_table[index]) {
            throw std::runtime_error("Cell is empty");
        }
        return *_table[index];
    }

    int size() const {
        return _size;
    }

    int capacity() const {
        return _capacity;
    }
};
