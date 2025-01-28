#include <iostream>
#include "HashLPTable.h"


int main() {
    std::vector numbers = {1, 2, 3, 10, 23, 4, 6, 13, 8, 0, 4};

    HashLPTable hashTable = HashLPTable(8);
    hashTable.print();
    std::cout << "\n";
    for (int i = 0; i < 10; ++i) {
        std::cout << "Вставляем " << numbers[i] << "\n";
        hashTable.insert(numbers[i]);
        hashTable.print();
        std::cout << "\n";
    }

    hashTable.remove(2);
    hashTable.print();
    return 0;
}

