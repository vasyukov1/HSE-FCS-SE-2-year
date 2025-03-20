using BankHSE.Models;

namespace BankHSE.Import;

public class CsvImportStrategy : IImportStrategy
{
    public (List<BankAccount>, List<Operation>, List<Category>) Import(string filePath)
    {
        var accounts = new List<BankAccount>();
        var operations = new List<Operation>();
        var categories = new List<Category>();

        foreach (var line in File.ReadLines(filePath))
        {
            var parts = line.Split(',');

            switch (parts[0])
            {
                case "Account":
                    int id = int.Parse(parts[1]);
                    string name = parts[2];
                    decimal balance = decimal.Parse(parts[3]);
                    accounts.Add(new BankAccount(id, name, balance));
                    break;

                case "Operation":
                    id = int.Parse(parts[1]);
                    decimal amount = decimal.Parse(parts[2]);
                    DateTime date = DateTime.Parse(parts[3]);
                    string description = parts[4];
                    int accountIdFrom = int.Parse(parts[5]);
                    int accountIdTo = int.Parse(parts[6]);
                    int categoryId = int.Parse(parts[7]);
                    OperationType type = Enum.Parse<OperationType>(parts[8]);

                    operations.Add(new Operation(id, amount, date, description, accountIdFrom, accountIdTo, categoryId, type));
                    break;

                case "Category":
                    id = int.Parse(parts[1]);
                    name = parts[2];
                    CategoryType typeCategory = Enum.Parse<CategoryType>(parts[3]);
                    categories.Add(new Category(id, name, typeCategory));
                    break;
            }
        }

        return (accounts, operations, categories);
    }
}