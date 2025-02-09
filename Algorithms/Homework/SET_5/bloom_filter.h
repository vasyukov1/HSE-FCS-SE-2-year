#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H
#include <iostream>
#include <unordered_set>

class BloomFilter {
public:
    BloomFilter(size_t, size_t);

    void add(const std::string&);

    bool verify(const std::string&);

    double getFPRate() const;

    size_t numberOfHashFunctions() const ;

    size_t numberOfBits() const;

private:
    inline size_t _get_hash(int, const std::string&) const;

    size_t _hashes;
    size_t _bits;

    int cnt = 0, false_positive_cnt = 0;

    std::hash<std::string> hasher{};
    std::vector<bool> _filter;
    std::unordered_set<std::string> _raw_set;
};


#endif //BLOOM_FILTER_H
