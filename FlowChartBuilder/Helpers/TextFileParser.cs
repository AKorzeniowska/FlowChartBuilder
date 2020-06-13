using FlowChartBuilder.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Text;
using System.Text.RegularExpressions;

namespace FlowChartBuilder.Helpers
{
    public class TextFileParser
    {
        public static List<INode> ParseText(string filePath)
        {
            List<INode> createdNodes = new List<INode>();
            string line;
            System.IO.StreamReader file = new System.IO.StreamReader(filePath);
            //dispose of first line
            file.ReadLine();
            while ((line = file.ReadLine()) != null)
            {
                System.Console.WriteLine(line);
                //createdNodes.Add(GetNodeFromData(line.Split(' ')));
                createdNodes.Add(GetNodeFromGSAData(line.Split((char[])null, StringSplitOptions.RemoveEmptyEntries)));
            }
            file.Close();

            if (createdNodes.Find(x => x.GetId() == 0) != null)
            {
                createdNodes.ForEach(x => x.IncreaseId());
            }

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
                    decisionNode.AddLeftNode(int.Parse(nodeData[3]), nodeData[5]);
                    decisionNode.AddRightNode(int.Parse(nodeData[2]), nodeData[4]);
                    return decisionNode;
                case "Proc":
                    var processNode = new ProcessNode(int.Parse(nodeData[0]));
                    processNode.AddFollowingNode(int.Parse(nodeData[2]));
                    return processNode;
                default:
                    return null;
            }
        }

        private static INode GetNodeFromGSAData(string[] nodeData)
        {
            var nodeType = nodeData[1].ToLower();
            if (nodeType == "begin")
            {
                var startingNode = new StartingNode(int.Parse(nodeData[0]));
                startingNode.AddFollowingNode(int.Parse(nodeData[2]));
                return startingNode;
            }
            else if (nodeType == "end")
            {
                return new EndingNode(int.Parse(nodeData[0]));
            }
            else if (new Regex(@"y[0-9]+").IsMatch(nodeType))
            {
                var processNode = new ProcessNode(int.Parse(nodeData[0]));
                processNode.AddFollowingNode(int.Parse(nodeData[2]));
                processNode.SetName(nodeData[1]);
                return processNode;
            }
            else if (new Regex(@"x[0-9]+").IsMatch(nodeType))
            {
                var decisionNode = new DecisionNode(int.Parse(nodeData[0]));
                decisionNode.AddLeftNode(int.Parse(nodeData[3]));
                decisionNode.AddRightNode(int.Parse(nodeData[2]));
                decisionNode.SetName(nodeData[1]);
                return decisionNode;
            }
            else return null;
        }
    }
}
