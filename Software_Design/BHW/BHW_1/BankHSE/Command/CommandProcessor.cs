using BankHSE.Facade;
using BankHSE.Export;
using BankHSE.Import;

namespace BankHSE.Command;

public class CommandProcessor(BankAccountFacade accountFacade, OperationFacade operationFacade, CategoryFacade categoryFacade)
{
    private readonly BankAccountFacade _accountFacade = accountFacade;
    private readonly OperationFacade _operationFacade = operationFacade;
    private readonly CategoryFacade _categoryFacade = categoryFacade;

    private readonly Stack<ICommand> _undoStack = new();
    private readonly Stack<ICommand> _redoStack = new();

    public void Process(string commandNumber)
    {
        ICommand? command = null;
        switch (commandNumber)
        {
            case "1": command = new CreateBankAccountCommand(_accountFacade); break;
            case "2": command = new DeleteBankAccountCommand(_accountFacade); break;
            case "3": _accountFacade.ShowAllBankAccounts(); break;
            case "4": command = new CreateCategoryCommmand(_categoryFacade); break;
            case "5": command = new DeleteCategoryCommand(_categoryFacade); break;
            case "6": _categoryFacade.ShowAllCategories(); break;
            case "7": command = new CreateOperationCommand(_operationFacade, _accountFacade, _categoryFacade); break;
            case "8": command = new DeleteOperationCommand(_operationFacade); break;
            case "9": _operationFacade.ShowAllOperations(); break;
            case "10": _operationFacade.ShowOperationsForAccountId(); break;
            case "11":
                var exportManager = new ExportManager(_accountFacade.GetAllBankAccounts(), _operationFacade.GetAllOperations(), _categoryFacade.GetAllCategories());
                exportManager.ExportData(); break;
            case "12":
                var importManager = new ImportManager();
                importManager.ImportData(_accountFacade, _operationFacade, _categoryFacade); break;
            case "13": Undo(); break;
            case "14": Redo(); break;
            default: Console.WriteLine("❌ Неизвестная команда!"); break;
        }

        if (command != null)
        {
            command.Execute();
            _undoStack.Push(command);
            _redoStack.Clear();
        }
    }

    private void Undo()
    {
        if (_undoStack.Count > 0)
        {
            var command = _undoStack.Pop();
            command.Undo();
            _redoStack.Push(command);
            Console.WriteLine("✅ Команда отменена!");
        }
        else
        {
            Console.WriteLine("❌ Нет команд для отмены!");
        }
    }

    private void Redo()
    {
        if (_redoStack.Count > 0) 
        {
            var command = _redoStack.Pop();
            command.Redo();
            _undoStack.Push(command);
            Console.WriteLine("✅ Команда возвращена!");
        }
        else
        {
            Console.WriteLine("❌ Нет команд для повтора!");
        }
    }
}