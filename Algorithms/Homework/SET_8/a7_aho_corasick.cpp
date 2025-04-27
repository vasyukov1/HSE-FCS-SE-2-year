#include <iostream>
#include <vector>
#include <queue>
#include <array>
#include <algorithm>

struct Node {
    std::array<int, 26> next;
    int suffix_link;
    int output_link;
    std::vector<int> patterns;
};

std::vector<Node> nodes;
std::vector<int> pattern_lengths;

void build_automaton(const std::vector<std::string>& patterns) {
    nodes.emplace_back();
    Node& root = nodes[0];
    std::ranges::fill(root.next.begin(), root.next.end(), -1);
    root.suffix_link = 0;
    root.output_link = -1;

    for (int i = 0; i < patterns.size(); ++i) {
        const std::string& s = patterns[i];
        int current = 0;
        for (const char c : s) {
            const int index = c - 'a';
            if (nodes[current].next[index] == -1) {
                nodes.emplace_back();
                Node& new_node = nodes.back();
                std::ranges::fill(new_node.next.begin(), new_node.next.end(), -1);
                new_node.suffix_link = -1;
                new_node.output_link = -1;
                nodes[current].next[index] = nodes.size() - 1;
            }
            current = nodes[current].next[index];
        }
        nodes[current].patterns.push_back(i);
    }

    pattern_lengths.reserve(patterns.size());
    for (const std::string& s : patterns) {
        pattern_lengths.push_back(s.size());
    }

    std::queue<int> q;
    for (int i = 0; i < 26; ++i) {
        if (nodes[0].next[i] != -1) {
            int child = nodes[0].next[i];
            nodes[child].suffix_link = 0;
            q.push(child);
        }
    }

    while (!q.empty()) {
        const int u = q.front();
        q.pop();

        for (int c = 0; c < 26; ++c) {
            if (nodes[u].next[c] == -1) {
                continue;
            }
            int child = nodes[u].next[c];
            int temp = nodes[u].suffix_link;

            while (temp != 0 && nodes[temp].next[c] == -1) {
                temp = nodes[temp].suffix_link;
            }

            nodes[child].suffix_link = (nodes[temp].next[c] != -1) ? nodes[temp].next[c] : 0;

            if (!nodes[nodes[child].suffix_link].patterns.empty()) {
                nodes[child].output_link = nodes[child].suffix_link;
            } else {
                nodes[child].output_link = nodes[nodes[child].suffix_link].output_link;
            }

            q.push(child);
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string s;
    std::cin >> s;

    int n;
    std::cin >> n;

    std::vector<std::string> patterns(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> patterns[i];
    }

    build_automaton(patterns);

    std::vector<std::vector<int>> occurrences(n);

    int current_node = 0;
    for (int i = 0; i < s.size(); ++i) {
        const char c = s[i] - 'a';
        while (current_node != 0 && nodes[current_node].next[c] == -1) {
            current_node = nodes[current_node].suffix_link;
        }
        if (nodes[current_node].next[c] != -1) {
            current_node = nodes[current_node].next[c];
        } else {
            current_node = 0;
        }

        int temp = current_node;
        while (temp != -1) {
            for (const int idx : nodes[temp].patterns) {
                int pos = i - pattern_lengths[idx] + 2;
                occurrences[idx].push_back(pos);
            }
            temp = nodes[temp].output_link;
        }
    }

    for (int i = 0; i < n; ++i) {
        std::ranges::sort(occurrences[i].begin(), occurrences[i].end());
        std::cout << occurrences[i].size();
        for (const int pos : occurrences[i]) {
            std::cout << " " << pos;
        }
        std::cout << "\n";
    }

    return 0;
}