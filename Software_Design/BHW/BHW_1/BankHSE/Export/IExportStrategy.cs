using BankHSE.Models;

namespace BankHSE.Export;

public interface IExportStrategy
{
    string ExportData(IEnumerable<BankAccount> accounts, IEnumerable<Operation> operations, IEnumerable<Category> categories);
}