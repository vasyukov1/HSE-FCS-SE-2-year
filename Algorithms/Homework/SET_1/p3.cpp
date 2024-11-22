#include <iostream>
#include <stack>
using std::cin;
using std::cout;
using std::string;
using std::stack;

int main() {
   string s;
   std::getline(std::cin, s);

   stack<int> counter;
   stack<string> words;

   for (char c : s) {
      if ('0' <= c && c <= '9') {
         string numberStr;
         numberStr = c;
         counter.push(std::stoi(numberStr));
      } else if (c == ']') {
         string word;
         while (!words.empty() && words.top() != "[") {
            word = words.top() + word;
            words.pop();
         }
         if (!words.empty() && words.top() == "[") {
            words.pop();
         }

         int repeat = counter.top();
         counter.pop();

         for (int i = 0; i < repeat; ++i) {
            words.push(word);
         }
      } else {
         string el;
         el = c;
         words.push(el);
      }
   }

   string result;
   while (!words.empty()) {
      result = words.top() + result ;
      words.pop();
   }

   cout << result;

   return 0;
}