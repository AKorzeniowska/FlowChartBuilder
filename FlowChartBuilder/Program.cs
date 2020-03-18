using FlowChartBuilder.Helpers;
using System;

namespace FlowChartBuilder
{
    class Program
    {
        static void Main(string[] args)
        {
            var path = AppDomain.CurrentDomain.BaseDirectory + @"testfiles\test3.txt";
            var list = TextFileParser.ParseText(path.Replace(@"FlowChartBuilder\bin\Debug\netcoreapp3.0\", ""));
            var grid = new BlockSpawner(list);
            grid.PrintGrid();
            grid.RemoveEmptyLines();
            grid.PrintGrid();
        
            var graph = new GraphMaker(list, grid.GetGrid());
            graph.LineCreator();
            Console.ReadKey();
        }
    }
}
