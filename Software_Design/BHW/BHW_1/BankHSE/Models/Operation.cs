using System.Text.Json.Serialization;

namespace BankHSE.Models;

public class Operation 
{
    public int Id { get; private set; }
    public OperationType Type { get; private set; }
    public int AccountIdFrom { get; private set; }
    public int AccountIdTo { get; private set; }
    public decimal Amount { get; private set; }
    public DateTime Date { get; private set; }
    public string Description { get; private set; }
    public int CategoryId { get; private set; }

    [JsonConstructor]
    public Operation(int id, decimal amount, DateTime date, string description, 
                     int accountIdFrom, int accountIdTo, int categoryId, OperationType type)
    {
        Id = id;
        Amount = amount;
        Date = date;
        Description = description;
        AccountIdFrom = accountIdFrom;
        AccountIdTo = accountIdTo;
        CategoryId = categoryId;
        Type = type;
    }
}

public enum OperationType
{
    Income,
    Expense
}