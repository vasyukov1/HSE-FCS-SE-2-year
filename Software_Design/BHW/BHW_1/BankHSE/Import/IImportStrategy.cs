using BankHSE.Models;

namespace BankHSE.Import;

public interface IImportStrategy
{
    (List<BankAccount>, List<Operation>, List<Category>) Import(string filePath);
}