using BankHSE.Models;

namespace BankHSE.Export;

public class TimeExportDecorator : ExportStrategyDecorator
{
    public TimeExportDecorator(IExportStrategy innerStrategy) : base(innerStrategy) {}

    public override string ExportData(IEnumerable<BankAccount> accounts, IEnumerable<Operation> operations, IEnumerable<Category> categories)
    {
        var data = base.ExportData(accounts, operations, categories);
        var timestamp = $"Export Time: {DateTime.Now}";
        return $"{data}\n{timestamp}";
    }
}
