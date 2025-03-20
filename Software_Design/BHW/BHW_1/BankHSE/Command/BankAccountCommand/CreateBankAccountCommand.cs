using BankHSE.Facade;
using BankHSE.Models;

namespace BankHSE.Command;

public class CreateBankAccountCommand(BankAccountFacade accountFacade) : ICommand
{
    private readonly BankAccountFacade _accountFacade = accountFacade;
    private BankAccount? _account;
    private int _id;
    private string? _name;
    private decimal _balance;

    public void Execute()
    {
        try
        {
            _id = new Random().Next(1000, 9999);

            _name = InputHelper.ReadString(
                "Введите название счёта (или 'отмена' для отмены команды): ",
                input => !string.IsNullOrWhiteSpace(input),
                "❌ Ошибка: Название счёта не может быть пустым."
            );

            _balance = InputHelper.ReadDecimal(
                "Введите начальный баланс (или 'отмена' для отмены команды): "
            );

            _account = _accountFacade.CreateBankAccount(_id, _name, _balance);
            Console.WriteLine($"✅ Счёт '{_name}' успешно создан! ID: {_id}");
        }
        catch (OperationCanceledException)
        {
            Console.WriteLine("❌ Команда создания счёта отменена.");
        }
        catch (ArgumentException)
        {
            Console.WriteLine("❌ Ошибка создания счёта.");
        }
    }

    public void Undo()
    {
        if (_account != null)
        {
            _accountFacade.DeleteBankAccount(_id);
            Console.WriteLine($"✅ Счёт [{_id}] {_name} удалён.");
        }
    }

    public void Redo()
    {
        if (!string.IsNullOrEmpty(_name))
        {
            _account = _accountFacade.CreateBankAccount(_id, _name, _balance);
            Console.WriteLine($"✅ Счёт '{_name}' успешно восстановлен! ID: {_id}");
        }
    }
}
