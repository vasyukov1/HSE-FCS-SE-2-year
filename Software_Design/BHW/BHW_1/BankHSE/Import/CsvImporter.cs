namespace BankHSE.Import;

public class CsvImporter : Importer
{
    public CsvImporter() : base(new CsvImportStrategy()) {}
}