namespace BankHSE.Export;

public class JsonExporter : Exporter
{
    public JsonExporter() : base(new JsonExportStrategy())
    {
    }
}