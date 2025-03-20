using System.Collections.Generic;
using System.Text;
using BankHSE.Models;

namespace BankHSE.Export;

public class CsvExportStrategy : IExportStrategy
{
    public string ExportData(IEnumerable<BankAccount> accounts, IEnumerable<Operation> operations, IEnumerable<Category> categories)
    {
        var sb = new StringBuilder();

        foreach (var account in accounts)
        {
            sb.AppendLine($"Account,{account.Id},{account.Name},{account.Balance}");
        }

        foreach (var operation in operations)
        {
            sb.AppendLine($"Operation,{operation.Id},{operation.Amount},{operation.Date},{operation.Description},{operation.AccountIdFrom},{operation.AccountIdTo},{operation.CategoryId},{operation.Type}");
        }

        foreach (var category in categories)
        {
            sb.AppendLine($"Category,{category.Id},{category.Name},{category.Type}");
        }

        return sb.ToString();
    }
}