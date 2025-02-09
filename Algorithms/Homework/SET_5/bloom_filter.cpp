#include "bloom_filter.h"
#include <iostream>
#include <unordered_set>
#include <string>

BloomFilter::BloomFilter(size_t hashes, size_t bits) : _hashes(hashes), _bits(bits), _filter(bits, false) {}

void BloomFilter::add(const std::string& value) {
    _raw_set.insert(value);
    for (int i = 0; i < _hashes; ++i) {
        size_t hash = _get_hash(i, value);
        _filter[hash % _bits] = true;
    }
}

bool BloomFilter::verify(const std::string& value) {
    ++cnt;
    for (int i = 0;  i < _hashes; ++i) {
        size_t hash = _get_hash(i, value);
        if (!_filter[hash % _bits]) {
            return false;
        }
    }
    if (!_raw_set.contains(value)) {
        ++false_positive_cnt;
    }
    return true;
}

double BloomFilter::getFPRate() const {
    if (cnt == 0) {
        return 0;
    }
    return static_cast<double>(false_positive_cnt) / cnt;
}


size_t BloomFilter::numberOfHashFunctions() const {
    return _hashes;
}


size_t BloomFilter::numberOfBits() const {
    return _bits;
}


inline size_t BloomFilter::_get_hash(int k, const std::string& value) const {
    std::string value_with_salt = value + std::to_string(k);
    return hasher(value_with_salt);
}