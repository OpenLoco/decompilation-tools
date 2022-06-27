using System.Text.RegularExpressions;
using Common;

using LanguageFile = System.Collections.Generic.Dictionary<int, string>;

Dictionary<string, LanguageFile> languageFiles = new();
Parallel.ForEach(Directory.EnumerateFiles(@$"{Constants.OpenLocoSourcePath}\data\language"), file => languageFiles.Add(Path.GetFileNameWithoutExtension(file), ParseLanguageFile(File.ReadAllLines(file))));

const string baseFilename = "en-GB";
var baseFile = languageFiles[baseFilename];
const string linebreak = @"----------";
var output = new List<string>();

foreach (var kv in languageFiles.Where(f => f.Key != baseFilename))
{
	var keyDiffA = baseFile.Keys.Except(kv.Value.Keys);
	var keyDiffB = kv.Value.Keys.Except(baseFile.Keys);

	foreach (var a in keyDiffA)
		output.Add($"{baseFilename} contains id {a} but {kv.Key} does not");

	output.Add(linebreak);

	foreach (var b in keyDiffB)
		output.Add($"{kv.Key} contains id {b} but {baseFilename} does not");

	output.Add(linebreak);
}

File.WriteAllLines("./output.txt", output);

Console.WriteLine("Finished");

// same as StringIdValidator::ParseYaml()
static LanguageFile ParseLanguageFile(string[] lines)
{
	var output = new LanguageFile();

	var sanitised = lines
		.Where(l => !string.IsNullOrEmpty(l))
		.Select(l => l.Trim())
		.Where(l => !l.StartsWith('#'))
		.Where(l => !l.EndsWith(':'));

	foreach (var line in sanitised)
	{
		var match = Regex.Match(line, @"(\d+): (.*$)");

		if (match.Success)
		{
			var id = int.Parse(match.Groups[1].Captures[0].Value);
			var str = match.Groups[2].Captures[0].Value;

			output.Add(id, str);
		}
		else
		{
			Console.WriteLine($"[Warning] couldn't recognise string '{line}'");
		}
	}

	return output;
}