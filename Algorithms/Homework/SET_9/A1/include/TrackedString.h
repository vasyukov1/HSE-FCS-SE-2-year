#ifndef TRACKEDSTRING_H
#define TRACKEDSTRING_H

#include <string>

struct TrackedString {
    std::string value;

    TrackedString() = default;
    explicit TrackedString(std::string  str) : value(std::move(str)) {}

    bool operator<(const TrackedString& other) const {
        size_t i = 0;
        while (i < value.size() && i < other.value.size()) {
            ++counter;
            if (value[i] != other.value[i])
                return value[i] < other.value[i];
            ++i;
        }
        ++counter;
        return value.size() < other.value.size();
    }

    bool operator==(const TrackedString& other) const {
        return value == other.value;
    }
};

#endif //TRACKEDSTRING_H
