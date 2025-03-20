using System.Text.Json;
using BankHSE.Models;

namespace BankHSE.Import;

public class JsonImportStrategy : IImportStrategy
{
    public (List<BankAccount>, List<Operation>, List<Category>) Import(string filePath)
    {
        var json = File.ReadAllText(filePath);
        var data = JsonSerializer.Deserialize<JsonElement>(json);

        var accounts = JsonSerializer.Deserialize<List<BankAccount>>(data.GetProperty("Accounts").ToString());
        var operations = JsonSerializer.Deserialize<List<Operation>>(data.GetProperty("Operations").ToString());
        var categories = JsonSerializer.Deserialize<List<Category>>(data.GetProperty("Categories").ToString());

        return (accounts, operations, categories);
    }
}