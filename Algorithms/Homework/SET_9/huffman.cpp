#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <unordered_map>
using std::cin;
using std::cout;
using std::string;

struct Node {
    char c;
    int freq;
    Node* left;
    Node* right;
    Node(char c, int freq) : c(c), freq(freq), left(nullptr), right(nullptr) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void deleteTree(Node* node) {
    if (!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void makeCode(Node* node, const std::string& code, std::unordered_map<char, std::string>& codes) {
    if (!node) {
        return;
    }

    if (!node->left && !node->right) {
        codes[node->c] = code;
    }

    makeCode(node->left, code + "0", codes);
    makeCode(node->right, code + "1", codes);
}

std::string encode(const std::string& s, std::unordered_map<char, std::string>& codes) {
    std::string encoded_s;
    for (char c : s) {
        encoded_s += codes[c];
    }
    return encoded_s;
}

void Huffman(const std::string& s) {
    std::map<char, int> letters;
    for (char c : s) {
        ++letters[c];
    }

    if (letters.size() == 1) {
        char c = letters.begin()->first;
        std::string code(s.size(), '0');
        std::cout << "1 " << s.size() << '\n';
        std::cout << c << ": 0\n";
        std::cout << code << '\n';
        return;
    }

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (auto& letter : letters) {
        pq.push(new Node(letter.first, letter.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); pq.pop();
        Node *right = pq.top(); pq.pop();

        Node* merge = new Node('\0', left->freq + right->freq);
        merge->left = left;
        merge->right = right;
        pq.push(merge);
    }

    std::unordered_map<char, std::string> codes;
    makeCode(pq.top(), "", codes);
    deleteTree(pq.top());

    std::string encoded_s = encode(s, codes);
    std::cout << letters.size() << " " << encoded_s.size() << '\n';
    for (const auto& code : codes) {
        std::cout << code.first << ": " << code.second << '\n';
    }
    std::cout << encoded_s << '\n';
}

int main() {
    std::string s;
    std::cin >> s;
    Huffman(s);
    return 0;
}
