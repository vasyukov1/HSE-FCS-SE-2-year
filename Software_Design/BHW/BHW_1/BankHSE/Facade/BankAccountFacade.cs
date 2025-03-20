using BankHSE.Models;

namespace BankHSE.Facade;

public class BankAccountFacade(BankFactory factory)
{
    private readonly BankFactory _factory = factory;
    private readonly List<BankAccount> _accounts = [];

    public BankAccount CreateBankAccount(int id, string name, decimal balance)
    {
        var account = _factory.CreateBankAccount(id, name, balance);
        _accounts.Add(account);
        return account;
    }

    public BankAccount GetBankAccount(int id)
    {
        var account = _accounts.FirstOrDefault(account => account.Id == id) ?? throw new InvalidOperationException($"❌ Аккаунт с ID {id} не найден.");
        return account;
    }

    public void DeleteBankAccount(int id)
    {
        var account = _accounts.FirstOrDefault(a => a.Id == id);
        if (account != null)
        {
            _accounts.Remove(account);
        }
    }

    public void ShowAllBankAccounts()
    {
        int index = 1;
        Console.WriteLine("\n📋 Список счетов:");
        foreach (BankAccount account in _accounts)
        {
            Console.WriteLine($"{index}. [{account.Id}] {account.Name}\t${account.Balance}");
        }
        if (_accounts.Count == 0)
        {
            Console.WriteLine("Счетов нет.");
        }
    }

    public bool UpdateBankAccountBalance(int accountIdFrom, int accountIdTo, decimal amount)
    {
        try
        {
            var accountFrom = GetBankAccount(accountIdFrom);
            var accountTo = GetBankAccount(accountIdTo);
            accountFrom!.UpdateBalance(-amount);
            accountTo!.UpdateBalance(amount);
            return true;
        }
        catch (InvalidOperationException ex)
        {
            Console.WriteLine(ex.Message);
        }
        return false;
    }

    public List<BankAccount> GetAllBankAccounts()
    {
        return _accounts;
    }
}