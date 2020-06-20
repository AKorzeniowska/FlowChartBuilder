using FlowChartBuilder;
using FlowChartBuilder.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace FlowChartBuilder.Helpers
{
    public static class ChartDLLConnector
    {
        public static void CreateGraphData(String filename, ref List<INode> nodes, ref List<LineModel> directLines, ref List<LineModel> indirectLines)
        {
            createGraphData(filename);
            nodes = TextFileParser.ParseTextToNodes(filename);
            TextFileParser.ParseTextToLines(filename, ref directLines, ref indirectLines);
            TextFileParser.SetOccupiedSides(nodes, directLines);
        }

        [DllImport(@"..\..\ChartDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void createGraphData([MarshalAs(UnmanagedType.LPStr)] String arg);

        [DllImport(@"..\..\ChartDLL.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int testDLL(int a);
    }
}
