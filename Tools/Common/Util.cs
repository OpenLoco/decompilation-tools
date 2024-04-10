global using LanguageFile = System.Collections.Generic.Dictionary<int, string>;
using System.Text.RegularExpressions;

namespace Common
{
	public static class Util
	{
		public static LanguageFile ParseLanguageFile(string[] lines)
		{
			var output = new LanguageFile();

			var sanitised = lines
				.Where(l => !string.IsNullOrEmpty(l))
				.Select(l => l.Trim())
				.Where(l => !l.StartsWith('#') && !l.EndsWith(':'));

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
	}
}
