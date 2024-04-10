using System.Text.RegularExpressions;
using Common;

//var structRegex = new Regex(@".*loco_global<([\w\d:\(\) \+\*\-\/\[\]_]+), (0x[a-fA-F0-9]{6,9})> (.*);.*", RegexOptions.IgnoreCase | RegexOptions.Singleline);

var structRegex = new Regex(@"struct (\w+)\r\n((?:.*\r\n)+\s+)};", RegexOptions.IgnoreCase);

var objectCodeFiles = Directory.EnumerateFiles($@"{Constants.OpenLocoSourcePath}\src\", "*", SearchOption.AllDirectories);
foreach (var codeFile in objectCodeFiles)
{
	var filename = Path.GetFileNameWithoutExtension(codeFile);
	if (Path.GetExtension(codeFile) == ".h" && filename.EndsWith("Object") && !filename.StartsWith("Object"))
	//f (extension == ".cpp" || extension == ".h")
	{
		Console.WriteLine($"parsing file {codeFile}");
		var fullText = File.ReadAllText(codeFile);
		var structText = structRegex.Matches(fullText)[0];
		var structName = structText.Groups[1];
		var innerData = structText.Groups[2];
		Console.WriteLine($"found struct {structName} with data {innerData}");
	}
}

Console.WriteLine("done");
Console.ReadLine();
