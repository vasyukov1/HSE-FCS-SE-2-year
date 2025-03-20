using BankHSE.Facade;
using BankHSE.Models;

namespace BankHSE.Command;

public class CreateOperationCommand(OperationFacade operationFacade, BankAccountFacade accountFacade, CategoryFacade categoryFacade) : ICommand
{
    private readonly OperationFacade _operationFacade = operationFacade;
    private readonly BankAccountFacade _accountFacade = accountFacade;
    private readonly CategoryFacade _categoryFacade = categoryFacade;
    private Operation? _operation;
    private int _id;
    private OperationType _type;
    private int _accountIdFrom;
    private int _accountIdTo;
    private decimal _amount;
    private DateTime _date;
    private string? _description;
    private int _categoryId;

    public void Execute()
    {
        try
        {
            _id = new Random().Next(1000, 9999);

            _type = InputHelper.ReadOperationType();

            _accountIdFrom = InputHelper.ReadInt(
                "Введите ID отправителя (или 'отмена' для отмены команды): ",
                id => _accountFacade.GetBankAccount(id) != null,
                "❌ Ошибка: Аккаунт с таким ID не найден."
            );

             _accountIdTo = InputHelper.ReadInt(
                "Введите ID получателя (или 'отмена' для отмены команды): ",
                id => _accountFacade.GetBankAccount(id) != null,
                "❌ Ошибка: Аккаунт с таким ID не найден."
            );

            _amount = InputHelper.ReadDecimal(
                "Введите сумму перевода: ",
                amount => _accountFacade.GetBankAccount(_accountIdFrom).Balance >= amount,
                "❌ Недостаточно средств!"
            );

            _date = DateTime.Now;

            _description = InputHelper.ReadSimpleString("Введите описание операции: ");

            _categoryId = InputHelper.ReadInt(
                "Введите ID категории (или 'отмена' для отмены команды): ",
                id => _categoryFacade.GetCategory(id) != null,
                "❌ Ошибка: Категория с таким ID не найдена."
            );

            _operation = _operationFacade.CreateOperation(_id, _amount, _date, _description, _accountIdFrom, _accountIdTo, _categoryId, _type);
            Console.WriteLine($"✅ Операция '{_id}' успешно проведена!");

        }
        catch (OperationCanceledException)
        {
            Console.WriteLine("❌ Команда создания операции отменена.");
        }
        catch (ArgumentException)
        {
            Console.WriteLine("❌ Ошибка создания операции.");
        }
        catch (InvalidOperationException ex)
        {
            Console.WriteLine(ex.Message);
        }
    }

    public void Undo()
    {
        _operationFacade.DeleteOperation(_id);
        Console.WriteLine($"✅ Операция {_id} удалена.");
    }

    public void Redo()
    {
        _operation = _operationFacade.CreateOperation(_id, _amount, _date, _description!, _accountIdFrom, _accountIdTo, _categoryId, _type);
        Console.WriteLine($"✅ Операция '{_id}' успешно восстановлена! ID: {_id}");
    }

}