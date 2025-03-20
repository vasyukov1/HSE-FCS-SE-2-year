using BankHSE.Facade;
using BankHSE.Models;

namespace BankHSE.Command;

public class DeleteCategoryCommand(CategoryFacade categoryFacade) : ICommand
{
    private readonly CategoryFacade _categoryFacade = categoryFacade;
    private Category? _category;

    public void Execute()
    {
        try
        {
            int categoryId = InputHelper.ReadInt(
                "Введите ID категории (или 'отмена' для отмены команды): ",
                id => _categoryFacade.GetCategory(id) != null,
                "❌ Ошибка: Категория с таким ID не найдена."
            );
            _category = _categoryFacade.GetCategory(categoryId);
            _categoryFacade.DeleteCategory(categoryId);
            Console.WriteLine($"✅ Категория [{categoryId}] {_category!.Name} удалёна!");
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
        _categoryFacade.CreateCategory(_category!.Id, _category.Name, _category.Type);
        Console.WriteLine($"✅ Категория {_category.Id} восстановлена!");
    }

    public void Redo()
    {
        _categoryFacade.DeleteCategory(_category!.Id);
        Console.WriteLine($"✅ Категория {_category.Id} снова удалёна!");
    }
}