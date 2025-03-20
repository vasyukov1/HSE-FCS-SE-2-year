namespace BankHSE.Export;

public class CsvExporter : Exporter
{
    public CsvExporter() : base(new CsvExportStrategy()) {}
}