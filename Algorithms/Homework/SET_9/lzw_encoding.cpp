#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using std::cin;
using std::cout;
using std::string;
using std::vector;

void LZW_encoding(const string& s) {
    std::unordered_map<string, int> chunks;

    for (int i = 0; i < 128; ++i) {
        chunks[string(1, static_cast<char>(i))] = i;
    }

    int code = 128;
    string chunk;
    vector<int> output;

    for (const char c : s) {
        string chunk_c = chunk + c;
        if (chunks.contains(chunk_c)) {
            chunk = chunk_c;
        } else {
            output.push_back(chunks[chunk]);
            chunks[chunk_c] = code++;
            chunk = c;
        }
    }

    if (!chunk.empty()) {
        output.push_back(chunks[chunk]);
    }

    cout << output.size() << '\n';
    for (const int el : output) {
        cout << el << ' ';
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    string s;
    std::getline(cin, s);
    LZW_encoding(s);
    return 0;
}
