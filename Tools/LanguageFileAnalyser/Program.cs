global using LanguageFile = System.Collections.Generic.Dictionary<int, string>;
using Common;

Dictionary<string, LanguageFile> languageFiles = [];
Parallel.ForEach(Directory.EnumerateFiles(@$"{Constants.OpenLocoSourcePath}\data\language"), file => languageFiles.Add(Path.GetFileNameWithoutExtension(file), Util.ParseLanguageFile(File.ReadAllLines(file))));

const string baseFilename = "en-GB";
var baseFile = languageFiles[baseFilename];
const string linebreak = "----------";
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
Console.ReadLine();