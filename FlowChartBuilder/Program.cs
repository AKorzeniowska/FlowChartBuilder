using FlowChartBuilder.Helpers;
using System;

namespace FlowChartBuilder
{
    class Program
    {
        static void Main(string[] args)
        {
            var path = AppDomain.CurrentDomain.BaseDirectory + @"testfiles\test2.txt";
            var list = TextFileParser.ParseText(path.Replace(@"FlowChartBuilder\bin\Debug\netcoreapp3.0\", ""));
            var grid = new BlockDistributor(list);
            grid.PrintGrid();
            grid.RemoveEmptyLines();
            grid.SetNodesPositions();
            grid.PrintGrid();

            var graph = new GraphMaker(grid.GetNodes(), grid.GetGrid()); ;
            graph.LineCreator();
            graph.RemoveMiddlePointsFromLines();
            Console.ReadKey();
        }
    }
}
