global using LanguageFile = System.Collections.Generic.Dictionary<int, string>;
using System.Text.RegularExpressions;
using Common;

var stringids = File.ReadAllLinesAsync(@$"{Constants.OpenLocoSourcePath}\src\OpenLoco\src\Localisation\StringIds.h");
var engbyml = File.ReadAllLinesAsync(@$"{Constants.OpenLocoSourcePath}\data\language\en-GB.yml");

stringids.Wait();
engbyml.Wait();

var stringIdDict = ParseStringIds(stringids.Result);
var enGbYaml = Util.ParseLanguageFile(engbyml.Result);

// format tooltips
foreach (var v in stringIdDict)
{
	if (!v.Value.Contains("tooltip"))
	{
		continue;
	}

	if (!v.Value.StartsWith("tooltip"))
	{
		Console.WriteLine("[Warning] tooltip name doesn't start with 'tooltip'");
	}

	if (!enGbYaml.ContainsKey(v.Key))
	{
		Console.WriteLine($"[Error] StringIds.h doesn't have a corresponding string in en-GB.yml '{v.Key}'-'{v.Value}'");
	}

	//Console.WriteLine($"Mapping '{v.Value}' to '{enGbYaml[v.Key]}'");

	if (!enGbYaml[v.Key].StartsWith("{SMALLFONT}{COLOUR BLACK}") && !enGbYaml[v.Key].StartsWith("\"{SMALLFONT}{COLOUR BLACK}"))
	{
		Console.WriteLine($"[Warning] tooltip {v.Key} doesn't have correct formatting - '{enGbYaml[v.Key]}'");
	}
}

// do more validation logic here
Console.ReadLine();

static LanguageFile ParseStringIds(string[] lines)
{
	var string_ids = lines.Select(s => s.Split('='));

	var stringIdDict = new LanguageFile();
	foreach (var v in string_ids)
	{
		if (!v[0].Contains("constexpr") || !v[0].Contains("string_id"))
		{
			continue;
		}

		if (!v[0].Contains("constexpr string_id"))
		{
			Console.WriteLine($"[Error] each string_id must be declared as 'constexpr string_id' code=\"{v[0].Trim()}\"");
			continue;
		}

		if (v.Length != 2)
		{
			Console.WriteLine($"[Error] each string_id must have 1 '=' sign code=\"{v[0].Trim()}\"");
			continue;
		}

		var x = v[1]
			.Split(';')[0]
			.Replace(';', ' ')
			.Replace('(', ' ')
			.Replace(')', ' ')
			.Replace("UNUSED_STR", string.Empty)
			.Trim();

		if (!int.TryParse(x, out var string_id))
		{
			Console.WriteLine($"[Error] Couldn't parse string_id format: '{x}'");
			continue;
		}

		var variable_name = v[0].Replace("constexpr string_id", string.Empty).Trim();

		if (stringIdDict.ContainsKey(string_id))
		{
			Console.WriteLine($"[Error] duplicate string_id found: {string_id}");
			continue;
		}

		stringIdDict.Add(string_id, variable_name);
	}

	return stringIdDict;
}