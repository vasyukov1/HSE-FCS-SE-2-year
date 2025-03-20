using System.Text.Json;
using BankHSE.Models;

namespace BankHSE.Export;

public class JsonExportStrategy : IExportStrategy
{
    public string ExportData(IEnumerable<BankAccount> accounts, IEnumerable<Operation> operations, IEnumerable<Category> categories)
    {
        var data = new
        {
            Accounts = accounts,
            Operations = operations,
            Categories = categories
        };

        return JsonSerializer.Serialize(data);
    }
}