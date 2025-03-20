using BankHSE.Models;

namespace BankHSE.Export;

public class LoggingExportDecorator : ExportStrategyDecorator
{
    public LoggingExportDecorator(IExportStrategy innerStrategy) : base(innerStrategy) {}

    public override string ExportData(IEnumerable<BankAccount> accounts, IEnumerable<Operation> operations, IEnumerable<Category> categories)
    {
        Console.WriteLine("🟢 Начало экспорта данных...");
        var result = base.ExportData(accounts, operations, categories);
        Console.WriteLine("🟢 Экспорт данных завершён.");
        return result;
    }
}
