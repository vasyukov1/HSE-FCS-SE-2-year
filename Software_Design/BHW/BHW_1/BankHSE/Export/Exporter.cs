using BankHSE.Models;

namespace BankHSE.Export;

public abstract class Exporter(IExportStrategy strategy)
{
    private readonly IExportStrategy _strategy = strategy;

    public void Export(string filePath, IEnumerable<BankAccount> accounts, IEnumerable<Operation> operations, IEnumerable<Category> categories)
    {
        var data = _strategy.ExportData(accounts, operations, categories);
        File.WriteAllText(filePath, data);
    }
}