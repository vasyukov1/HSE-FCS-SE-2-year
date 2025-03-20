namespace BankHSE.Export;

public class JsonExporter : Exporter
{
    public JsonExporter() : base(new LoggingExportDecorator(new TimeExportDecorator(new CsvExportStrategy()))) {}
}