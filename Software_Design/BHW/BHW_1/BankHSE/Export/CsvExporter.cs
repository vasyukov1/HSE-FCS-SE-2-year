namespace BankHSE.Export;

public class CsvExporter : Exporter
{
    public CsvExporter() : base(new LoggingExportDecorator(new TimeExportDecorator(new CsvExportStrategy()))) {}
}