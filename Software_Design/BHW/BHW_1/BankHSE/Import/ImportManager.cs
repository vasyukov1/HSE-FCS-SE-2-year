using BankHSE.Models;
using BankHSE.Facade;

namespace BankHSE.Import;

public class ImportManager()
{
    public void ImportData(BankAccountFacade accountFacade, OperationFacade operationFacade, CategoryFacade categoryFacade)
    {
        try
        {
            Console.WriteLine("Выберите формат импорта:");
            Console.WriteLine("1. CSV");
            Console.WriteLine("2. JSON");

            int choice = InputHelper.ReadInt(
                "Ваш выбор: ",
                input => 1 <= input && input <= 3,
                "❌ Ошибка: Некорректный ввод."
            );

            string filePath = InputHelper.ReadString(
                "Введите путь к файлу для импорта: ",
                input => !string.IsNullOrWhiteSpace(input),
                "❌ Ошибка: Путь к файлу не может быть пустым."
            );

            Importer importer = choice switch
            {
                1 => new CsvImporter(),
                2 => new JsonImporter(),
                _ => throw new InvalidOperationException("Неподдерживаемый формат.")
            };

            var (accounts, operations, categories) = importer.Import(filePath);

            RestoreData(
                accountFacade, 
                operationFacade, 
                categoryFacade, 
                accounts, 
                operations, 
                categories
            );
            Console.WriteLine($"✅ Данные успешно импортированы!");
        }
        catch (OperationCanceledException)
        {
            Console.WriteLine("❌ Импорт отменён.");
        }
        catch (ArgumentException ex)
        {
            Console.WriteLine($"❌ Ошибка импорта: {ex.Message}");
        }
        catch (FileNotFoundException)
        {
            Console.WriteLine("❌ Ошибка: Файл не найден.");
        }
        catch (Exception ex)
        {
            Console.WriteLine($"❌ Неизвестная ошибка: {ex}");
        }
    }

    private static void RestoreData(
        BankAccountFacade accountFacade, 
        OperationFacade operationFacade, 
        CategoryFacade categoryFacade, 
        List<BankAccount> accounts, 
        List<Operation> operations, 
        List<Category> categories
    )
    {
        foreach (BankAccount account in accounts)
        {
            accountFacade.CreateBankAccount(account.Id, account.Name, account.Balance);
        }

        foreach (Operation operation in operations)
        {
            operationFacade.CreateOperation(
                operation.Id, 
                operation.Amount, 
                operation.Date, 
                operation.Description, 
                operation.AccountIdFrom, 
                operation.AccountIdTo, 
                operation.CategoryId, 
                operation.Type
            );
        }

        foreach (Category category in categories)
        {
            categoryFacade.CreateCategory(category.Id, category.Name, category.Type);
        }
    }
}