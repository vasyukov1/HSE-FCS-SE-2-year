using System.Text.Json.Serialization;

namespace BankHSE.Models;

public class BankAccount 
{
    public int Id { get; private set; } 
    public string Name { get; private set; }
    public decimal Balance { get; private set; }
    
    [JsonConstructor]
    public BankAccount(int id, string name, decimal balance)
    {
        if (string.IsNullOrEmpty(name))
        {
            throw new ArgumentException("Необходимо указать имя.");
        }
        if (balance < 0)
        {
            throw new ArgumentException("Баланс не может быть отрицательным.");
        }
        Id = id;
        Name = name;
        Balance = balance;
    }

    public void UpdateBalance(decimal amount)
    {
        if (Balance < amount)
        {
            throw new InvalidOperationException("Баланс не должен быть отрицательным.");
        }
        Balance += amount;
    }
}