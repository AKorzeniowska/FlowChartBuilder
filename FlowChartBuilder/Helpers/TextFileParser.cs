using FlowChartBuilder.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;

namespace FlowChartBuilder.Helpers
{
    public class TextFileParser
    {
        public static List<INode> ParseText(string filePath)
        {
            List<INode> createdNodes = new List<INode>();
            string line;
            System.IO.StreamReader file = new System.IO.StreamReader(filePath);
            while ((line = file.ReadLine()) != null)
            {
                System.Console.WriteLine(line);
                createdNodes.Add(GetNodeFromData(line.Split(' ')));
            }
            file.Close();

            return createdNodes;
        }

        private static INode GetNodeFromData(string[] nodeData)
        {
            switch (nodeData[1])
            {
                case "Begin":
                    var startingNode = new StartingNode(int.Parse(nodeData[0]));
                    startingNode.AddFollowingNode(int.Parse(nodeData[2]));
                    return startingNode;
                case "End":
                    return new EndingNode(int.Parse(nodeData[0]));
                case "Decis":
                    var decisionNode = new DecisionNode(int.Parse(nodeData[0]));
                    decisionNode.AddLeftNode(int.Parse(nodeData[2]), nodeData[4]);
                    decisionNode.AddRightNode(int.Parse(nodeData[3]), nodeData[5]);
                    return decisionNode;
                case "Proc":
                    var processNode = new ProcessNode(int.Parse(nodeData[0]));
                    processNode.AddFollowingNode(int.Parse(nodeData[2]));
                    return processNode;
                default:
                    return null;
            }
        }
    }
}
