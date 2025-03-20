using BankHSE.Facade;

namespace BankHSE.Import;

public class JsonImporter : Importer
{
    public JsonImporter() : base(new JsonImportStrategy()) {}
}