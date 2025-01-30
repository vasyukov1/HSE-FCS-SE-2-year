#include "HashLPTable.h"
#include <iostream>

HashLPTable::HashLPTable(size_t capacity) : capacity(capacity), size(0){
    table.resize(capacity);
}

size_t HashLPTable::hash(int value) const {
    return value % capacity;
}

void HashLPTable::rehash() {
    capacity *= 2;
    std::vector<Cell> new_table(capacity);

    for (int i = 0; i < capacity / 2; ++i) {
        if (table[i].isOccupied) {
            const int value = table[i].value;
            const size_t key = hash(value);

            int j = 0;
            while (j < capacity) {
                if (const size_t current_key = (key + j) % capacity;
                    !new_table[current_key].isOccupied) {
                    new_table[current_key].value = value;
                    new_table[current_key].isOccupied = true;
                    break;
                }
                ++j;
            }
        }
    }

    table = std::move(new_table);
}

bool HashLPTable::insert(int value) {
    if (size >= capacity * 3/4) {
        rehash();
    }

    size_t key = hash(value);
    while (table[key].isOccupied) {
        if (table[key].value == value) {
            return false;
        }
        ++key;
        key %= capacity;
    }

    table[key] = Cell(value, true);
    ++size;
    return true;
}

void HashLPTable::remove(int value) {
    size_t key = search(value);
    if (key == -1) {
       return;
    }
    size_t firstKey = key;
    size_t nextKey = (key + 1) % capacity;
    while (table[nextKey].isOccupied) {
        if (key < nextKey && hash(table[nextKey].value) < nextKey ||
            key > nextKey && hash(table[nextKey].value) > nextKey) {
            table[key] = table[nextKey];
            key = nextKey;
        }
        nextKey = (nextKey + 1) % capacity;
        if (nextKey == firstKey) {
            break;
        }
    }
    table[key].isOccupied = false;
    --size;
}

size_t HashLPTable::search(int value) const {
    size_t key = hash(value);
    if (!table[key].isOccupied) {
        return -1;
    }
    size_t firstKey = key;
    while (table[key].isOccupied) {
        if (table[key].value == value) {
            return key;
        }
        ++key;
        key %= capacity;
        if (key == firstKey) {
            return -1;
        }
    }
    return -1;
}

void HashLPTable::print() const {
    std::cout << "[\t";
    std::vector<std::string> values(capacity);
    for (int i = 0; i < capacity; ++i) {
        std::cout << i << "\t";
        if (table[i].isOccupied) {
            values[i] = std::to_string((table[i].value));
        } else {
            values[i] = "-";
        }
    }
    std::cout << "]\n[\t";
    for (int i = 0; i < capacity; ++i) {
        std::cout << values[i] << "\t";
    }
    std::cout << "]\n";
}

