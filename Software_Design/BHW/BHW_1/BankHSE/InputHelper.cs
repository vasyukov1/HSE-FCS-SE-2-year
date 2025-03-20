using BankHSE.Models;

namespace BankHSE;

public static class InputHelper
{
    public static int ReadInt(string prompt, Func<int, bool>? validation = null, string errorMessage = "❌ Ошибка: Некорректный ввод.")
    {
        while (true)
        {
            Console.Write(prompt);
            string? input = Console.ReadLine();

            if (string.IsNullOrWhiteSpace(input))
            {
                Console.WriteLine("❌ Ошибка: Ввод не может быть пустым.");
                continue;
            }

            if (input.ToLower() == "отмена")
            {
                throw new OperationCanceledException("❌ Команда отменена пользователем.");
            }

            if (!int.TryParse(input, out int result))
            {
                Console.WriteLine(errorMessage);
                continue;
            }

            if (validation != null && !validation(result))
            {
                Console.WriteLine(errorMessage);
                continue;
            }

            return result;
        }
    }

    public static decimal ReadDecimal(string prompt, Func<decimal, bool>? validation = null, string errorMessage = "❌ Ошибка: Некорректный ввод.")
    {
        while (true)
        {
            Console.Write(prompt);
            string? input = Console.ReadLine();

            if (string.IsNullOrWhiteSpace(input))
            {
                Console.WriteLine("❌ Ошибка: Ввод не может быть пустым.");
                continue;
            }

            if (input.ToLower() == "отмена")
            {
                throw new OperationCanceledException("❌ Команда отменена пользователем.");
            }

            if (!decimal.TryParse(input, out decimal result))
            {
                Console.WriteLine(errorMessage);
                continue;
            }

            if (validation != null && !validation(result))
            {
                Console.WriteLine(errorMessage);
                continue;
            }

            return result;
        }
    }

    public static string ReadString(string prompt, Func<string, bool>? validation = null, string errorMessage = "❌ Ошибка: Некорректный ввод.")
    {
        while (true)
        {
            Console.Write(prompt);
            string? input = Console.ReadLine();

            if (string.IsNullOrWhiteSpace(input))
            {
                Console.WriteLine("❌ Ошибка: Ввод не может быть пустым.");
                continue;
            }

            if (input.ToLower() == "отмена")
            {
                throw new OperationCanceledException("❌ Команда отменена пользователем.");
            }

            if (validation != null && !validation(input))
            {
                Console.WriteLine(errorMessage);
                continue;
            }

            return input;
        }
    }

    public static string ReadSimpleString(string prompt)
    {
        while (true)
        {
            Console.Write(prompt);
            string? input = Console.ReadLine();

            if (string.IsNullOrWhiteSpace(input))
            {
                input = "";
            }

            return input;
        }
    }

    public static CategoryType ReadCategoryType()
    {
        while (true)
        {
            Console.WriteLine("Выберите категорию:");
            Console.WriteLine("1. Доход");
            Console.WriteLine("2. Трата");
            Console.WriteLine("0. Выйти из команды");
            Console.Write("\nВаш выбор: ");

            string? input = Console.ReadLine();

            if (input == null)
            {
                Console.WriteLine("❌ Ошибка: Введите число (1, 2 или 0).");
                continue;
            }

            if (input.ToLower() == "отмена")
            {
                throw new OperationCanceledException("Команда отменена пользователем.");
            }

            if (!int.TryParse(input, out int choice))
            {
                Console.WriteLine("❌ Ошибка: Введите число (1, 2 или 0).");
                continue;
            }

            switch (choice)
            {
                case 0:
                    throw new OperationCanceledException("Команда отменена пользователем.");
                case 1:
                    return CategoryType.Income;
                case 2:
                    return CategoryType.Expense;
                default:
                    Console.WriteLine("❌ Ошибка: Введите 1, 2 или 0.");
                    break;
            }
        }
    }

    public static OperationType ReadOperationType()
    {
        while (true)
        {
            Console.WriteLine("Выберите категорию:");
            Console.WriteLine("1. Доход");
            Console.WriteLine("2. Трата");
            Console.WriteLine("0. Выйти из команды");
            Console.Write("\nВаш выбор: ");

            string? input = Console.ReadLine();

            if (input == null)
            {
                Console.WriteLine("❌ Ошибка: Введите число (1, 2 или 0).");
                continue;
            }

            if (input.ToLower() == "отмена")
            {
                throw new OperationCanceledException("Команда отменена пользователем.");
            }

            if (!int.TryParse(input, out int choice))
            {
                Console.WriteLine("❌ Ошибка: Введите число (1, 2 или 0).");
                continue;
            }

            switch (choice)
            {
                case 0:
                    throw new OperationCanceledException("Команда отменена пользователем.");
                case 1:
                    return OperationType.Income;
                case 2:
                    return OperationType.Expense;
                default:
                    Console.WriteLine("❌ Ошибка: Введите 1, 2 или 0.");
                    break;
            }
        }
    }
}