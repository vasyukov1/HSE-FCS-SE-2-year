using BankHSE.Models;

namespace BankHSE.Import;

public abstract class Importer(IImportStrategy strategy)
{
    private readonly IImportStrategy _strategy = strategy;

    public (List<BankAccount>, List<Operation>, List<Category>) Import(string filePath)
    {
        return _strategy.Import(filePath);
    }
}