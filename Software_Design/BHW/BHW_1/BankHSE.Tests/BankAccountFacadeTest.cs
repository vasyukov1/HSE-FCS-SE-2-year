using Xunit;
using BankHSE.Models;
using BankHSE.Facade;

namespace BankHSE.Tests;

public class BankAccountFacadeTest
{
    private readonly BankFactory _factory = new();

    [Fact]
    public void CreateBankAccountTest()
    {
        var facade = new BankAccountFacade(_factory);
        var account = facade.CreateBankAccount(1, "Test Account", 1000m);
        
        Assert.Equal(1, account.Id);
        Assert.Equal("Test Account", account.Name);
        Assert.Equal(1000m, account.Balance);
        Assert.Single(facade.GetAllBankAccounts());
    }

    [Fact]
    public void GetBankAccountTest()
    {
        var facade = new BankAccountFacade(_factory);
        facade.CreateBankAccount(1, "Account1", 500m);

        var account = facade.GetBankAccount(1);
        
        Assert.Equal("Account1", account.Name);
        Assert.Equal(500m, account.Balance);
    }

    [Fact]
    public void GetBankAccountInvalidTest()
    {
        var facade = new BankAccountFacade(_factory);
        
        Assert.Throws<InvalidOperationException>(() => facade.GetBankAccount(99));
    }

    [Fact]
    public void DeleteBankAccountTest()
    {
        var facade = new BankAccountFacade(_factory);
        facade.CreateBankAccount(1, "Account1", 500m);
        
        facade.DeleteBankAccount(1);
        
        Assert.Empty(facade.GetAllBankAccounts());
    }

    [Fact]
    public void UpdateBankAccountBalanceTest()
    {
        var facade = new BankAccountFacade(_factory);
        facade.CreateBankAccount(1, "Account1", 1000m);
        facade.CreateBankAccount(2, "Account2", 500m);

        var result = facade.UpdateBankAccountBalance(1, 2, 200m);
        
        Assert.True(result);
        Assert.Equal(800m, facade.GetBankAccount(1).Balance);
        Assert.Equal(700m, facade.GetBankAccount(2).Balance);
    }

    [Fact]
    public void UpdateBankAccountBalanceInvalidTest()
    {
        var facade = new BankAccountFacade(_factory);
        facade.CreateBankAccount(1, "Account1", 1000m);
        
        var result = facade.UpdateBankAccountBalance(1, 99, 200m);
        
        Assert.False(result);
    }
}