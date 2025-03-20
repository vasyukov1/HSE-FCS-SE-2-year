using BankHSE.Facade;
using BankHSE.Models;
using Xunit;

namespace BankHSE.Tests;

public class OperationFacadeTests
{
    private readonly BankFactory _factory;
    private readonly BankAccountFacade _accountFacade;
    private readonly OperationFacade _operationFacade;

    public OperationFacadeTests()
    {
        _factory = new BankFactory();
        _accountFacade = new BankAccountFacade(_factory);
        _operationFacade = new OperationFacade(_factory, _accountFacade);
    }

    [Fact]
    public void CreateOperationTest()
    {
        var operationId = 1;
        decimal amount = 100m;
        DateTime date = DateTime.Now;
        var description = "Test Operation";
        var accountIdFrom = 1;
        var accountIdTo = 2;
        var categoryId = 1;
        var operationType = OperationType.Income;

        _accountFacade.CreateBankAccount(accountIdFrom, "FckSD", 1000);
        _accountFacade.CreateBankAccount(accountIdTo, "BiLyaaaaaal", 666);

        var result = _operationFacade.CreateOperation(operationId, amount, date, description, accountIdFrom, accountIdTo, categoryId, operationType);

        Assert.NotNull(result);
        Assert.Equal(operationId, result.Id);
        Assert.Equal(amount, result.Amount);
        Assert.Equal(description, result.Description);
        Assert.Equal(accountIdFrom, result.AccountIdFrom);
        Assert.Equal(accountIdTo, result.AccountIdTo);
        Assert.Equal(categoryId, result.CategoryId);
        Assert.Equal(operationType, result.Type);

        var operations = _operationFacade.GetAllOperations();
        Assert.Contains(result, operations);
    }

    [Fact]
    public void CreateOperationInvalidTest()
    {
        var operationId = 1;
        var amount = 100m;
        var date = DateTime.Now;
        var description = "Test Operation";
        var accountIdFrom = 1;
        var accountIdTo = 999;
        var categoryId = 1;
        var operationType = OperationType.Income;

        var result = _operationFacade.CreateOperation(operationId, amount, date, description, accountIdFrom, accountIdTo, categoryId, operationType);
        
        Assert.NotNull(result);
        var operations = _operationFacade.GetAllOperations();
        Assert.DoesNotContain(result, operations);
    }

    [Fact]
    public void DeleteOperationTest()
    {
        var operationId = 1;
        var amount = 100m;
        var date = DateTime.Now;
        var description = "Test Operation";
        var accountIdFrom = 1;
        var accountIdTo = 2;
        var categoryId = 1;
        var operationType = OperationType.Income;

        _operationFacade.CreateOperation(operationId, amount, date, description, accountIdFrom, accountIdTo, categoryId, operationType);

        _operationFacade.DeleteOperation(operationId);
    }

    [Fact]
    public void DeleteOperationInvalidTest()
    {
        var operationId = 999;
        _operationFacade.DeleteOperation(operationId);

        var operations = _operationFacade.GetAllOperations();
        Assert.Empty(operations);
    }

    [Fact]
    public void GetOperationTest()
    {
        var operationId = 1;
        var amount = 100m;
        var date = DateTime.Now;
        var description = "Test Operation";
        var accountIdFrom = 1;
        var accountIdTo = 2;
        var categoryId = 1;
        var operationType = OperationType.Income;

        _accountFacade.CreateBankAccount(accountIdFrom, "FckSD", 1000);
        _accountFacade.CreateBankAccount(accountIdTo, "BLyaaaaaaL", 666);

        var createdOperation = _operationFacade.CreateOperation(operationId, amount, date, description, accountIdFrom, accountIdTo, categoryId, operationType);

        var retrievedOperation = _operationFacade.GetOperation(operationId);

        Assert.Equal(createdOperation, retrievedOperation);
    }

    [Fact]
    public void GetOperationInvalidTest()
    {
        var operationId = 999;

        Assert.Throws<InvalidOperationException>(() => _operationFacade.GetOperation(operationId));
    }

    [Fact]
    public void GetAllOperationsTest()
    {
        _accountFacade.CreateBankAccount(1, "Fack SD", 1000);
        _accountFacade.CreateBankAccount(2, "BiLyaaaaaal", 666);

        var operation1 = _operationFacade.CreateOperation(1, 100m, DateTime.Now, "Test Operation 1", 1, 2, 1, OperationType.Income);
        var operation2 = _operationFacade.CreateOperation(2, 200m, DateTime.Now, "Test Operation 2", 2, 1, 2, OperationType.Income);
        
        var allOperations = _operationFacade.GetAllOperations();

        Assert.Contains(operation1, allOperations);
        Assert.Contains(operation2, allOperations);
        Assert.Equal(2, allOperations.Count);
    }

    [Fact]
    public void GetAllOperationsEmptyTest()
    {
        var allOperations = _operationFacade.GetAllOperations();

        Assert.Empty(allOperations);
    }
}