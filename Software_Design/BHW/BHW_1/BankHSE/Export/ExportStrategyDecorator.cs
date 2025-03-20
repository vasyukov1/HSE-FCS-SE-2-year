using BankHSE.Models;

namespace BankHSE.Export;

public class ExportStrategyDecorator : IExportStrategy
{
    protected readonly IExportStrategy _innerStrategy;

    public ExportStrategyDecorator(IExportStrategy innerStrategy)
    {
        _innerStrategy = innerStrategy;
    }

    public virtual string ExportData(IEnumerable<BankAccount> accounts, IEnumerable<Operation> operations, IEnumerable<Category> categories)
    {
        return _innerStrategy.ExportData(accounts, operations, categories);
    }
}
