using BankHSE.Facade;
using BankHSE.Models;

namespace BankHSE.Command;

public class DeleteBankAccountCommand(BankAccountFacade accountFacade) : ICommand
{
    private readonly BankAccountFacade _accountFacade = accountFacade;
    private BankAccount? _account;

    public void Execute()
    {
        try
        {
            int accountId = InputHelper.ReadInt(
                "Введите ID счёта (или 'отмена' для отмены команды): ",
                id => _accountFacade.GetBankAccount(id) != null,
                "❌ Ошибка: Счёт с таким ID не найден."
            );
            _account = _accountFacade.GetBankAccount(accountId);
            _accountFacade.DeleteBankAccount(accountId);
            Console.WriteLine($"✅ Счёт {accountId} удалён!");
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

    public void Undo()
    {
        _accountFacade.CreateBankAccount(_account!.Id, _account.Name, _account.Balance);
        Console.WriteLine($"✅ Счёт {_account.Id} восстановлен!");
    }

    public void Redo()
    {
        _accountFacade.DeleteBankAccount(_account!.Id);
        Console.WriteLine($"✅ Счёт {_account.Id} снова удалён!");
    }
}