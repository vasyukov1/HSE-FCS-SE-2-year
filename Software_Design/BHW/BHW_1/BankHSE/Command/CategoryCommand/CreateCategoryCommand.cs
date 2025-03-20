using BankHSE.Facade;
using BankHSE.Models;

namespace BankHSE.Command;

public class CreateCategoryCommmand(CategoryFacade categoryFacade) : ICommand
{
    private readonly CategoryFacade _categoryFacade = categoryFacade;
    private Category? _category;
    private int _id;
    private string? _name;
    private CategoryType _type;


    public void Execute()
    {
        try
        {
            _id = new Random().Next(1, 100);

            _name = InputHelper.ReadString(
                "Введите название категории (или 'отмена' для отмены команды): ",
                input => !string.IsNullOrWhiteSpace(input),
                "❌ Ошибка: Название категории не может быть пустым."
            );
            
            _type = InputHelper.ReadCategoryType();

            _category = _categoryFacade.CreateCategory(_id, _name, _type);
            Console.WriteLine($"✅ Категория '{_name}' успешно создана! ID: {_id}");
        }
        catch (OperationCanceledException)
        {
            Console.WriteLine("❌ Команда создания категории отменена.");
        }
        catch (ArgumentException)
        {
            Console.WriteLine("❌ Ошибка создания категории.");
        }
    }

    public void Undo()
    {
        _categoryFacade.DeleteCategory(_category!.Id);
        Console.WriteLine($"✅ Категория {_name} удалена.");
    }

    public void Redo()
    {
        _category = _categoryFacade.CreateCategory(_id, _name!, _type);
        Console.WriteLine($"✅ Категория '{_name}' успешно восстановлена! ID: {_id}");
    }
}