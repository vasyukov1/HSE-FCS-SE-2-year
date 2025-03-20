using Xunit;
using BankHSE.Models;

namespace BankHSE.Tests;

public class BankFactoryTest
{
    private readonly BankFactory _bankFactory;
    public BankFactoryTest()
    {
        _bankFactory = new BankFactory();
    }

    [Fact]
    public void CreateBankAccountTest()
    {
        var expectedId = 1;
        var expectedName = "Test Account";
        var expectedBalance = 1000m;

        var bankAccount = _bankFactory.CreateBankAccount(expectedId, expectedName, expectedBalance);

        Assert.Equal(expectedId, bankAccount.Id);
        Assert.Equal(expectedName, bankAccount.Name);
        Assert.Equal(expectedBalance, bankAccount.Balance);
    }

    [Fact]
    public void CreateCategoryTest()
    {
        var expectedId = 1;
        var expectedName = "Test Category";
        CategoryType expectedType = CategoryType.Income;

        var category = _bankFactory.CreateCategory(expectedId, expectedName, expectedType);
        
        Assert.Equal(expectedId, category.Id);
        Assert.Equal(expectedName, category.Name);
        Assert.Equal(expectedType, category.Type);
    }

    [Fact]
    public void CreateOperationTest()
    {
        var expectedId = 1;
        var expectedAmount = 100;
        DateTime expectedDate = DateTime.Now;
        var expectedDescription = "Test Description";
        var expectedAccountIdFrom = 1000;
        var expectedAccountIdTo = 2000;
        var expectedCategoryId = 1;
        OperationType expectedType = OperationType.Expense;

        var operation = _bankFactory.CreateOperation(
            expectedId,
            expectedAmount, 
            expectedDate, 
            expectedDescription, 
            expectedAccountIdFrom, 
            expectedAccountIdTo, 
            expectedCategoryId, 
            expectedType
        );
        Assert.Equal(expectedId, operation.Id);
        Assert.Equal(expectedAmount, operation.Amount);
        Assert.Equal(expectedDate, operation.Date);
        Assert.Equal(expectedDescription, operation.Description);
        Assert.Equal(expectedAccountIdFrom, operation.AccountIdFrom);
        Assert.Equal(expectedAccountIdTo, operation.AccountIdTo);
        Assert.Equal(expectedCategoryId, operation.CategoryId);
        Assert.Equal(expectedType, operation.Type);
    }

    [Fact]
    public void CreateInvaliOperationTest()
    {
        var expectedId = 1;
        var expectedAmount = -100;
        DateTime expectedDate = DateTime.Now;
        var expectedDescription = "Invalid Test Description";
        var expectedAccountIdFrom = 1000;
        var expectedAccountIdTo = 2000;
        var expectedCategoryId = 1;
        OperationType expectedType = OperationType.Expense;

        var exception = Assert.Throws<ArgumentException>(() => _bankFactory.CreateOperation(
            expectedId,
            expectedAmount, 
            expectedDate, 
            expectedDescription, 
            expectedAccountIdFrom, 
            expectedAccountIdTo, 
            expectedCategoryId, 
            expectedType
        ));
            
        Assert.Equal("Сумма должна быть положительной.", exception.Message);
    }
}