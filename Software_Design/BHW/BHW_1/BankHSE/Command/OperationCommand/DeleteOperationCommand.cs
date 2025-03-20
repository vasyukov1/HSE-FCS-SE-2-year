using BankHSE.Facade;
using BankHSE.Models;

namespace BankHSE.Command;

public class DeleteOperationCommand(OperationFacade operationFacade) : ICommand
{
    private readonly OperationFacade _operationFacade = operationFacade;
    private Operation? _operation;

    public void Execute()
    {
        try
        {
            int operationId = InputHelper.ReadInt(
                "Введите ID операции (или 'отмена' для отмены команды): ",
                id => _operationFacade.GetOperation(id) != null,
                "❌ Ошибка: Операция с таким ID не найдена."
            );
            _operation = _operationFacade.GetOperation(operationId);
            _operationFacade.DeleteOperation(operationId);
            Console.WriteLine($"✅ Операция {_operation.Id} удалёна!");
        }
        catch (OperationCanceledException)
        {
            Console.WriteLine("❌ Команда удаления операции отменена.");
        }
        catch (InvalidOperationException ex)
        {
            Console.WriteLine(ex.Message);
        }
    }

    public void Undo()
    {
        if (_operation == null)
        {
            throw new InvalidOperationException("❌ Ошибка: Операция не была удалена или не найдена.");
        }
        _operationFacade.CreateOperation(
            _operation.Id, 
            _operation.Amount, 
            _operation.Date, 
            _operation.Description, 
            _operation.AccountIdFrom, 
            _operation.AccountIdTo, 
            _operation.CategoryId, 
            _operation.Type
        );
        Console.WriteLine($"✅ Операция {_operation!.Id} восстановлена!");
    }

    public void Redo()
    {
        if (_operation == null)
        {
            throw new InvalidOperationException("❌ Ошибка: Операция не была удалена или не найдена.");
        }
        _operationFacade.DeleteOperation(_operation.Id);
        Console.WriteLine($"✅ Операция {_operation.Id} снова удалёна!");
    }
}