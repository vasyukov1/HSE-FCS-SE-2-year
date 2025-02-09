#include "SkipList.h"
#include <iostream>
#include <ctime>

int main() {
    srand(time(0));
    SkipList skiplist = SkipList();

    skiplist.insert(3);
    skiplist.insert(6);
    skiplist.insert(7);
    skiplist.insert(9);
    skiplist.insert(12);
    skiplist.insert(19);
    skiplist.insert(17);
    skiplist.insert(26);
    skiplist.insert(21);
    skiplist.insert(25);

    skiplist.print();

    std::cout << "Search 6: " << (skiplist.search(6) ? "Found" : "Not Found") << "\n";
    std::cout << "Search 8: " << (skiplist.search(15) ? "Found" : "Not Found") << "\n";

    skiplist.remove(6);
    skiplist.print();

    return 0;
}

