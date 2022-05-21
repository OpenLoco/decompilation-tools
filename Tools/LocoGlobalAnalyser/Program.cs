using System.Text.RegularExpressions;
using Common;

var codeFiles = Directory.EnumerateFiles(Constants.OpenLocoSourcePath, "*", SearchOption.AllDirectories);

List<loco_global> locoGlobalList = new();

var regex = new Regex(@".*loco_global<([\w\d:\(\) \+\*\-\/\[\]_]+), (0x[a-fA-F0-9]{6,9})> (.*);.*", RegexOptions.IgnoreCase | RegexOptions.Singleline);

// parse input
foreach (var codeFile in codeFiles)
{
	var lines = File.ReadAllLines(codeFile);
	locoGlobalList.AddRange(lines
		.Select((line, lineNumber) => (line, lineNumber))
		.Where(l => regex.IsMatch(l.line))
		.Select(l =>
		{
			var m = regex.Matches(l.line);
			return new loco_global(
				Path.GetFileName(codeFile),
				l.lineNumber,
				m[0].Groups[1].Value,
				sanitiseHexAddr(m[0].Groups[2].Value),
				m[0].Groups[3].Value);
		}));
}

var locoGlobalDict = locoGlobalList
	.ToLookup(t => t.address, t => t)
	.ToDictionary(t => t.Key, t => t.ToList());

// console log
foreach (var lg in locoGlobalDict)
{
	Console.WriteLine($"{lg.Key}");
	foreach (var lgg in lg.Value)
	{
		Console.WriteLine($"  - {lgg}");
	}
}

// gamestate struct offsets
var gamestateTop = Convert.ToInt32("0x00525E18", 16);
var gamestateSize = Convert.ToInt32("0x004A0644", 16);
var gamestateBottom = gamestateTop + gamestateSize;

// write to file
var ordered = locoGlobalDict.OrderByDescending(kv => kv.Value.Count);
var strings = new List<string>();
strings.Add($"Total count: {ordered.Count()}");

var verbose = true;

foreach (var kv in ordered)
{
	var gameStateOffset = string.Empty;
	var lgOffset = Convert.ToInt32(kv.Key, 16);
	if (lgOffset >= gamestateTop && lgOffset < gamestateBottom)
	{
		gameStateOffset = $" - gamestate offset = 0x{lgOffset - gamestateTop:X}";
	}

	strings.Add($"{kv.Key} - {kv.Value.Count}{gameStateOffset}");

	// print every occurence of the loco_global and its corrresponding file+line number
	if (verbose)
	{
		foreach (var occurence in kv.Value)
		{
			strings.Add($"\t - {occurence.type} {occurence.filename}:{occurence.lineNumber}");
		}
	}
}

File.WriteAllLines("./output.txt", strings);

Console.WriteLine();
Console.WriteLine("Done");
Console.ReadLine();

// done

string sanitiseHexAddr(string addr) => $"0x{addr.Trim().Replace("0x", string.Empty).ToUpper().TrimStart('0').PadLeft(8, '0')}"; // need c#11 for multiline interpolated strings

internal record loco_global(string filename, int lineNumber, string type, string address, string variableName);