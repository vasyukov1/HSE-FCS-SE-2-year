namespace BankHSE.Command;

interface ICommand
{
    void Execute();
    void Undo();
    void Redo();
}