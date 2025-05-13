using System.Text;
using Xunit.Abstractions;

namespace StudIS.Common.Tests;

public class XUnitTestOutputConverter(ITestOutputHelper output) : TextWriter
{
    public override Encoding Encoding => Encoding.UTF8;

    public override void WriteLine(string? line) => output.WriteLine(line);

    public override void WriteLine(string format, params object?[] args) => output.WriteLine(format, args);
}