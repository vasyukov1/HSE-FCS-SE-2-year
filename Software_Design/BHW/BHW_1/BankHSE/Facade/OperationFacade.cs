using BankHSE.Models;

namespace BankHSE.Facade;

public class OperationFacade(BankFactory factory, BankAccountFacade accountFacade)
{
    private readonly BankFactory _factory = factory;
    private readonly BankAccountFacade _accountFacade = accountFacade;
    private readonly List<Operation> _operations = [];

    public Operation CreateOperation(int id, decimal amount, DateTime date, string description, 
                                     int accountIdFrom, int accountIdTo, int categoryId, OperationType type)
    {
        var operation = _factory.CreateOperation(id, amount, date, description, accountIdFrom, accountIdTo, categoryId, type);
        if (_accountFacade.UpdateBankAccountBalance(accountIdFrom, accountIdTo, amount))
        {
            _operations.Add(operation);
        }
        return operation;
    }

    public void DeleteOperation(int id)
    {
        var operation = _operations.FirstOrDefault(op => op.Id == id);
        if (operation != null)
        {
            _accountFacade.UpdateBankAccountBalance(operation.AccountIdTo, operation.AccountIdFrom, operation.Amount);
            _operations.Remove(operation);
        }
    }

    public Operation GetOperation(int id)
    {
        var operation = _operations.FirstOrDefault(operation => operation.Id == id) ?? throw new InvalidOperationException($"❌ Операция с ID {id} не найдена.");
        return operation;
    }

    public void ShowAllOperations()
    {
        Console.WriteLine("\n📋 Список операций:");
        foreach (Operation operation in _operations)
        {
            Console.WriteLine($"{operation.Date}. [{operation.Id}] {operation.AccountIdFrom} -> {operation.AccountIdFrom}: {operation.Amount}$");
        }
        if (_operations.Count == 0)
        {
            Console.WriteLine("Операций нет.");
        }
    }

    public void ShowOperationsForAccountId()
    {
        try
        {
            int accountId = InputHelper.ReadInt(
                "Введите ID счёта (или 'отмена' для отмены команды): ",
                id => _accountFacade.GetBankAccount(id) != null,
                "❌ Ошибка: Счёт с таким ID не найден."
            );

            Console.WriteLine($"\n📋 Список операций аккаунта ID {accountId}:");
            foreach (Operation operation in _operations)
            {
                if (operation.AccountIdFrom == accountId || operation.AccountIdTo == accountId)
                Console.WriteLine($"{operation.Date}. [{operation.Id}] {operation.Type} with ID {operation.AccountIdFrom}: {operation.Amount}$\t{operation.CategoryId}");
            }
            if (_operations.Count == 0)
            {
                Console.WriteLine("Операций нет.");
            }
        }
        catch (OperationCanceledException)
        {
            Console.WriteLine("❌ Команда удаления счёта отменена.");
        }
        catch (InvalidOperationException ex)
        {
            Console.WriteLine(ex.Message);
        }
    }

    public List<Operation> GetAllOperations()
    {
        return _operations;
    }
}