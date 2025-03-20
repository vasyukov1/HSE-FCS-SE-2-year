namespace BankHSE.Models;

public class BankFactory
{
    public BankAccount CreateBankAccount(int id, string name, decimal balance)
    {
        return new BankAccount(id, name, balance);
    }

    public Category CreateCategory(int id, string name, CategoryType type)
    {
        return new Category(id, name, type);
    }

    public Operation CreateOperation(int id, decimal amount, DateTime date, string description,
                                      int accountIdFrom, int accountIdTo, int categoryId, OperationType type)
    {
        if (amount <= 0)
        {
            throw new ArgumentException("Сумма должна быть положительной.");
        }
        return new Operation(id, amount, date, description, accountIdFrom, accountIdTo, categoryId, type);
    }
}