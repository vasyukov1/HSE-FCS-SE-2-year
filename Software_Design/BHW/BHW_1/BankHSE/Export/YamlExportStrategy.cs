using System.Text;
using BankHSE.Models;
using YamlDotNet.Serialization;
using YamlDotNet.Serialization.NamingConventions;

namespace BankHSE.Export;

public class YamlExportStrategy : IExportStrategy
{
    public string ExportData(IEnumerable<BankAccount> accounts, IEnumerable<Operation> operations, IEnumerable<Category> categories)
    {
        var serializer = new SerializerBuilder()
            .WithNamingConvention(CamelCaseNamingConvention.Instance)
            .Build();

        var data = new
        {
            Accounts = accounts,
            Operations = operations,
            Categories = categories
        };

        return serializer.Serialize(data);
    }
}