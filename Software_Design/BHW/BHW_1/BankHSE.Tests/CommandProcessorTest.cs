using BankHSE.Command;
using BankHSE.Facade;
using BankHSE.Models;
using Xunit;

namespace BankHSE.Tests;

[Collection("CommandProcessorTests")]
public class CommandProcessorTest
{
    private readonly BankAccountFacade _accountFacade;
    private readonly OperationFacade _operationFacade;
    private readonly CategoryFacade _categoryFacade;
    private readonly CommandProcessor _commandProcessor;

    public CommandProcessorTest()
    {
        var factory = new BankFactory();
        _accountFacade = new BankAccountFacade(factory);
        _operationFacade = new OperationFacade(factory, _accountFacade);
        _categoryFacade = new CategoryFacade(factory);
        _commandProcessor = new CommandProcessor(_accountFacade, _operationFacade, _categoryFacade);
    }

    [Fact]
    public void Process1Test()
    {
        using (new ConsoleInputRedirect("1\n100\n"))
        {
            _commandProcessor.Process("1");
        }
        
        var accounts = _accountFacade.GetAllBankAccounts();
        Assert.NotEmpty(accounts);
    }

    [Fact]
    public void Process2Test()
    {
        using (new ConsoleInputRedirect("1\n"))
        {
            _accountFacade.CreateBankAccount(1, "A", 100m);
            _commandProcessor.Process("2");
        }
        
        var accounts = _accountFacade.GetAllBankAccounts();
        Assert.Empty(accounts);
    }

    [Fact]
    public void Process3Test()
    {
        var exception = Record.Exception(() => _commandProcessor.Process("3"));
        Assert.Null(exception);
    }

    [Fact]
    public void Process4Test()
    {
        using (new ConsoleInputRedirect("1\n1\n"))
        {
            _commandProcessor.Process("4");
        }

        var categories = _categoryFacade.GetAllCategories();
        Assert.Single(categories);
    }

    [Fact]
    public void Process5Test()
    {
        using (new ConsoleInputRedirect("1\n"))
        {
            _categoryFacade.CreateCategory(1, "Food", CategoryType.Expense);
            _commandProcessor.Process("5");
        }

        var categories = _categoryFacade.GetAllCategories();
        Assert.Empty(categories);
    }

    [Fact]
    public void Process6Test()
    {
        var exception = Record.Exception(() => _commandProcessor.Process("6"));
        Assert.Null(exception);
    }

    [Fact]
    public void Process7Test()
    {
        using (new ConsoleInputRedirect("1\n1\n2\n20\nFor Dasha\n1\n"))
        {
            _accountFacade.CreateBankAccount(1, "A", 100m);
            _accountFacade.CreateBankAccount(2, "B", 200m);
            _categoryFacade.CreateCategory(1, "Food", CategoryType.Expense);

            _commandProcessor.Process("7");
        }
        var operations = _operationFacade.GetAllOperations();
        Assert.NotEmpty(operations);
    }

    [Fact]
    public void Process8Test()
    {
        using (new ConsoleInputRedirect("1\n"))
        {
            _accountFacade.CreateBankAccount(1, "Sasha", 100m);
            _accountFacade.CreateBankAccount(2, "Dasha", 200m);
            _categoryFacade.CreateCategory(1, "Food", CategoryType.Expense);
            _operationFacade.CreateOperation(1, 50m, DateTime.Now, "Test", 1, 2, 1, OperationType.Income);

            _commandProcessor.Process("8");
        }

        var operations = _operationFacade.GetAllOperations();
        Assert.Empty(operations);
    }

    [Fact]
    public void Process9Test()
    {
        var exception = Record.Exception(() => _commandProcessor.Process("9"));
        Assert.Null(exception);
    }

    [Fact]
    public void Process10Test()
    {
        using (new ConsoleInputRedirect("1\n"))
        {
            var exception = Record.Exception(() => _commandProcessor.Process("10"));
            Assert.Null(exception);
        }
    }

    [Fact]
    public void Process11Test()
    {
        using (new ConsoleInputRedirect("1\n"))
        {
            var exception = Record.Exception(() => _commandProcessor.Process("11"));
            Assert.Null(exception);
        }
    }

    [Fact]
    public void Process12Test()
    {
        using (new ConsoleInputRedirect("1\nexport.csv\n"))
        {
            var exception = Record.Exception(() => _commandProcessor.Process("12"));
            Assert.Null(exception);
        }
    }

    [Fact]
    public void Process13Test()
    {
        using (new ConsoleInputRedirect("Dasha\n1000\n1"))
        {
            _commandProcessor.Process("1");
            var exception = Record.Exception(() => _commandProcessor.Process("13"));
            Assert.Null(exception);
        }       
    }

    [Fact]
    public void Process14Test()
    {
        using (new ConsoleInputRedirect("Dasha\n1000\n1"))
        {
            _commandProcessor.Process("1");
            _commandProcessor.Process("13");

            var exception = Record.Exception(() => _commandProcessor.Process("14"));
            Assert.Null(exception);
        }
    }

    [Fact]
    public void ProcessUnknownCommandTest()
    {
        var exception = Record.Exception(() => _commandProcessor.Process("999"));
        Assert.Null(exception);
    }
}

[CollectionDefinition("CommandProcessorTests")]
public class CommandProcessorTestCollection : ICollectionFixture<CommandProcessorTest> {}

public class ConsoleInputRedirect : IDisposable
{
    private readonly TextReader _originalInput;

    public ConsoleInputRedirect(string input)
    {
        _originalInput = Console.In;
        Console.SetIn(new StringReader(input));
    }

    public void Dispose()
    {
        Console.SetIn(_originalInput);
    }
}