using BankHSE.Models;

namespace BankHSE.Facade;

public class CategoryFacade(BankFactory factory)
{
    private readonly BankFactory _factory = factory;
    private readonly List<Category> _categories = [];

    public Category CreateCategory(int id, string name, CategoryType type)
    {
        var category = _factory.CreateCategory(id, name, type);
        _categories.Add(category);
        return category;
    }

    public Category GetCategory(int id)
    {
        var category = _categories.FirstOrDefault(category => category.Id == id) ?? throw new InvalidOperationException($"❌ Категория с ID {id} не найдена.");
        return category;
    }

    public void DeleteCategory(int id)
    {
        var category = _categories.FirstOrDefault(c => c.Id == id);
        if (category != null)
        {
            _categories.Remove(category);
        }
    }

    public void ShowAllCategories()
    {
        int index = 1;
        Console.WriteLine("\n📋 Список категорий:");
        foreach (Category category in _categories)
        {
            Console.WriteLine($"{index}. [{category.Id}] {category.Name}\t{category.Type}");
        }
        if (_categories.Count == 0)
        {
            Console.WriteLine("Категорий нет.");
        }
    }

    public List<Category> GetAllCategories()
    {
        return _categories;
    }
}