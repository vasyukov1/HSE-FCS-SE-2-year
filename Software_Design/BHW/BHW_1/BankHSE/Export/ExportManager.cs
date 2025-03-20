using BankHSE.Models;

namespace BankHSE.Export;

public class ExportManager(List<BankAccount> accounts, List<Operation> operations, List<Category> categories)
{
    private readonly List<BankAccount> _accounts = accounts;
    private readonly List<Operation> _operations = operations;
    private readonly List<Category> _categories = categories;

    public void ExportData()
    {
        try
        {
            Console.WriteLine("Выберите формат экспорта:");
            Console.WriteLine("1. CSV");
            Console.WriteLine("2. JSON");
            Console.WriteLine("3. YAML");

            int choice = InputHelper.ReadInt(
                "Ваш выбор: ",
                input => 1 <= input && input <= 3,
                "❌ Ошибка: Некорректный ввод."
            );
            Exporter exporter = choice switch
            {
                1 => new CsvExporter(),
                2 => new JsonExporter(),
                3 => new YamlExporter(),
                _ => throw new InvalidOperationException("Неподдерживаемый формат.")
            };

            string filePath = "export." + (choice == 1 ? "csv" : choice == 2 ? "json" : "yaml");
            exporter.Export(filePath, _accounts, _operations, _categories);

            Console.WriteLine($"✅ Данные успешно экспортированы в файл: {filePath}");
        }
        catch (OperationCanceledException)
        {
            Console.WriteLine("❌ Экспорт отменён.");
        }
        catch (ArgumentException)
        {
            Console.WriteLine("❌ Ошибка экспорта.");
        }
    }
}