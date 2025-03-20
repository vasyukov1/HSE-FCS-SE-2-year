# Software Design | BHW 1

Author: Alexander Vasyukov

## Description of project "Bank HSE"

This project is a console app for management of money in C# using SOLID, DI, GRASP and other patterns.

Functions:
1. Create account
2. Delete account
3. Show accounts
4. Create category
5. Delete category
6. Show categories
7. Create operation
8. Delete operation
9. Show operations
10. Show operations with account ID
11. Export data
12. Import data
13. Undo command
14. Redo command
0. Exit

## Project launch

1.  Clone git repository:
```bash
git clone https://github.com/vasyukov1/HSE-FCS-SE-2-year
cd HSE-FCS-SE-2-year/Software_Design/BHW/BHW_1
```
2. Run app
```bash
cd BankHSE
dotnet run
```
3. Run tests
```bash
cd ../BankHSE/BankHSE.Tests
dotnet
```
4. Get test report
```bash
dotnet test /p:CollectCoverage=true /p:CoverletOutputFormat=opencover
```

## Patterns

The project follows SOLID and GRASP principles and includes a DI container. It also follows the next design patterns:
- **Facade**: There are 3 facades—BankAccountFacade, CategoryFacade, and OperationFacade—that contain the core logic of the entities BankAccount, Category, and Operation.
- **Factory**: BankFactory creates accounts, categories, and operations.
- **Command**: Each user scenario is represented by a separate command.
- **Template Method**: Used for importing and exporting data in different formats.
- **Strategy**: Used for selecting actions and data formats during export and import.
- **Decorator**: Wrapper for logging data during export.

Functionality:
1. Create accounts, delete accounts, and view all accounts (id, name, balance).
2. Create categories, delete categories, and view all categories (id, name, type). It is important to create at least one category before creating an operation.
3. Create operations, delete operations (and refund money to accounts), view all operations, and view operations related to a specific account.
4. Export data in CSV, JSON, and YAML formats.
5. Import data in CSV and JSON formats.
6. Undo command.
7. Redo command.

Also there are tests, and the test coverage is approximately 65.99%.