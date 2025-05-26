#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
using std::cin;
using std::cout;
using std::string;
using std::vector;

void LZW_decoding(const vector<int>& input) {
    std::unordered_map<int, string> chunks;

    for (int i = 0; i < 128; ++i) {
        chunks[i] = string(1, static_cast<char>(i));
    }

    int next_code = 128;
    string prev_chunk = chunks[input[0]];
    string result = prev_chunk;

    for (int i = 1; i < input.size(); ++i) {
        int code = input[i];
        string chunk;

        if (chunks.contains(code)) {
            chunk = chunks[code];
        } else {
            chunk = prev_chunk + prev_chunk[0];
        }

        result += chunk;

        chunks[next_code++] = prev_chunk + chunk[0];
        prev_chunk = chunk;
    }

    cout << result;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int k, el;
    cin >> k;
    vector<int> input(k);
    for (int i = 0; i < k; ++i) {
        cin >> el;
        input[i] = el;
    }

    LZW_decoding(input);
    return 0;
}
