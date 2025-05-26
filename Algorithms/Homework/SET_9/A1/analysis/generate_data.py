import json
import string_generator as sg

"""
Создание генератора и генерация 3 выборок:
- Случайная выборка
- Обратно отсортированная выборка
- Почти отсортированная выборка
"""
generator = sg.StringGenerator()
array_random = generator.generate()
array_reversed = generator.generate_reversed_sorted()
array_almost_sorted = generator.generate_almost_sorted()

# Создание json-файла с полученными выборками.
data = {
    "random": array_random,
    "reversed": array_reversed,
    "almost_sorted": array_almost_sorted
}

with open("data/strings.json", "w", encoding="utf-8") as f:
    json.dump(data, f, ensure_ascii=False, indent=2)