using BankHSE.Facade;
using BankHSE.Models;
using Xunit;

namespace BankHSE.Tests;

public class CategoryFacadeTests
{
    private readonly BankFactory _factory;
    private readonly CategoryFacade _categoryFacade;

    public CategoryFacadeTests()
    {
        _factory = new BankFactory();
        _categoryFacade = new CategoryFacade(_factory);
    }

    [Fact]
    public void CreateCategoryTest()
    {
        var category = _categoryFacade.CreateCategory(1, "Food", CategoryType.Expense);

        Assert.Equal(1, category.Id);
        Assert.Equal("Food", category.Name);
        Assert.Equal(CategoryType.Expense, category.Type);
    }

    [Fact]
    public void GetCategoryTest()
    {

        var createdCategory = _categoryFacade.CreateCategory(1, "Food", CategoryType.Expense);
        var retrievedCategory = _categoryFacade.GetCategory(1);

        Assert.Equal(createdCategory, retrievedCategory);
    }

    [Fact]
    public void GetCategoryInvalidTest()
    {
        Assert.Throws<InvalidOperationException>(() => _categoryFacade.GetCategory(999));
    }

    [Fact]
    public void DeleteCategoryTest()
    {
        _categoryFacade.CreateCategory(1, "Food", CategoryType.Expense);
        _categoryFacade.DeleteCategory(1);

        Assert.Throws<InvalidOperationException>(() => _categoryFacade.GetCategory(1));
    }

    [Fact]
    public void GetAllCategoriesTest()
    {
        var category1 = _categoryFacade.CreateCategory(1, "Food", CategoryType.Expense);
        var category2 = _categoryFacade.CreateCategory(2, "Salary", CategoryType.Income);

        var allCategories = _categoryFacade.GetAllCategories();

        Assert.Contains(category1, allCategories);
        Assert.Contains(category2, allCategories);
        Assert.Equal(2, allCategories.Count);
    }

    [Fact]
    public void GetAllCategoriesEmptyTest()
    {
        var allCategories = _categoryFacade.GetAllCategories();

        Assert.Empty(allCategories);
    }
} 
