using FlowChartBuilder.Helpers;
using System;
using System.Diagnostics;

namespace FlowChartBuilder
{
    class Program
    {
        static void Main(string[] args)
        {
            var path = AppDomain.CurrentDomain.BaseDirectory + @"testfiles\test3.txt";
            var list = TextFileParser.ParseText(path.Replace(@"FlowChartBuilder\bin\Debug\netcoreapp3.0\", ""));
            var grid = new BlockDistributor(list);
            grid.RemoveEmptyLines();
            grid.SetNodesPositions();
            grid.PrintGrid();

            var graph = new GraphMaker(grid.GetNodes(), grid.GetGrid()); ;
            Stopwatch sw = new Stopwatch();
            sw.Start();
            graph.LineCreator();
            sw.Stop();
            Console.WriteLine("ElapsedTotal={0}", sw.Elapsed);
            graph.RemoveMiddlePointsFromLines();
            Console.ReadKey();
        }
    }
}
