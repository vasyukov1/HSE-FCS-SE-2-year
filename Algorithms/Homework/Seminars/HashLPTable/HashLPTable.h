#ifndef HASHLPTABLE_H
#define HASHLPTABLE_H

#include <vector>

class HashLPTable {

    struct Cell {
        int value;
        bool isOccupied;
    };

    std::vector<Cell> table;
    size_t capacity;
    size_t size;

    size_t hash(int value) const;
    void rehash();

public:
    explicit HashLPTable(size_t capacity);
    bool insert(int value);

    void remove(int value);

    size_t search(int value) const;

    void print() const;
};

#endif //HASHLPTABLE_H
