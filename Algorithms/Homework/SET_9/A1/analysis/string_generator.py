import random

random.seed(42)

class StringGenerator():
    """
    Класс StringGenerator для генерации выборки случайных строк.
    """
    symbols = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#%:;^&*()-"
    
    strings_len = 3000
    string_min_len = 10
    string_max_len = 200

    prefix_min_len = 3
    prefix_max_len = 10
    count_prefix = 10
    prefix_strings_min_count = 5
    prefix_strings_max_count = 30

    def generate_string(self, min_len: int = string_min_len, max_len: int = string_max_len) -> str:
        string_len = random.randint(min_len, max_len)
        string = []
        for _ in range(string_len):
            symbol = random.randint(0, len(self.symbols) - 1)
            string.append(self.symbols[symbol])
        return "".join(string)
    
    def generate_prefix_string(self) -> list:
        strings = []
        for _ in range(self.count_prefix):
            prefix = self.generate_string(self.prefix_min_len, self.prefix_max_len)
            string_with_prefix_count = random.randint(self.prefix_strings_min_count, self.prefix_strings_max_count)
            for _ in range(string_with_prefix_count):
                prefix_len = random.randint(self.prefix_min_len, len(prefix))
                strings.append(prefix[:prefix_len] + self.generate_string())
        return strings

    def generate(self) -> list:
        strings = self.generate_prefix_string()
        count_without_prefix = self.strings_len - len(strings)
        for _ in range(count_without_prefix):
            strings.append(self.generate_string())
        return strings
    
    def generate_sorted(self) -> list:
        return sorted(self.generate())
    
    def generate_reversed_sorted(self) -> list:
        return self.generate_sorted()[::-1]

    def generate_almost_sorted(self) -> list:
        strings = self.generate_sorted()
        k = random.randint(50, 150)
        for _ in range(k):
            a = random.randint(0, len(strings) - 1)
            b = random.randint(0, len(strings) - 1)
            strings[a], strings[b] = strings[b], strings[a]
        return strings