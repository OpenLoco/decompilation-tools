using System.Text.RegularExpressions;
using Common;

var codeFiles = Directory.EnumerateFiles($@"{Constants.OpenLocoSourcePath}\src\OpenLoco\src\", "*", SearchOption.AllDirectories);

List<LocoGlobal> locoGlobalList = [];

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
			return new LocoGlobal(
				Path.GetFileName(codeFile),
				l.lineNumber,
				m[0].Groups[1].Value,
				sanitiseHexAddr(m[0].Groups[2].Value),
				m[0].Groups[3].Value);
		}));
}

var locoGlobalDict = locoGlobalList
	.ToLookup(t => t.Address, t => t)
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

// log bad loco_global variable names
foreach (var lg in locoGlobalDict)
{
	var bad = lg.Value.Where(r => !r.VariableName.StartsWith('_'));
	foreach (var badlg in bad)
	{
		Console.WriteLine($"bad - {badlg.VariableName}, {badlg.Filename}");
	}
}

// gamestate struct offsets
var gamestateTop = Convert.ToInt32("0x00525E18", 16);
var gamestateSize = Convert.ToInt32("0x004A0644", 16);
var gamestateBottom = gamestateTop + gamestateSize;

// write to file
var ordered = locoGlobalDict.OrderByDescending(kv => kv.Value.Count);
var strings = new List<string>
{
	$"Unique count: {ordered.Count()} (the number of unique offsets used by loco_global or addr",
	$"Multiuse count: {ordered.Count(a => a.Value.Count > 1)} (the number of unique loco_globals that are used more than once)",
	$"Total count: {ordered.Sum(a => a.Value.Count)} (the total number of loco_globals used)"
};

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

	// print every occurrence of the loco_global and its corresponding file+line number
	if (verbose)
	{
		foreach (var occurence in kv.Value)
		{
			strings.Add($"\t - {occurence.Type} {occurence.Filename}:{occurence.LineNumber}");
		}
	}
}

File.WriteAllLines("./output.txt", strings);

Console.WriteLine();
Console.WriteLine("Done");
Console.ReadLine();

// done

static string sanitiseHexAddr(string addr) => $"0x{addr.Trim().Replace("0x", string.Empty).ToUpper().TrimStart('0').PadLeft(8, '0')}"; // need c#11 for multiline interpolated strings

internal record LocoGlobal(string Filename, int LineNumber, string Type, string Address, string VariableName);