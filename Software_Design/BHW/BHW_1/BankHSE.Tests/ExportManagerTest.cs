using BankHSE.Export;
using BankHSE.Models;
using Xunit;
using System;
using System.Collections.Generic;
using System.IO;

namespace BankHSE.Tests;

public class ExportManagerTests
{
    [Fact]
    public void ExportData1Test()
    {
        var accounts = new List<BankAccount> { new BankAccount(1, "Dasha", 100m) };
        var operations = new List<Operation> { new Operation(1, 50m, DateTime.Now, "Test", 1, 2, 1, OperationType.Income) };
        var categories = new List<Category> { new Category(1, "Food", CategoryType.Expense) };

        var exportManager = new ExportManager(accounts, operations, categories);

        var input = new StringReader("1\n");
        Console.SetIn(input);

        var filePath = "export.csv";
        if (File.Exists(filePath))
        {
            File.Delete(filePath);
        }

        exportManager.ExportData();

        Assert.True(File.Exists(filePath));
    }

    // [Fact]
    // public void ExportData2Test()
    // {
    //     var accounts = new List<BankAccount> { new BankAccount(1, "Dasha", 100m) };
    //     var operations = new List<Operation> { new Operation(1, 50m, DateTime.Now, "Test", 1, 2, 1, OperationType.Income) };
    //     var categories = new List<Category> { new Category(1, "Food", CategoryType.Expense) };

    //     var exportManager = new ExportManager(accounts, operations, categories);

    //     var input = new StringReader("2\n");
    //     Console.SetIn(input);

    //     var filePath = "export.json";
    //     if (File.Exists(filePath))
    //     {
    //         File.Delete(filePath);
    //     }

    //     exportManager.ExportData();

    //     Assert.True(File.Exists(filePath));
    // }

    [Fact]
    public void ExportData3Test()
    {
        var accounts = new List<BankAccount> { new BankAccount(1, "Dasha", 100m) };
        var operations = new List<Operation> { new Operation(1, 50m, DateTime.Now, "Test", 1, 2, 1, OperationType.Income) };
        var categories = new List<Category> { new Category(1, "Food", CategoryType.Expense) };

        var exportManager = new ExportManager(accounts, operations, categories);

        var input = new StringReader("3\n");
        Console.SetIn(input);

        var filePath = "export.yaml";
        if (File.Exists(filePath))
        {
            File.Delete(filePath);
        }

        exportManager.ExportData();

        Assert.True(File.Exists(filePath));
    }
}