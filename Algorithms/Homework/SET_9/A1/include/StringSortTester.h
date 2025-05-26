#ifndef STRINGSORTTESTER_H
#define STRINGSORTTESTER_H

#include "json.hpp"
#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <chrono>

using nlohmann::json;

struct StringData {
    std::vector<std::string> random;
    std::vector<std::string> reversed;
    std::vector<std::string> almost_sorted;
};

class StringSortTester {
public:
    explicit StringSortTester(const std::string& inputPath) {
        std::ifstream file(inputPath);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + inputPath);
        }

        json j;
        file >> j;

        data.random = j.at("random").get<std::vector<std::string>>();
        data.reversed = j.at("reversed").get<std::vector<std::string>>();
        data.almost_sorted = j.at("almost_sorted").get<std::vector<std::string>>();
    }

    template<typename SortFunc>
    void testAlgorithm(const std::string& name, SortFunc sortFn) {
        for (int size = 100; size <= 3000; size += 100) {
            analyzeOnData(size, sortFn, name);
        }
    }

    void saveResult(const std::string& outputPath) const {
        std::ofstream out(outputPath);
        if (!out) {
            throw std::runtime_error("Failed to open output file: " + outputPath);
        }
        out << resultJson.dump(2);
    }
private:
    StringData data;
    json resultJson;

    template<typename SortFunc>
    static std::pair<int, int> analyze(std::vector<std::string>& strings, SortFunc sortFn) {
        extern int counter;
        long long time_sum = 0;
        int counter_sum = 0;

        for (int i = 0; i < 10; ++i) {
            counter = 0;
            std::vector<std::string> copy = strings;

            auto start = std::chrono::high_resolution_clock::now();
            sortFn(copy, 0, static_cast<int>(copy.size()) - 1);
            auto end = std::chrono::high_resolution_clock::now();

            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            time_sum += duration.count();
            counter_sum += counter;
        }

        return { counter_sum / 10, static_cast<int>(time_sum / 10) };
    }

    template<typename SortFunc>
    void analyzeOnData(int count, SortFunc sortFn, const std::string& name) {
        {
            std::vector v(data.random.begin(), data.random.begin() + count);
            auto [comp, time] = analyze(v, sortFn);
            resultJson[name]["random"] += { {"size", count}, {"comparisons", comp}, {"time", time} };
        }
        {
            std::vector v(data.reversed.begin(), data.reversed.begin() + count);
            auto [comp, time] = analyze(v, sortFn);
            resultJson[name]["reversed"] += { {"size", count}, {"comparisons", comp}, {"time", time} };
        }
        {
            std::vector v(data.almost_sorted.begin(), data.almost_sorted.begin() + count);
            auto [comp, time] = analyze(v, sortFn);
            resultJson[name]["almost_sorted"] += { {"size", count}, {"comparisons", comp}, {"time", time} };
        }
    }
};

#endif //STRINGSORTTESTER_H
