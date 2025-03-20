using System.Text.Json.Serialization;

namespace BankHSE.Models;

public class Category
{
    public int Id { get; private set; }
    public string Name { get; private set; }
    public CategoryType Type { get; private set; }

    [JsonConstructor]
    public Category(int id, string name, CategoryType type)
    {
        if (string.IsNullOrEmpty(name))
        {
            throw new ArgumentException("Категория не может быть пустой.");
        }
        Id = id;
        Name = name;
        Type = type;
    }
}

public enum CategoryType
{
    Income,
    Expense
}