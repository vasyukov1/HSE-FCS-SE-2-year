using BankHSE.Facade;
using BankHSE.Command;
using BankHSE.Models;
using Microsoft.Extensions.DependencyInjection;

namespace BankHSE;

class Program
{
    public static void Main()
    {
        var serviceProvider = new ServiceCollection()
                .AddSingleton<BankFactory>()
                .AddSingleton<BankAccountFacade>()
                .AddSingleton<OperationFacade>()
                .AddSingleton<CategoryFacade>()
                .AddSingleton<CommandProcessor>()
                .BuildServiceProvider();
        
        var commandProcessor = serviceProvider.GetRequiredService<CommandProcessor>();

        while (true)
        {
            Console.Clear();
            Console.WriteLine("🏦 Добро пожаловать в BankHSE!");
            Console.WriteLine("Выберите действие:");
            Console.WriteLine("1. Создать счёт");
            Console.WriteLine("2. Удалить счёт");
            Console.WriteLine("3. Просмотреть счета");
            Console.WriteLine("4. Создать категорию");
            Console.WriteLine("5. Удалить категорию");
            Console.WriteLine("6. Просмотреть категории");
            Console.WriteLine("7. Провести операция");
            Console.WriteLine("8. Удалить операцию");
            Console.WriteLine("9. Показать все операции");
            Console.WriteLine("10. Показать операции аккаунта");
            Console.WriteLine("11. Экспортировать данные");
            Console.WriteLine("12. Импортировать данные");
            Console.WriteLine("13. Undo command");
            Console.WriteLine("14. Redo command");
            Console.WriteLine("0. Выйти");

            string choice = InputHelper.ReadString(
                "\nВаш выбор: ",
                input => !string.IsNullOrWhiteSpace(input),
                "❌ Ошибка: Пустой ввод."
            );

            if (choice == "0") 
            {
                break;
            }
            commandProcessor.Process(choice);

            Console.WriteLine("\nНажмите любую клавишу для продолжения...");
            Console.ReadKey();
        }
    }
}
