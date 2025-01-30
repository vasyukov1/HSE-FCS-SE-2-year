#include <iostream>
#include "HashLPTable.h"


int main() {
    const std::vector numbers = {1, 2, 3, 10, 23, 4, 6, 13, 8, 0, 4};

    auto hashTable = HashLPTable(8);
    hashTable.print();
    std::cout << "\n";
    for (int i = 0; i < 11; ++i) {
        std::cout << "Вставили " << numbers[i] << "\n";
        hashTable.insert(numbers[i]);
        hashTable.print();
        std::cout << "\n";
    }

    std::cout << "Удалили 2\n";
    hashTable.remove(2);
    hashTable.print();

    std::cout << "Вставили 22\n";
    hashTable.insert(22);
    hashTable.print();
    return 0;
}

