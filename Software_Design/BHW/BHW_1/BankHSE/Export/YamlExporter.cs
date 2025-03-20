namespace BankHSE.Export;
public class YamlExporter : Exporter
{
    public YamlExporter() : base(new LoggingExportDecorator(new TimeExportDecorator(new CsvExportStrategy()))) {}
}