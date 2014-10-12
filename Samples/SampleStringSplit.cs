class TestStringSplit
{
    static void Main()
    {
        char[] delimiterChars = { ' ', ',', '.', ':', '\t', '\n' };

        string text = "one\ttwo three:four,five six seven\n";
        System.Console.WriteLine("Original text: '{0}'", text);

        string[] words = text.Split(delimiterChars);
        System.Console.WriteLine("{0} words in text:", words.Length);

        foreach (string s in words)
        {
            System.Console.WriteLine(s);
        }

        // Keep the console window open in debug mode.
        System.Console.WriteLine("Press any key to exit.");
        System.Console.ReadKey();
    }
}