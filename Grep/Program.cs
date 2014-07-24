using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using System.IO;

using ExtraContainers;

namespace Grep
{
    class Program
    {
        static void Usage(string msg)
        {
            Console.Error.WriteLine(msg);
            Console.Error.WriteLine(@"
Usage: Grep /A=n /B=n /C=n /l regex filenames ...

Print lines matching a regular expression with optional context.

Options:

    /A=n    print n lines of context after a match
    /B=n    print n lines of context before a match
    /C=n    print n lines of context before and after a match
    /l      print filename and linenumber along with the matching lines

    n defaults to 0 and if conflicting options are given the maximum is taken.
");
        }

        static void Print(string line, bool show, int linenumber, string filename)
        {
            if (show)
            {
                Console.WriteLine("{0}:{1} {2}", filename, linenumber, line);
            }
            else
            {
                Console.WriteLine("{0}", line);
            }
        }

        static void Main(string[] args)
        {
            bool linenumbers = false;
            UInt16 before = 0;
            UInt16 after = 0;
            UInt16 context = 0;
            Regex r = null;

            int i = 0;

            try
            {
                #region option processing
                while (args[i].StartsWith("/"))
                {
                    string[] parts = args[i].Split('=');
                    switch (parts.Length)
                    {
                        case 1:
                            switch (parts[0])
                            {
                                case "/l":
                                    linenumbers = true;
                                    break;
                                default: throw new ArgumentException("unkown option: " + args[i]);
                            }
                            break;
                        case 2:
                            switch (parts[0])
                            {
                                case "/A":
                                    if (!UInt16.TryParse(parts[1], out after))
                                    {
                                        throw new ArgumentException("illegal number format: " + args[i]);
                                    }
                                    break;
                                case "/B":
                                    if (!UInt16.TryParse(parts[1], out before))
                                    {
                                        throw new ArgumentException("illegal number format: " + args[i]);
                                    }
                                    break;
                                case "/C":
                                    if (!UInt16.TryParse(parts[1], out context))
                                    {
                                        throw new ArgumentException("illegal number format: " + args[i]);
                                    }
                                    break;
                                default: throw new ArgumentException("unkown option: " + args[i]);
                            }
                            break;
                        default: throw new ArgumentException("illegal option format: " + args[i]);
                    }
                    i++;
                }
            #endregion

                before = Math.Max(before, context);
                after = Math.Max(after, context);

                if (i >= args.Length) throw new ArgumentException("missing regular expression");
                try
                {
                    r = new Regex(args[i++]);
                }
                catch
                {
                    throw new ArgumentException("illegal regular expression");
                }
            }
            catch(Exception e)
            {
                Usage(e.Message);
                Environment.Exit(-1);
            }

            #region ringbuffer initialization
            int size = before + after + 1;
            var lines = new RingBuffer<string>(size);
            var printed = new RingBuffer<bool>(size);
            #endregion

            if (i >= args.Length)
            {
                Usage("missing arguments");
                Environment.Exit(-2);
            }

            string filename=""; 
            try
            {
                while (i < args.Length)
                {
                    filename = args[i++];
                    int linenumber = 0;

                    #region scan a single file
                    var f = new StreamReader(File.OpenRead(filename));
                    int nafter = 0;
                    while (!f.EndOfStream)
                    {
                        var line = f.ReadLine();
                        linenumber++;

                        lines.Add(line);
                        printed.Add(false);

                        Match m = r.Match(line);
                        if (m.Success)
                        {
                            for (int n = Math.Min(before,lines.Count-1); n >= 0; n--) // 0 is the recently matched line itself
                            {
                                if (!printed[n])
                                {
                                    Print(lines[n], linenumbers, linenumber, filename);
                                    printed[n] = true;
                                }
                            }
                            nafter = after; // mark number of lines in trailing context 
                        }
                        else if (nafter > 0)
                        {
                            nafter--;
                            Print(line, linenumbers, linenumber, filename);
                            printed[0] = true;
                        }

                    }
                    f.Close();
                    f.Dispose();
                    lines.Clear();
                    printed.Clear();
                    #endregion
                }
            }catch(IOException){
                Console.Error.WriteLine("could not read file {0}",filename);
                Environment.Exit(-3);
            }
        }
    } 

}
